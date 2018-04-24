#include "stdafx.h"
#include "SearchActions.h"

bool SearchActions::ShouldWriteIntoState(const SearchCoords* const current, const SearchCoords* const target,int cost, int lifeLost, int ropeUsed)
{
	if (target->completePrice == 0) return true;
	if (target->currentDistance > current->currentDistance + cost) return true;
	if (target->spelunkerState.lifeCount < current->spelunkerState.lifeCount - lifeLost) return true;
	if (target->spelunkerState.ropeCount < current->spelunkerState.ropeCount - ropeUsed) return true;
	return false;
}

int SearchActions::GetCost(int x, int y, const SearchCoords* const prevCoord)
{
	int dx = abs(prevCoord->x - x);
	int dy = abs(y - prevCoord->y);
	if (dx != 0 && dy != 0) --dy;
	return dx + dy;
}

// ------ SIDE MOVE ------
vector<SearchCoords*> SearchActions::SideMove::GetNextNodes(SearchCoords* startState)
{
	if (startState->notToSide) return vector<SearchCoords*>();
	if (map.NodeIsClimable(*startState) && !map.NodeIsTerrain(startState->x, startState->y + 1)) return vector<SearchCoords*>();
	this->startState = startState;
	CurrentActionGetter = SearchActions::SideMove::GetAction;
	vector<SearchCoords*> rightDir = SideControl(1);
	vector<SearchCoords*> leftDir = SideControl(-1);
	rightDir.insert(rightDir.end(), leftDir.begin(), leftDir.end());
	return rightDir;
}
vector<SearchCoords*> SearchActions::SideMove::SideControl(int dx)
{
	vector<SearchCoords*> retCoords;
	vector<SearchCoords*> help;
	int x = startState->x;
	int y = startState->y;
	if (!map.NodeIsTerrain(x,y+1))
	{
		retCoords = Falling(dx, x, y, &buffer[x][y]);
		if (x == 0) return retCoords; // only return possible landing spaces - cant compute further
	}
	x += dx; // starting from next chunk
	while (!map.NodeIsTerrain(x,y))
	{	
		if (map.NodeIsClimable(x, y))
		{
			if (WriteToState(x, y, FallType::normal, &buffer[x - dx][y]))
			{
				help = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][y]);
				retCoords.insert(retCoords.end(), help.begin(), help.end());
				retCoords.push_back(&buffer[x][y]);
			}
			break;
		}
		else if (map.NodeIsUnknown(x, y))
		{
			if (WriteToState(x, y, FallType::toLand, &buffer[x - dx][y]))
			{
				retCoords.push_back(&buffer[x][y]);
			}
			break;
		}
		else if (!map.NodeIsTerrain(x,y+1))
		{
			help = Falling(dx, x, y, &buffer[x - dx][y]);
			retCoords.insert(retCoords.end(), help.begin(), help.end());
			if (x == 0) break; // further falling distance not clear -> devide actions
		}
		else // not falling - horizontal move
		{
			if (WriteToState(x, y, FallType::normal, &buffer[x - dx][y]))
			{
				buffer[x][y].notToSide = true; // no other side control from this block
				retCoords.push_back(&buffer[x][y]);
			}
			else break; // already visited state
		}
		x += dx;		
	}
	return retCoords;
}
vector<SearchCoords*> SearchActions::SideMove::Falling(int dx, int& x, int& y, SearchCoords* prevCoord)
{
	vector<SearchCoords*> retCoords;
	int botY;
	int firstY = y;
	bool columnBlocked = false;
	while (true)
	{
		botY = y;
		while (!map.NodeIsTerrain(x, botY))
		{
			if (map.NodeIsClimable(x, botY))
			{
				// finish at climeable - could be blocking
				if (WriteToState(x, botY, FallType::toLand, prevCoord))
				{
					vector<SearchCoords*> ret2 = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][botY]);
					retCoords.insert(retCoords.end(), ret2.begin(), ret2.end());
				}
				break;
			}
			else if (map.NodeIsUnknown(x, botY))
			{
				if(WriteToState(x, botY, FallType::toLand, prevCoord))
					retCoords.push_back(&buffer[x][botY]);
				break;
			}
			++botY; // find bot in current falling column
		}
		--botY; // landing chunk coordinate
		if (map.NodeIsTerrain(x,botY+1) && WriteToState(x, botY, FallType::toLand, prevCoord))
		{
			retCoords.push_back(&buffer[x][botY]);
		}
		x += dx; // move to next column
		if (y == firstY && startState->x != x-dx)
		{
			// x-dx - check, bot wasnt already falling - this case initial speed is missing
			// next checks will fail at firt condition
			if (map.NodeIsTerrain(x, y++) || map.NodeIsTerrain(x, y)) // add 1 to y
			{
				// next column blocked, for second column special case - 2 blocking blocks
				columnBlocked = true;
			}
		}
		else
		{
			if (y == firstY + 1) y += 1; // third column
			else if (y == firstY + 2) y += 3; // fourth column
			else y += 4; // every 4 block of height is falled distance longer - other columns
			if (y >= botY) // entring block for next collumn is unreachable
			{
				x -= dx; // previsou block had further reachable possition
				y = botY;
				break;
			}
			else if (map.NodeIsTerrain(x, y)) columnBlocked = true;
		} 
		if (columnBlocked)
		{
			if (y - firstY < 3)
			{
				SearchCoords* coords = HangingCheck(x, y, prevCoord); // check hanged blocks
				if (coords != nullptr) retCoords.push_back(coords);
			}

			while (map.NodeIsTerrain(x, y) && y <= botY) ++y; // scrolling down the blocking column of terrain
			if (y > botY) { y = botY; x -= dx; break; } // no reachable block -> previsou column was last one
			while (!map.NodeIsTerrain(x, y + 1)) ++y; // scrolling empty blocks -> falling in this column
			if (WriteToState(x, y, FallType::withWaiting, prevCoord)) // write into bottom of column with waiting
			{
				retCoords.push_back(&buffer[x][y]);
			}
			x = 0;
			break; // cant fall further
		}
	}
	return retCoords;
}
vector<SearchCoords*> SearchActions::SideMove::Falling(int dx, int& x, int& y, SearchCoords* startState, GetActionFunction GetAction)
{
	this->startState = startState;
	CurrentActionGetter = GetAction;
	vector<SearchCoords*> ret = move(Falling(dx, x, y, startState));
	CurrentActionGetter = SearchActions::SideMove::GetAction;
	return move(ret);
}
SearchCoords* SearchActions::SideMove::HangingCheck(int x, int y, SearchCoords* prevCoord)
{
	if (!map.NodeIsTerrain(x, y-1))
	{
		if(WriteToState(x, y-1, FallType::blocked, prevCoord))
			return &buffer[x][y - 1];
	}
	return nullptr;
}
bool SearchActions::SideMove::WriteToState(int x, int y, FallType type, SearchCoords* prevCoord)
{
	int cost = GetCost(x, y, prevCoord);
	if (type == FallType::blocked) cost += 2;
	int lifeLost;
	SearchCoords* state = &buffer[x][y];
	if (y - prevCoord->y > 7 || map.NodeIsUnknown(x,y)) lifeLost = 1;
	else lifeLost = 0;
	if (ShouldWriteIntoState(prevCoord, state, cost, lifeLost, 0))
	{
		state->SetCoords(x, y);
		state->Visit(startState, prevCoord->currentDistance + cost, move(CurrentActionGetter(x,y,prevCoord->x,type)), prevCoord->spelunkerState);
		if (lifeLost != 0) state->spelunkerState.lifeCount -= lifeLost; // too hight for surviving
		return true;
	}
	return false;
}

