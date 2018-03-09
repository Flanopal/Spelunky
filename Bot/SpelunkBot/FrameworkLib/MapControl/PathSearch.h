#pragma once

class PathSearch;

#include <vector>
#include <math.h>
#include <functional>
#include <queue>


#include "MapControl.h"
#include "FrameworkLibrary.h"
#include "IBotAPI.h"
#include "SearchAlgorithmInterfaces.h"

using namespace std;

class PathSearch
{
public: 
	PathSearch(MapControl &map) :map(map)
	{
		searcher = make_unique<AStar>(map);
	}
	void FindPath(Coordinates start, Coordinates finish);
	unique_ptr<ActionHandlerFactory> GetNextMilestone();
private:
	unique_ptr<ISearch> searcher;
	MapControl &map;
	vector<unique_ptr<ActionHandlerFactory>> path;
};