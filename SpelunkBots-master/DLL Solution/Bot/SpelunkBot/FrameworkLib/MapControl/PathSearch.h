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
	// methods for path finding
	void FindPath(Coordinates start, Coordinates finish);
	/// Return path to unknown state closest to finish
	void FindPathToUnknown(Coordinates start, Coordinates finish);

	// also return path immediately
	vector<unique_ptr<ActionHandlerFactory>> FindAndGetPath(Coordinates start, Coordinates finish);
	vector<unique_ptr<ActionHandlerFactory>> FindAndGetPathToUnknown(Coordinates start, Coordinates finish);
	
	// seting used tools for searching
	void SetLifesForSearch(int lifes) { lifeCount = lifes; }
	void SetRopesForSearch(int ropes) { ropeCount = ropes; }


	unique_ptr<ActionHandlerFactory> GetNextMilestone();
	/// Return last path saved in this object
	vector<unique_ptr<ActionHandlerFactory>> GetCurrentPath();
private:
	/// Prepare start state into state object used in searching
	SearchCoords GetInitialState(Coordinates start);

	// tools used for searching - if is value < 0 -> bots actual stats will be used
	int lifeCount = -1;
	int ropeCount = -1;

	unique_ptr<ISearch> searcher;
	/// Filter function for unknown states
	bool UnknownStatesFilter(SearchCoords* state);
	IBotAPI* bot;
	MapControl &map;
	vector<unique_ptr<ActionHandlerFactory>> path;
};