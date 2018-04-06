#pragma once
#include <vector>
#include <memory>
#include <iostream>

class FrameworkLibrary;
class MovingController;

#include "IBotAPI.h"
#include "DataStructures.h"
#include "EnemyControl.h"
#include "MapControl.h"
#include "PlayerActions.h"
#include "PathSearch.h"

using namespace std;

class FrameworkLibrary
{
public:
	FrameworkLibrary(IBotAPI* bot);
	bool Update();
	//unique_ptr<EnemyControl> enemyControl;
	unique_ptr<MapControl> mapControl;
	unique_ptr<PathSearch> pathSearch;
	unique_ptr<PlayerActions> playerActions;
	Coordinates GetPrevPossition()
	{
		return previousPossition;
	}
private:
	void UpdatePrevisousPossition();
	Coordinates previousPossition;
	IBotAPI* bot;
};

class MovingController
{
public:
	MovingController(FrameworkLibrary* lib, IBotAPI* bot, int limit) :lib(lib), bot(bot), samePlaceTimeLimit(limit) {}
	bool Update()
	{
		Coordinates curPos = bot->GetPlayerCoordinates();
		if (curPos == lib->GetPrevPossition())
		{
			++samePlaceTime;
			if (samePlaceTime == samePlaceTimeLimit)
				return false;
		}
		else
		{
			samePlaceTime = 0;
		}
		return true;
	}
private:
	int samePlaceTime = 0;
	int samePlaceTimeLimit = 5;
	FrameworkLibrary* lib;
	IBotAPI* bot;
};