unique_ptr<ActionHandlerFactory> SearchActions::SideMove::GetAction(int x, int y, int prevX, FallType type)
{
	if (type == FallType::normal) 
		return make_unique<MoveToActionFactory>(x + 0.5, y);
	else if (type == FallType::withWaiting)
	{
		int dx = 1;
		if (prevX < x) dx = -1;
		unique_ptr<ActionListFactory> action = make_unique<ActionListFactory>(make_unique<MoveToActionFactory>(x + dx + 0.5, y));
		action->AddActionFactory(make_unique<WaitActionFactory>(x + dx + 0.5, y, waitTime));
		action->AddActionFactory(make_unique<MoveToActionFactory>(x + 0.5, y));
		return move(action);
	}
	else if (type == FallType::blocked)
	{
		//int dx = 1;
		//if (prevX < x) dx = -1;
		unique_ptr<ActionListFactory> action = make_unique<ActionListFactory>(make_unique<MoveToActionFactory>(x + 0.5, y));
		//action->AddActionFactory(make_unique<MoveForActionFactory>(x + dx + 0.5, y,-10*dx));
		action->AddActionFactory(make_unique<JumpToActionFactory>(x + 0.5, y));
		return move(action);
	}
	else if (type == FallType::toLand)
	{
		return make_unique<MoveToAndLandActionFactory>(x + 0.5, y);
	}
	return nullptr;
}

