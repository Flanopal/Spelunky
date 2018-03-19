#pragma once

class IHeuristic;
class ISearch;

#include <vector>

#include "DataStructures.h"

class MapControl;

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
	void SetRopesToUse(int count) { ropeCount = count; }
	void SetLivesToUse(int count) { lifeCount = count; if (lifeCount < 1) lifeCount = 1; }
	void SetBombsToUse(int count) { bombCount = count; }
	int GetFinalLifeCount() { return finalState.lifeCount; }
	int GetFinalRopeCount() { return finalState.ropeCount; }
	int GetFinalBombCount() { return finalState.bombCount; }
	virtual vector<unique_ptr<ActionHandlerFactory>> FindPath(Coordinates start, Coordinates finish)=0;
	virtual ~ISearch() {}
protected:
	int ropeCount = 0;
	int lifeCount = 1;
	int bombCount = 0;
	MapControl &map;
	unique_ptr<IHeuristic> heuristic;
	AdditionalInfo finalState;
};

