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
	vector<SearchCoords*> GetNextStates(SearchCoords* state, SearchCoords(&buffer)[42][34]);
	void EvaluateStatesToTarget(vector<SearchCoords*> states); //add evaluation of heuristic
	SearchCoords* GetBetterState(SearchCoords* currentBest, SearchCoords* newState);
	vector<unique_ptr<ActionHandlerFactory>> CompletePath(SearchCoords(&buffer)[42][34], SearchCoords* target);

	void ShowBuffer(SearchCoords(&buffer)[42][34]);
	AdditionalInfo finalStats;
};