#include "stdafx.h"
#include "MapControl.h"


bool MapControl::NodeIsTerrain(int x, int y)
{
	if (!CoordControl(x,y)) return false;
	if (cave[x][y] == spStandardTerrain)
		return true;
	return false;
}
void MapControl::Update()
{
	UpdateCaveMap();
}
bool MapControl::NodeIsTerrain(Coordinates coords)
{
	return NodeIsTerrain(coords.x, coords.y);
}
bool MapControl::NodeIsTerrain(SearchCoords& coords)
{
	return NodeIsTerrain(coords.x, coords.y);
}

bool MapControl::NodeIsClimable(int x, int y)
{
	if (!CoordControl(x, y)) return false;
	if (cave[x][y] == spLadder || cave[x][y] == spRope)
		return true;
	return false;
}
bool MapControl::NodeIsClimable(Coordinates coords)
{
	return NodeIsClimable(coords.x, coords.y);
}
bool MapControl::NodeIsClimable(SearchCoords& coords)
{
	return NodeIsClimable(coords.x, coords.y);
}

bool MapControl::NodeIsUnknown(int x, int y)
{
	if (!CoordControl(x, y)) return false;
	if (cave[x][y] == spUnknownNode) return true;
	return false;
}
bool MapControl::NodeIsUnknown(Coordinates coords)
{
	return NodeIsUnknown(coords.x, coords.y);
}
bool MapControl::NodeIsUnknown(SearchCoords& coords)
{
	if (cave[coords.x][coords.y] == spUnknownNode) return true;
	return false;
}

bool MapControl::NodeIsEmpty(int x, int y)
{
	if (!CoordControl(x, y)) return false;
	if (cave[x][y] == spEmptyNode) return true;
	return false;
}
bool MapControl::NodeIsEmpty(Coordinates coords)
{
	return NodeIsUnknown(coords.x, coords.y);
}
bool MapControl::NodeIsEmpty(SearchCoords& coords)
{
	if (cave[coords.x][coords.y] == spEmptyNode) return true;
	return false;
}

void MapControl::EmplaceRope(LeaveDirection dir)
{
	Coordinates pos = bot->GetPlayerCoordinates();
	int currentX = (int)pos.x;
	int currentY = (int)pos.y;
	int length = 8;
	int dy = 1;
	switch (dir) // get right direction params
	{
	case LeaveDirection::stay: dy = -1; currentY -= 1; --length; break;
	case LeaveDirection::left: currentX -= 1; break;
	case LeaveDirection::right: currentX += 1; break;
	default: return;
	}
	while (NodeIsEmpty(currentX, currentY) && length > 0)
	{
		cave[currentX][currentY] = spRope;
		currentY += dy;
		--length;
	}
	if (dir == LeaveDirection::stay && length != 7 && NodeIsEmpty(currentX, (int)pos.y))
	{
		// rope placed sucessfully and agent stands at empty node -> rope is also here
		cave[currentX][(int)pos.y] = spRope;
	}
}

bool MapControl::ExitIsFound()
{
	return exitFound;
}
vector<Coordinates> MapControl::GetRopesAndCrates()
{
	return ropesAndCrates;
}
void MapControl::RemoveRopeOrChest()
{
	Coordinates pos = bot->GetPlayerCoordinates();
	for (size_t i = 0; i < ropesAndCrates.size(); ++i)
	{
		if (ropesAndCrates[i].EqualCoordinates(pos))
		{
			ropesAndCrates.erase(ropesAndCrates.begin() + i);
			return;
		}
	}
}
void MapControl::SaveMap()
{
	// called at begining - cycle over whole cave
	for (int y = 0; y < Y_NODES; ++y)
	{
		for (int x = 0; x < X_NODES; ++x)
		{
			SavePosition(x, y);
			NodeExitCheck(x, y);
			if (top == -1 && cave[x][y] != spUnknownNode)
			{
				left = x;
				top = y;
				continue;
			}
		}
	}
	// find borders of visible screen
	for (int y = top; y < Y_NODES; ++y)
	{
		if (cave[left][y] == spUnknownNode)
		{
			bottom = y - 1;
			break;
		}
	}
	for (int x = left; x < X_NODES; ++x)
	{
		if (cave[top][x] == spUnknownNode)
		{
			right = x - 1;
			break;
		}
	}
}
Coordinates MapControl::GetExitPos()
{
	return exitPos;
}

void MapControl::SavePosition(int x, int y)
{
	cave[x][y] = bot->GetNodeState(x, y, NODE_COORDS);
	if (bot->GetIDOfCollectableInNode(spCrate, x, y, NODE_COORDS) > 0.5 || bot->GetIDOfCollectableInNode(spRope, x, y, NODE_COORDS) > 0.5)
	{
		ropesAndCrates.push_back(Coordinates(x+0.5, y+0.5));
	}
}
void MapControl::NodeExitCheck(int x, int y)
{
	if (bot->GetNodeState(x, y, NODE_COORDS) == spExit)
	{
		exitFound = true;
		exitPos.x = x;
		exitPos.y = y;
		cout << "FOUND EXIT" << endl;
	}
}

void MapControl::UpdateCaveMap()
{
	double x = bot->GetPlayerPositionXNode();
	double y = bot->GetPlayerPositionYNode();
	Coordinates prevPos = lib->GetPrevPossition();
	if (prevPos.x == 0) return; // initial state was previous

	// bots position difference
	int dx = (int)x - (int)prevPos.x;
	int dy = (int)y - (int)prevPos.y;
	int lineX;
	int lineY;
	if (dx != 0)
	{
		if (dx < 0) lineX = left+dx;
		else lineX = right+dx;
		if (VerticalBorderControl(lineX))
		{
			left += dx;
			right += dx;
		}
		else dx = 0;
	}
	if (dy != 0)
	{
		
		if (dy > 0) lineY = bottom+dy;
		else lineY = top+dy;
		if (HorizontalBorderControl(lineY))
		{
			top += dy;
			bottom += dy;
			HorizontalBorderUpdate(lineY);
		}
	}
	if (dx!=0) VerticalBorderUpdate(lineX);
}
bool MapControl::VerticalBorderControl(int line)
{
	if (line<0 || line>41) return false;
	int index = (top + bottom) / 2;
	if (bot->GetNodeState(line, index, NODE_COORDS) == spUnknownNode) return false;
	return true;
}
bool MapControl::HorizontalBorderControl(int line)
{
	if (line<0 || line>33) return false;
	int index = (left + right) / 2;
	if (bot->GetNodeState(index, line, NODE_COORDS) == spUnknownNode) return false;
	return true;
}
void MapControl::VerticalBorderUpdate(int& line)
{
	for (int i = top; i <= bottom; ++i)
	{
		SavePosition(line, i);
		if (!exitFound)
			NodeExitCheck(line, i);
	}
}

void MapControl::HorizontalBorderUpdate(int& line)
{
	for (int i = left; i <= right; ++i)
	{
		SavePosition(i, line);
		if (!exitFound)
			NodeExitCheck(i, line);
	}
}

bool MapControl::CoordControl(int x, int y)
{
	if (x < 0 || x>41 || y < 0 || y>33)
	{
		cout << "COORDINATES OUT OF RANGE - x:" << x << " y:" << y << endl;
		return false;
	}
	return true;
}