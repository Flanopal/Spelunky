#include "stdafx.h"
#include "MapControl.h"


bool MapControl::NodeIsTerrain(double x, double y)
{
	if (bot->GetNodeState(x, y, NODE_COORDS) == spStandardTerrain)
		return true;
	return false;
}
bool MapControl::NodeIsTerrain(Coordinates coords)
{
	return NodeIsTerrain(coords.x, coords.y);
}

bool MapControl::NodeIsClimable(double x, double y)
{
	if (bot->GetNodeState(x, y, NODE_COORDS) == spLadder)
		return true;
	// rope
	return false;
}
bool MapControl::NodeIsClimable(Coordinates coords)
{
	return NodeIsClimable(coords.x, coords.y);
}

bool MapControl::ExitIsFound()
{
	return exitFound;
}

void MapControl::FindExit()
{
	for (int y = 0; y < Y_NODES; y++)
	{
		for (int x = 0; x < X_NODES; x++)
		{
			if (bot->GetNodeState(x, y, NODE_COORDS) == spExit)
			{
				exitFound = true;
				exitPos.x = x;
				exitPos.y = y;
				std::cout << "FOUND EXIT" << std::endl;
			}
		}
	}
}