// ------ JUMP ------
vector<SearchCoords*> SearchActions::Jump::GetNextNodes(SearchCoords* startState)
{
	if (!map.NodeIsClimable(*startState) && !map.NodeIsTerrain(startState->x, startState->y + 1)) return vector<SearchCoords*>();
	this->startState = startState;
	vector<SearchCoords*> rightDir = SideControl(1);
	vector<SearchCoords*> leftDir = SideControl(-1);
	rightDir.insert(rightDir.end(), leftDir.begin(), leftDir.end());
	return rightDir;
}
vector<SearchCoords*> SearchActions::Jump::SideControl(int dx)
{
	int x = startState->x;
	int y = startState->y;
	vector<SearchCoords*> coords = JumpControl(dx, x, y); //return coords of jump
	if (x != 0)
	{
		vector<SearchCoords*> fall = Falling(dx, x, y);
		coords.insert(coords.end(), fall.begin(), fall.end());
	}
	return coords;
}
vector<SearchCoords*> SearchActions::Jump::JumpControl(int dx, int& x, int& y)
{
	int prevX = x;
	vector<SearchCoords*> ret;
	SearchCoords* prev = &buffer[x][y];
	if (map.NodeIsTerrain(x, y - 1)) // jump blocked
	{
		x = 0; // no falling - falling by side move
		return ret;
	}
	if (map.NodeIsClimable(x, y - 1) && WriteToState(x, y - 1, prev, make_unique<LongJumpActionFactory>(x, y - 1)))
	{
		vector<SearchCoords*> ret2 = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][y - 1]);
		ret.insert(ret.end(), ret2.begin(), ret2.end());
	}
	x += dx; // prevX = x+dx
	if (map.NodeIsTerrain(x, y)) // block right before agent
	{
		if (!map.NodeIsTerrain(x, y - 1)) // can jump on this block
		{
			if (WriteToState(x, y - 1, prev, make_unique<JumpToActionFactory>(x + 0.5, y-1)))
				ret.push_back(&buffer[x][y - 1]); // new state found
		}
		else if (!map.NodeIsTerrain(x, y - 2) && !map.NodeIsTerrain(prevX, y - 2)) // 2 blocks before agent
		{
			if (WriteToState(x, y - 2, prev, make_unique<JumpToActionFactory>(x + 0.5, y - 2)))
				ret.push_back(&buffer[x][y - 2]); // new state found
		}
		x = 0;	// no falling
		return ret;
	}
	if (map.NodeIsTerrain(x, y - 1))
	{
		if (!map.NodeIsTerrain(prevX, y - 2) && !map.NodeIsTerrain(x, y - 2))
		{			
			// jump on second block up - special higher jump
			unique_ptr<ActionListFactory> actions = make_unique<ActionListFactory>(make_unique<LongJumpActionFactory>(prevX, y));
			actions->AddActionFactory(make_unique<WaitActionFactory>(prevX, y, 10));
			actions->AddActionFactory(make_unique<JumpToActionFactory>(x + 0.5, y - 2));
			if (WriteToState(x, y - 2, prev, move(actions)))
			{
				ret.push_back(&buffer[x][y - 2]);
			}
		}
		x = 0; // no falling - falling by side control
		return ret;
	}

	if (map.NodeIsClimable(x, y - 1) && WriteToState(x, y - 1, prev, make_unique<LongJumpActionFactory>(x, y - 1)))
	{
		// if there is ladder, we can search it now - better than search it again in search algorithm
		vector<SearchCoords*> ret2 = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][y - 1]);
		ret.insert(ret.end(), ret2.begin(), ret2.end());
	}

	if (map.NodeIsTerrain(x, y - 2))
	{
		// jump bocked from top - shorter fall
		y -= 1;
		return ret;
	}
	x += dx; // prevX = x+2*dx
	if (map.NodeIsTerrain(x, y - 1))
	{
		if (!map.NodeIsTerrain(x, y - 2))
		{
			unique_ptr<ActionHandlerFactory> action = GetAction(x, y - 2, prevX, FallType::blocked);
			if (WriteToState(x, y - 2, prev, move(action)))
			{
				ret.push_back(&buffer[x][y - 2]);
			}
		}
		x = 0;
		y -= 1;
		return ret;
	}

	if (map.NodeIsClimable(x, y - 1) && WriteToState(x, y - 1, prev, make_unique<LongJumpActionFactory>(x, y - 1)))
	{
		vector<SearchCoords*> ret2 = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][y - 1]);
		ret.insert(ret.end(), ret2.begin(), ret2.end());
	}
	// else fall from typical spot x+=2*dx
	if (map.NodeIsClimable(x, y) && !map.NodeIsTerrain(x, y)) x -= dx;
	y -= 1;
	return ret;
}
vector<SearchCoords*> SearchActions::Jump::Falling(int dx, int& x, int& y)
{
	return move(SearchActions::SideMove(map, buffer).Falling(dx, x, y, startState, GetAction));
}
bool SearchActions::Jump::WriteToState(int x, int y, SearchCoords* prevCoord, unique_ptr<ActionHandlerFactory> action)
{
	int cost = GetCost(x, y, prevCoord);
	cost += 4;
	SearchCoords* state = &buffer[x][y];
	if (ShouldWriteIntoState(prevCoord, state, cost, 0, 0))
	{
		state->SetCoords(x, y);
		state->Visit(startState, prevCoord->currentDistance + cost, move(action), prevCoord->spelunkerState);
		return true;
	}
	return false;
}
unique_ptr<ActionHandlerFactory> SearchActions::Jump::GetAction(int x, int y, int prevX, FallType type)
{
	if (type == FallType::normal)
		return make_unique<JumpToActionFactory>(x + 0.5, y);
	else if (type == FallType::toLand)
	{
		//return make_unique<JumpToActionFactory>(x + 0.5, y);
		unique_ptr<ActionListFactory> action = make_unique<ActionListFactory>(make_unique<JumpToActionFactory>(x + 0.5, y));
		action->AddActionFactory(make_unique<WaitForLandingActionFactory>(x + 0.5, y));
		return move(action);
	}
	else if (type == FallType::withWaiting)
	{
		int dx = 1;
		if (prevX < x) dx = -1;
		unique_ptr<ActionListFactory> action = make_unique<ActionListFactory>(make_unique<JumpToActionFactory>(x + dx + 0.5, y));
		action->AddActionFactory(make_unique<WaitActionFactory>(x + dx + 0.5, y, waitTime));
		action->AddActionFactory(make_unique<MoveToActionFactory>(x + 0.5, y));
		return move(action);
	}
	else if (type == FallType::blocked)
	{
		double side = -0.2; // add move to side in current chunk to maximaze the jump 
		if (prevX < x) { side = 1.2; }
		unique_ptr<ActionListFactory> action = make_unique<ActionListFactory>(make_unique<MoveToActionFactory>(prevX + side, y));
		action->AddActionFactory(make_unique<JumpToActionFactory>(x + 0.5, y));
		return move(action);
	}
	return nullptr;
}



