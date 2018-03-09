#include "stdafx.h"
#include "SearchActions.h"

bool SearchActions::ShouldWriteIntoState(const SearchCoords& current, const SearchCoords& target,int lifeLost, int ropeUsed)
{
	if (target.completePrice == 0) return true;
	if (target.spelunkerState.lifeCount < current.spelunkerState.lifeCount-lifeLost) return true;
	if (target.spelunkerState.ropeCount < current.spelunkerState.ropeCount - ropeUsed) return true;
	return false;
}
vector<SearchCoords&> SearchActions::BasicAction::GetNextNodes(MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34])
{
	vector<SearchCoords&> retCoords;
	SearchCoords& coord1 = buffer[coords.x+1][coords.y];
	if (!map.NodeIsTerrain(coord1)) retCoords.push_back(coord1);
	SearchCoords& coord2 = buffer[coords.x-1][coords.y];
	if (!map.NodeIsTerrain(coord2)) retCoords.push_back(coord2);
	SearchCoords& coord3 = buffer[coords.x][coords.y+1];
	if (!map.NodeIsTerrain(coord3)) retCoords.push_back(coord3);
	SearchCoords& coord4 = buffer[coords.x][coords.y-1];
	if (!map.NodeIsTerrain(coord4)) retCoords.push_back(coord4);
	return retCoords;
}

vector<SearchCoords&> SearchActions::SideMove::GetNextNodes(MapControl &map, SearchCoords& startState, SearchCoords(&buffer)[42][34])
{
	vector<SearchCoords&> rightDir = SideControl(1, map, startState, buffer);
	vector<SearchCoords&> leftDir = SideControl(-1, map, startState, buffer);
	rightDir.insert(rightDir.end(), leftDir.begin(), leftDir.end());
	return rightDir;
}
vector<SearchCoords&> SearchActions::SideMove::SideControl(int dx, MapControl &map, SearchCoords& startState, SearchCoords(&buffer)[42][34])
{
	vector<SearchCoords&> retCoords;
	vector<SearchCoords&> help;
	int y = startState.y;
	int x = startState.x;
	if (!map.NodeIsTerrain(buffer[x][y - 1]))
	{
		retCoords = Falling(dx, x, y, map, startState, buffer); 
		if (x == 0) return retCoords; // only return possible landing spaces - cant compute further
	}
	x += dx; // starting from next chunk
	while (!map.NodeIsTerrain(buffer[x][y]))
	{		
		if (!map.NodeIsTerrain(buffer[x][y - 1]))
		{
			help = Falling(dx, x, y, map, startState, buffer);
			retCoords.insert(retCoords.end(), help.begin(), help.end());
			if (x == 0) break; // further falling distance not clear -> devide actions			
		}
		else // not falling - horizontal move
		{
			if (WriteToState(x, y, startState, buffer[x - dx][y], buffer))
			{
				retCoords.push_back(buffer[x][y]);
			}
			else break; // already visited state
		}
		x += dx;		
	}
	return retCoords;
}
vector<SearchCoords&> SearchActions::SideMove::Falling(int dx, int& x, int& y, MapControl &map, SearchCoords& startState, SearchCoords(&buffer)[42][34])
{
	vector<SearchCoords&> retCoords;
	SearchCoords& prevCoord = buffer[x][y];
	int botY;
	int firstY = y;
	bool columnBlocked;
	while (true)
	{
		columnBlocked = false;
		botY = y;
		while (!map.NodeIsTerrain(x, botY)) ++botY; // find bot in current falling column
		--botY; // landing chunk coordinate
		if (WriteToState(x, botY, startState, prevCoord, buffer))
		{
			retCoords.push_back(buffer[x][botY]);
		}
		x += dx; // move to next column
		if (y == startState.y && startState.x - x != dx)
		{
			// coords.x-x - check, bot wasnt already falling - this case initial speed is missing
			// next checks will fail at firt condition
			if (map.NodeIsTerrain(x, y++) || map.NodeIsTerrain(x, y))
			{
				// next column blocked, for second column special case - 2 blocking blocks
				columnBlocked = true;
			}
		}
		else
		{
			if (y==firstY+1) y += 1; // third column
			else if (y == firstY + 2) y += 3; // fourth column
			else y += 4; // every 4 block of height is falled distance longer - other columns
			if (y > botY) // entring block for next collumn is unreachable
			{
				x -= dx; // previsou block had further reachable possition
				y = botY;
				break;
			}
			else if (map.NodeIsTerrain(x, y)) columnBlocked = true;
		}
		if (columnBlocked)
		{
			while (map.NodeIsTerrain(x, ++y) && y <= botY); // scrolling down the blocking column of terrain
			if (y > botY) { y = botY; x -= dx; break; } // no reachable block -> previsou column was last one
			while (!map.NodeIsTerrain(x, y + 1)) ++y; // scrolling empty blocks -> falling in this column
			if (WriteToState(x, y, startState, prevCoord, buffer)) // write into bottom of column
			{
				retCoords.push_back(buffer[x][y]);
			}
			break; // cant fall further
		}
	}
	return retCoords;
}

bool SearchActions::SideMove::WriteToState(int x, int y, SearchCoords& startState, SearchCoords& prevCoord, SearchCoords(&buffer)[42][34])
{
	int lifeLost;
	SearchCoords& state = buffer[x][y];
	if (y - prevCoord.y > 7) lifeLost = 1;
	else lifeLost = 0;
	if (ShouldWriteIntoState(prevCoord, state, lifeLost, 0))
	{
		state.SetCoords(x, y);
		state.Visit(startState, prevCoord.currentDistance + 1, make_unique<MoveToActionFactory>(x), prevCoord.spelunkerState);
		if (lifeLost != 0) state.spelunkerState.lifeCount -= lifeLost; // too hight for surviving
		return true;
	}
	return false;
}