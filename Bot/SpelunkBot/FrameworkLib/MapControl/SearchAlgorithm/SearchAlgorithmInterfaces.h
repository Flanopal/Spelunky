#pragma once

class IHeuristic;
class ISearch;
class MapControl;

#include <vector>

#include "DataStructures.h"


class IHeuristic
{
public:
	IHeuristic(const SearchCoords* finish) :finish(finish) {}
	virtual int GetStatePrice(SearchCoords* state) = 0;
	virtual ~IHeuristic() {}
protected:
	const SearchCoords* finish;
};

class ISearch
{
public:
	ISearch(MapControl& map) :map(map) {}
	int GetFinalLifeCount() { return finalState.lifeCount; }
	int GetFinalRopeCount() { return finalState.ropeCount; }
	int GetFinalBombCount() { return finalState.bombCount; }
	virtual vector<unique_ptr<ActionHandlerFactory>> FindPath(SearchCoords start, Coordinates finish)=0;
	virtual ~ISearch() {}
protected:
	MapControl &map;
	unique_ptr<IHeuristic> heuristic;
	AdditionalInfo finalState;
};