// ------ CLIMB LADDER ------
vector<SearchCoords*> SearchActions::ClimbLadder::GetNextNodes(SearchCoords* startState)
{
	vector<SearchCoords*> ret;
	if (!map.NodeIsClimable(*startState)) return ret;
	this->startState = startState;
	int x = startState->x;
	int y = startState->y;

	while (map.NodeIsClimable(x, --y))
	{
		if (WriteToState(x, y))
			ret.push_back(&buffer[x][y]);
	}
	y = startState->y;
	if (!map.NodeIsTerrain(x, y + 1))
	{
		unique_ptr<ActionListFactory> action = make_unique<ActionListFactory>(move(startState->action));
		action->AddActionFactory(make_unique<GetOnClimbingActionFactory>(x+0.5, y+0.5));
		startState->action = move(action);
		startState->notToSide = true;

		while (map.NodeIsClimable(x, ++y))
		{
			if (WriteToState(x, y))
				ret.push_back(&buffer[x][y]);
		}
	}
	return ret;
}
bool SearchActions::ClimbLadder::WriteToState(int x, int y)
{
	int cost = abs(startState->y - y) * 2;
	SearchCoords* state = &buffer[x][y];
	if (ShouldWriteIntoState(startState, state, cost,0, 0))
	{
		bool last = map.NodeIsTerrain(x, y + 1);
		state->SetCoords(x, y);
		state->Visit(startState, startState->currentDistance + cost, GetAction(x,y,last), startState->spelunkerState);
		if (!last) state->notToSide = true;
		return true;
	}
	return false;
}
unique_ptr<ActionHandlerFactory> SearchActions::ClimbLadder::GetAction(int x, int y, bool last)
{
	if (!last) return make_unique<ClimbToActionFactory>(x, y+0.5);
	unique_ptr<ActionListFactory> actions = make_unique<ActionListFactory>(make_unique<ClimbToActionFactory>(x, y+0.5));
	actions->AddActionFactory(make_unique<LeaveClimbingActionFactory>(x, y));
	return move(actions);
}
