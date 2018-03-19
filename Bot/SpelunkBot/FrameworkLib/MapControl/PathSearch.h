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
	PathSearch(MapControl &map);
	void FindPath(Coordinates start, Coordinates finish);
	unique_ptr<ActionHandlerFactory> GetNextMilestone();
private:
	unique_ptr<ISearch> searcher;
	MapControl &map;
	vector<unique_ptr<ActionHandlerFactory>> path;
};