#include "stdafx.h"
#include "SearchActions.h"

bool SearchActions::ShouldWriteIntoState(const SearchCoords* const current, const SearchCoords* const target, int cost, int lifeLost, int ropeUsed)
{
	if (current->spelunkerState.lifeCount - lifeLost <= 0) return false;
	if (target->completePrice == 0) return true;

	if (target->spelunkerState.lifeCount != current->spelunkerState.lifeCount - lifeLost) // quelity is decided by life count
		return (target->spelunkerState.lifeCount < current->spelunkerState.lifeCount - lifeLost);

	if (target->spelunkerState.ropeCount != current->spelunkerState.ropeCount - ropeUsed) // quality is decided by rope count
		return (target->spelunkerState.ropeCount < current->spelunkerState.ropeCount - ropeUsed);
	
	// life count and rope count same -> quality is decided by distance
	if (target->currentDistance > current->currentDistance + cost) return true;
	return false;
}

int SearchActions::GetCost(int x, int y, const SearchCoords* const prevCoord, MoveType type)
{
	int dx = abs(prevCoord->x - x);
	int dy = abs(y - prevCoord->y);
	if (dx != 0 && dy != 0) --dy;
	switch (type)
	{
	case MoveType::sideMove: return dx;
	case MoveType::jump: return 2 * dx + dy + 4;
	case MoveType::jumpFromLadder:  return 3 * dx + dy + 4;
	case MoveType::withoutSpeed: return dx + dy;
	default:return 500;
	}
}


