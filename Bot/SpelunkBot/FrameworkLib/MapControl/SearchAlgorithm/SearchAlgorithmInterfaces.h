#pragma once

#include <vector>

class IHeuristic;
class ISearch;

#include "MapControl.h"
#include "AStarSearch.h"
#include "Heuristic.h"
#include "DataStructures.h"
#include "SearchActions.h"
#include "ActionFactories.h"

struct AdditionalInfo;

class IHeuristic
{
public:
	IHeuristic(SearchCoords& const finish) :finish(finish) {}
	virtual int GetStatePrice(SearchCoords& state) = 0;
	virtual ~IHeuristic() {}
protected:
	SearchCoords& const finish;
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
	vector<ISearchAction> actions;
	AdditionalInfo finalState;
};


struct AdditionalInfo
{
	int lifeCount = 1;
	int ropeCount = 0;
	int bombCount = 0;
};

struct SearchCoords
{
	int x=0;
	int y=0;

	SearchCoords* previousState = nullptr;
	unique_ptr<ActionHandlerFactory> action;

	int completePrice=0;
	int currentDistance = INT32_MAX;

	AdditionalInfo spelunkerState;

	void SetCoords(int x, int y) { this->x = x; this->y = y; }

	void Visit(SearchCoords& prevState, int actionCount, unique_ptr<ActionHandlerFactory> action, AdditionalInfo state)
	{
		if (actionCount >= currentDistance) return;
		previousState = &prevState;
		currentDistance = actionCount;
		action = move(action);
		spelunkerState = state;
	}

	bool IsEqualCoords(const SearchCoords& a) const
	{
		if (a.x == x && a.y == y) return true;
		return false;
	}

	bool operator<(const SearchCoords& a) const
	{
		if (a.completePrice > completePrice) return true;
		return false;
	}
	bool operator>(const SearchCoords& a) const
	{
		if (a.completePrice < completePrice) return true;
		return false;
	}
	bool operator==(const SearchCoords& a) const
	{
		if (a.completePrice == completePrice) return true;
		return false;
	}
};
