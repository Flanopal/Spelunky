#pragma once

class MapControl;

using namespace std;

#include "IBotAPI.h"
#include "FrameworkLibrary.h"
#include "DataStructures.h"

class MapControl
{
public:
	MapControl(IBotAPI* bot) :bot(bot), exitPos(-1,-1),exitFound(false)
	{}
	bool NodeIsTerrain(double x, double y);
	bool NodeIsTerrain(Coordinates coords);
	bool NodeIsClimable(double x, double y);
	bool NodeIsClimable(Coordinates coords);
	bool ExitIsFound();
	void FindExit();
	Coordinates GetExitPos()
	{
		return exitPos;
	}
private:
	bool exitFound;
	Coordinates exitPos;
	IBotAPI* bot;
};