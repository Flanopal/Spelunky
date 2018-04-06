#include "stdafx.h"
#include "MapControl.h"


bool MapControl::NodeIsTerrain(int x, int y)
{
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
	if (cave[x][y] == spLadder)
		return true;
	// rope
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

bool MapControl::NodeIsUnknown(SearchCoords& coords)
{
	if (cave[coords.x][coords.y] == spUnknownNode) return true;
	return false;
}

bool MapControl::ExitIsFound()
{
	return exitFound;
}

void MapControl::SaveMap()
{
	for (int y = 0; y < Y_NODES; ++y)
	{
		for (int x = 0; x < X_NODES; ++x)
		{
			cave[x][y] = bot->GetNodeState(x, y, NODE_COORDS);
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

void MapControl::NodeExitCheck(int x, int y)
{
	if (bot->GetNodeState(x, y, NODE_COORDS) == spExit)
	{
		exitFound = true;
		exitPos.x = x;
		exitPos.y = y;
		std::cout << "FOUND EXIT" << std::endl;
	}
}

void MapControl::UpdateCaveMap()
{
	double x = bot->GetPlayerPositionXNode();
	double y = bot->GetPlayerPositionYNode();
	Coordinates prevPos = lib->GetPrevPossition();
	if (prevPos.x == 0) return; // initial state was previous
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
		cave[line][i] = bot->GetNodeState(line, i, NODE_COORDS);
		if (!exitFound)
			NodeExitCheck(line, i);
	}
}

void MapControl::HorizontalBorderUpdate(int& line)
{
	for (int i = left; i <= right; ++i)
	{
		cave[i][line] = bot->GetNodeState(i, line, NODE_COORDS);
		if (!exitFound)
			NodeExitCheck(i, line);
	}
}