// ----- FALL ------
vector<SearchCoords*> SearchActions::Fall::Falling(int dx, int& x, int& y, SearchCoords* startState, SearchCoords* prevCoord)
{
	this->startState = startState;
	vector<SearchCoords*> retCoords;
	int botY;
	firstY = y;
	int column = -1;
	bool columnBlocked = false;
	int size = yCoordsDiffs.size() - 1;
	while (true)
	{
		botY = y;
		while (!map.NodeIsTerrain(x, botY))
		{
			if (map.NodeIsClimable(x, botY))
			{
				// finish at climeable - could be blocking
				if (WriteToState(x, botY, FallType::normal, prevCoord))
				{
					retCoords.push_back(&buffer[x][botY]);
					vector<SearchCoords*> ret2 = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][botY]);
					retCoords.insert(retCoords.end(), ret2.begin(), ret2.end());
				}
				break;
			}
			else if (map.NodeIsUnknown(x, botY))
			{
				if (WriteToState(x, botY, FallType::toLand, prevCoord))
					retCoords.push_back(&buffer[x][botY]);
				break;
			}
			++botY; // find bot in current falling column
		}
		--botY; // landing chunk coordinate
		if (map.NodeIsTerrain(x, botY + 1) && WriteToState(x, botY, FallType::toLand, prevCoord))
		{
			retCoords.push_back(&buffer[x][botY]);
		}
		x += dx; // move to next column
		if (column < size)
			++column;
		if (column == 0 && yCoordsDiffs[column] == 1)
		{
			// in case of falling to second column - case of side moving
			if (map.NodeIsTerrain(x, y++) || map.NodeIsTerrain(x, y))
			{
				columnBlocked = true;
			}
		}
		else
		{
			y += yCoordsDiffs[column];
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
			if (ContainsValue(hangColumns, column))
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
vector<SearchCoords*> SearchActions::Fall::Falling(int dx, int& x, int& y, SearchCoords* startState, SearchCoords* prevCoords, MoveType type)
{
	vector<int> yCoordsPrev = yCoordsDiffs;
	vector<int> hangPrev = hangColumns;
	GetFallingVectors(type);
	vector<SearchCoords*> ret = Falling(dx, x, y, startState, prevCoords);
	yCoordsDiffs = yCoordsPrev;
	hangColumns = hangPrev;
	return ret;
}
void SearchActions::Fall::GetFallingVectors(MoveType type)
{
	switch (type)
	{
	case MoveType::sideMove:
		yCoordsDiffs = sideMoveColumnsYCoordDiff;
		hangColumns = sideMoveHanging;
		return;
	case MoveType::jump:
		yCoordsDiffs = jumpColumnsYCoordDiff;
		hangColumns = jumpHanging;
		return;
	case MoveType::jumpFromLadder:
		yCoordsDiffs = ladderJumpColumnsYCoordDiff;
		hangColumns = ladderJumpHanging;
		return;
	case MoveType::ladderWithTopBlock:
		yCoordsDiffs = ladderWithTopBlockColumnsYCoordDiff;
		hangColumns = ladderWithTopBlockHang;
		return;
	default: // MoveType::withoutSpeed
		yCoordsDiffs = withoutInitiativeColumnsYCoordDiff;
		hangColumns = withoutInitiativeHanging;
		return;

	}
}
SearchCoords* SearchActions::Fall::HangingCheck(int x, int y, SearchCoords* prevCoord)
{
	if (!map.NodeIsTerrain(x, y - 1))
	{
		if (WriteToState(x, y - 1, FallType::blocked, prevCoord))
			return &buffer[x][y - 1];
	}
	return nullptr;
}
bool SearchActions::Fall::WriteToState(int x, int y, FallType type, SearchCoords* prevCoord)
{
	int cost = GetCost(x, y, prevCoord, moveType);
	if (type == FallType::blocked) cost += 2;
	int lifeLost = 0;
	SearchCoords* state = &buffer[x][y];
	SearchCoords* previous = startState;
	if (map.NodeIsUnknown(x, y)) { previous = prevCoord; lifeLost = 1; }
	else if (y - firstY > 7) lifeLost = 1;

	if (ShouldWriteIntoState(prevCoord, state, cost, lifeLost, 0))
	{
		state->SetCoords(x, y);
		state->Visit(previous, prevCoord->currentDistance + cost, move(ActionGetter(x, y, prevCoord->x, type)), prevCoord->spelunkerState);
		if (lifeLost != 0) state->spelunkerState.lifeCount -= lifeLost; // too hight for surviving
		return true;
	}
	return false;
}
bool SearchActions::Fall::ContainsValue(vector<int> values, int x)
{
	if (values.size() == 0) return false;
	int* current = &values[0];
	if (x == *current) return true;

	int size = values.size() - 1;
	for (; size > 0; --size)
	{
		current++;
		if (x == *current) return true;
	}
	return false;
}

// ------ SIDE MOVE ------
vector<SearchCoords*> SearchActions::SideMove::GetNextNodes(SearchCoords* startState)
{
	if (startState->notToSide) return vector<SearchCoords*>();
	if (map.NodeIsClimable(*startState) && !map.NodeIsTerrain(startState->x, startState->y + 1)) return vector<SearchCoords*>();
	this->startState = startState;
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
	if (!map.NodeIsTerrain(x, y + 1))
	{
		falling = make_unique<Fall>(map, buffer, *GetAction, MoveType::withoutSpeed);
		retCoords = Falling(dx, x, y, startState);
		return retCoords; // only return possible landing spaces - cant compute further
	}
	x += dx; // starting from next chunk
	while (!map.NodeIsTerrain(x, y))
	{
		if (map.NodeIsClimable(x, y))
		{
			if (WriteToState(x, y, &buffer[x - dx][y]))
			{
				retCoords.push_back(&buffer[x][y]);
				help = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][y]);
				retCoords.insert(retCoords.end(), help.begin(), help.end());
			}
			break;
		}
		else if (map.NodeIsUnknown(x, y))
		{
			if (WriteToState(x, y, &buffer[x - dx][y]))
			{
				retCoords.push_back(&buffer[x][y]);
			}
			break;
		}
		else if (!map.NodeIsTerrain(x, y + 1))
		{
			help = Falling(dx, x, y, &buffer[x - dx][y]);
			retCoords.insert(retCoords.end(), help.begin(), help.end());
			if (x == 0) break; // further falling distance not clear -> devide actions
		}
		else // not falling - horizontal move
		{
			if (WriteToState(x, y, &buffer[x - dx][y]))
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
vector<SearchCoords*> SearchActions::SideMove::Falling(int dx, int& x, int& y, SearchCoords* prevCoords)
{
	if (falling == nullptr) falling = make_unique<Fall>(map, buffer, *GetAction, MoveType::sideMove);
	return falling->Falling(dx, x, y, startState, prevCoords);
}
bool SearchActions::SideMove::WriteToState(int x, int y, SearchCoords* prevCoord)
{
	int cost = GetCost(x, y, prevCoord, MoveType::sideMove);
	SearchCoords* state = &buffer[x][y];
	SearchCoords* previous = startState;
	if (map.NodeIsUnknown(x, y)) previous = prevCoord;

	if (ShouldWriteIntoState(prevCoord, state, cost, 0, 0))
	{
		state->SetCoords(x, y);
		state->Visit(previous, prevCoord->currentDistance + cost, move(GetAction(x, y, prevCoord->x, FallType::normal)), prevCoord->spelunkerState);
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
		unique_ptr<ActionListFactory> action = make_unique<ActionListFactory>(make_unique<MoveToActionFactory>(x - 0.5, y));
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
	if (!startState->climbing && !map.NodeIsTerrain(startState->x, startState->y + 1)) return vector<SearchCoords*>();
	if (startState->climbing) type = MoveType::jumpFromLadder;
	else type = MoveType::jump;
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
	vector<SearchCoords*> coords;
	if (type == MoveType::jump)
		coords = JumpControl(dx, x, y); //return coords of jump
	else coords = LadderJumpControl(dx, x, y);
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
	if (map.NodeIsTerrain(x, y - 1) || map.NodeIsUnknown(x, y - 1)) // jump blocked
	{
		x = 0; // no falling - falling by side move
		return ret;
	}
	if (map.NodeIsClimable(x, y - 1) && WriteToState(x, y - 1, prev, make_unique<LongJumpActionFactory>(x, y - 1)))
	{
		ret.push_back(&buffer[x][y - 1]);
		vector<SearchCoords*> ret2 = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][y - 1]);
		ret.insert(ret.end(), ret2.begin(), ret2.end());
	}
	x += dx; // prevX = x+dx
	if (map.NodeIsTerrain(x, y)) // block right before agent
	{
		if (!map.NodeIsTerrain(x, y - 1)) // can jump on this block
		{
			if (WriteToState(x, y - 1, prev, make_unique<JumpToActionFactory>(x + 0.5, y - 1)))
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
	if (map.NodeIsClimable(x, y - 1) && WriteToState(x, y - 1, prev, make_unique<JumpToActionFactory>(x + 0.5, y - 1)))
	{
		// if there is ladder, we can search it now - better than search it again in search algorithm
		ret.push_back(&buffer[x][y - 1]);
		vector<SearchCoords*> ret2 = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][y - 1]);
		ret.insert(ret.end(), ret2.begin(), ret2.end());
	}
	if (map.NodeIsTerrain(x, y - 2) || map.NodeIsUnknown(x, y - 2))
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
	if (map.NodeIsClimable(x, y - 1) && WriteToState(x, y - 1, prev, make_unique<JumpToActionFactory>(x + 0.5, y - 1)))
	{
		ret.push_back(&buffer[x][y - 1]);
		vector<SearchCoords*> ret2 = ClimbLadder(map, buffer).GetNextNodes(&buffer[x][y - 1]);
		ret.insert(ret.end(), ret2.begin(), ret2.end());
	}
	y -= 1;
	if (map.NodeIsUnknown(x, y)) // unknown node - cant fall or write
	{
		x = 0;
	}

	return ret;
}
vector<SearchCoords*> SearchActions::Jump::LadderJumpControl(int dx, int& x, int& y)
{
	int prevX = x;
	vector<SearchCoords*> ret;
	SearchCoords* prev = &buffer[x][y];
	if (!map.NodeIsTerrain(x, y - 1) && !map.NodeIsTerrain(x + dx, y - 1))
	{
		x += dx; // start falling
		y -= 1;
		if (map.NodeIsTerrain(x, y - 1))
		{
			// jump blocked from top, a bit different way of falling
			ret = Falling(dx, x, y, MoveType::ladderWithTopBlock);
			x = 0; // no other falling
		}
		return ret;
	}
	else if (!map.NodeIsTerrain(x + dx, y))
	{
		x += dx;
		if (WriteToState(x, y, prev, GetAction(x, y, prevX, FallType::normal)))
		{
			ret.push_back(&buffer[x][y]);
		}
		x = 0; // no falling - will fall without initiative speed
		return ret;
	}
	x = 0; // no falling
	return ret;
}
vector<SearchCoords*> SearchActions::Jump::Falling(int dx, int& x, int& y)
{
	if (falling == nullptr) falling = make_unique<Fall>(map, buffer, *GetAction, type);
	return falling->Falling(dx, x, y, startState, startState);
}
vector<SearchCoords*> SearchActions::Jump::Falling(int dx, int& x, int& y, MoveType newType)
{
	if (falling == nullptr) falling = make_unique<Fall>(map, buffer, *GetAction, type);
	return falling->Falling(dx, x, y, startState, startState, newType);
}
bool SearchActions::Jump::WriteToState(int x, int y, SearchCoords* prevCoord, unique_ptr<ActionHandlerFactory> action)
{
	int cost = GetCost(x, y, prevCoord, type);
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
	{
		return make_unique<JumpToActionFactory>(x + 0.5, y);
	}
	else if (type == FallType::toLand)
	{
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
		if (WriteToState(x, y, startState))
			ret.push_back(&buffer[x][y]);
	}
	y = startState->y;
	if (!map.NodeIsTerrain(x, y + 1))
	{
		unique_ptr<ActionListFactory> action = make_unique<ActionListFactory>(move(startState->action));
		action->AddActionFactory(make_unique<GetOnClimbingActionFactory>(x + 0.5, y + 0.5));
		startState->action = move(action);
		startState->notToSide = true;
		startState->climbing = true;

		while (map.NodeIsClimable(x, ++y))
		{
			if (WriteToState(x, y, startState))
			{
				ret.push_back(&buffer[x][y]);
			}
		}
	}
	return ret;
}
bool SearchActions::ClimbLadder::WriteToState(int x, int y, SearchCoords* prevCoords)
{
	int cost = abs(startState->y - y) * 2;
	SearchCoords* state = &buffer[x][y];
	SearchCoords* previous = startState;
	if (map.NodeIsUnknown(x, y)) previous = prevCoords;
	if (ShouldWriteIntoState(startState, state, cost, 0, 0))
	{
		bool last = map.NodeIsTerrain(x, y + 1);
		state->SetCoords(x, y);
		state->Visit(prevCoords, startState->currentDistance + cost, GetAction(x, y, last), startState->spelunkerState);
		if (!last)
		{
			state->notToSide = true;
			state->climbing = true;
		}
		return true;
	}
	return false;
}
unique_ptr<ActionHandlerFactory> SearchActions::ClimbLadder::GetAction(int x, int y, bool last)
{
	if (!last) return make_unique<ClimbToActionFactory>(x, y + 0.5);
	unique_ptr<ActionListFactory> actions = make_unique<ActionListFactory>(make_unique<ClimbToActionFactory>(x, y + 0.5));
	actions->AddActionFactory(make_unique<LeaveClimbingActionFactory>(x, y));
	return move(actions);
}



// ------ CLIMB ROPES ------
vector<SearchCoords*> SearchActions::ClimbRope::GetNextNodes(SearchCoords* startState)
{
	this->startState = startState;
	int x = startState->x;
	int y = startState->y;
	vector<SearchCoords*> ret = GetColumn(x, y - 1, -1, LeaveDirection::stay);
	vector<SearchCoords*> help = GetColumn(x - 1, y, 1, LeaveDirection::left);
	ret.insert(ret.end(), help.begin(), help.end());
	help = GetColumn(x + 1, y, 1, LeaveDirection::right);
	ret.insert(ret.end(), help.begin(), help.end());
	return move(ret);
}
vector<SearchCoords*> SearchActions::ClimbRope::GetColumn(int initialX, int initialY, int dy, LeaveDirection dir)
{
	vector<SearchCoords*> ret;
	if (!ColumnWorthToUse(initialX, initialY, dy)) return ret;
	if (!WriteToInitialPossition(initialX, initialY, dir)) return ret;
	int length = 1;
	if (dir == LeaveDirection::stay) length = 2; // starting from upper chunk -> actual chunk is covered too
	int currentY = initialY + dy;
	SearchCoords* initialState = &buffer[initialX][initialY];
	while(length<8 && map.NodeIsEmpty(initialX,currentY))
	{
		if (WriteToState(initialX, currentY, initialState))
		{
			ret.push_back(&buffer[initialX][currentY]);
			currentY += dy;
			++length;
		}
		else
			break;		
	}
	return ret;
}
bool SearchActions::ClimbRope::ColumnWorthToUse(int x, int y, int dy)
{
	int limit = 3;
	if (dy > 0) limit = 9;
	while (map.NodeIsEmpty(x, y) && limit > 0)
	{
		--limit;
		y += dy;
	}
	return limit == 0; // height limit exceeded? -> worth to calculate column
}
bool SearchActions::ClimbRope::WriteToInitialPossition(int x, int y, LeaveDirection dir)
{
	SearchCoords* state = &buffer[x][y];
	if (ShouldWriteIntoState(startState, state, 1, 0, 1))
	{
		state->SetCoords(x, y);
		state->Visit(startState, startState->currentDistance + 1, GetInitialAction(x, y, dir), startState->spelunkerState);
		state->spelunkerState.ropeCount--;
		return true;
	}
	return false;
}
bool SearchActions::ClimbRope::WriteToState(int x, int y, SearchCoords* prevCoords)
{
	int cost = abs(startState->y - y) * 2;
	SearchCoords* state = &buffer[x][y];
	if (ShouldWriteIntoState(startState, state, cost, 0, 1))
	{
		bool last = map.NodeIsTerrain(x, y + 1);
		state->SetCoords(x, y);
		state->Visit(prevCoords, startState->currentDistance + cost, GetAction(x, y, last), startState->spelunkerState);
		state->spelunkerState.ropeCount--;
		if (!last)
		{
			state->notToSide = true;
			state->climbing = true;
		}
		return true;
	}
	return false;
}
unique_ptr<ActionHandlerFactory> SearchActions::ClimbRope::GetInitialAction(int x, int y, LeaveDirection dir)
{
	unique_ptr<ActionListFactory> actions = make_unique<ActionListFactory>(make_unique<SetRopeActionFactory>(x, y + 0.5,dir));
	actions->AddActionFactory(make_unique<WaitActionFactory>(x, y, 25)); // must wait for rope to be placed
	if (dir != LeaveDirection::stay) // move to chunk border
	{
		int dx = -0.4;
		if (dir == LeaveDirection::right) dx = 1.4;
		actions->AddActionFactory(make_unique<MoveToActionFactory>(x+dx, y));
	}
	else y -= 1; // stay starts at upper node than current (current doesn´t have to be empty)
	actions->AddActionFactory(make_unique<GetOnClimbingActionFactory>(x, y));
	return move(actions);
}
unique_ptr<ActionHandlerFactory> SearchActions::ClimbRope::GetAction(int x, int y, bool last)
{
	if (!last) return make_unique<ClimbToActionFactory>(x, y + 0.5);
	unique_ptr<ActionListFactory> actions = make_unique<ActionListFactory>(make_unique<ClimbToActionFactory>(x, y + 0.5));
	actions->AddActionFactory(make_unique<LeaveClimbingActionFactory>(x, y));
	return move(actions);
}
