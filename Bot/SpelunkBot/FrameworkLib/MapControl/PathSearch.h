#pragma once

class PathSearch;

#include <vector>
#include <memory>
#include <math.h>
#include <functional>
#include <queue>

#include "SearchAlgorithmInterfaces.h"

#include "ActionFactories.h"
#include "MapControl.h"

#include "AStarSearch.h"
#include "Heuristic.h"

using namespace std;

class PathSearch
{
public: 
	PathSearch(MapControl &map, IBotAPI* bot);
	void FindPath(Coordinates start, Coordinates finish);
	void FindPathToUnknown(Coordinates start, Coordinates finish);
	vector<unique_ptr<ActionHandlerFactory>> FindAndGetPath(Coordinates start, Coordinates finish);
	vector<unique_ptr<ActionHandlerFactory>> FindAndGetPathToUnknown(Coordinates start, Coordinates finish);
	void SetLifesForSearch(int lifes) { lifeCount = lifes; }
	void SetRopesForSearch(int ropes) { ropeCount = ropes; }
	unique_ptr<ActionHandlerFactory> GetNextMilestone();
	vector<unique_ptr<ActionHandlerFactory>> GetCurrentPath();
private:
	SearchCoords GetInitialState(Coordinates start);
	int lifeCount = -1;
	int ropeCount = -1;
	unique_ptr<ISearch> searcher;

	bool UnknownStatesFilter(SearchCoords* state);
	IBotAPI* bot;
	MapControl &map;
	vector<unique_ptr<ActionHandlerFactory>> path;
};