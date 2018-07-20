#pragma once

class AStar;

#include <string>
#include <queue>

#include "PathSearch.h"
#include "SearchActions.h"


class AStar :public ISearch
{
public:
	AStar(MapControl& map) :ISearch(map){}
	virtual vector<unique_ptr<ActionHandlerFactory>> FindPath(SearchCoords start, Coordinates finish);
	virtual ~AStar() {}
private:
	/// Get states reachable from param state by aplying predefined movements, also fill them into buffer
	vector<SearchCoords*> GetNextStates(SearchCoords* state, SearchCoords(&buffer)[42][34]);

	/// Add evaluation from heuristic for all passes states
	void EvaluateStatesToTarget(vector<SearchCoords*> states); 

	/// Decide which state is closer to current target
	SearchCoords* GetBetterState(SearchCoords* currentBest, SearchCoords* newState);

	/// Complete final path after searching from data in buffer
	vector<unique_ptr<ActionHandlerFactory>> CompletePath(SearchCoords(&buffer)[42][34], SearchCoords* target);

	/// Wrote buffer state into console - used for debuging
	void ShowBuffer(SearchCoords(&buffer)[42][34]);
};