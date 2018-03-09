#pragma once

class AStar;

#include "SearchAlgorithmInterfaces.h"
#include "Heuristic.h"



class AStar :public ISearch
{
public:
	AStar(MapControl& map) :ISearch(map){}
	virtual vector<unique_ptr<ActionHandlerFactory>> FindPath(Coordinates start, Coordinates finish);
	virtual ~AStar() {}
private:
	vector<SearchCoords&> GetNextStates(SearchCoords& state, SearchCoords(&buffer)[42][34]);
	void EvaluateStatesToTarget(vector<SearchCoords&> states); //add evaluation of heuristic
	vector<unique_ptr<ActionHandlerFactory>> CompletePath(SearchCoords(&buffer)[42][34], SearchCoords* target);
	AdditionalInfo finalStats;
};