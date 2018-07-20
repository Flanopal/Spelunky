#pragma once

class IHeuristic;
class ISearch;
class MapControl;

#include <vector>

#include "DataStructures.h"

typedef unique_ptr<function<bool(SearchCoords*)>> FilterFunction;

class IHeuristic
{
public:
	IHeuristic(const SearchCoords* finish) :finish(finish) {}

	///return price of passed state
	virtual int GetStatePrice(SearchCoords* state) = 0;
	virtual ~IHeuristic() {}
protected:
	const SearchCoords* finish; // finish state - for object is still same
};

class ISearch
{
public:
	ISearch(MapControl& map) :map(map) {}
	virtual vector<unique_ptr<ActionHandlerFactory>> FindPath(SearchCoords start, Coordinates finish)=0;
	/// Find path using filter function for acceptable states
	virtual vector<unique_ptr<ActionHandlerFactory>> FindPath(SearchCoords start, Coordinates finish, FilterFunction StateFilter)
	{
		this->StateFilter = move(StateFilter);
		auto ret = move(FindPath(move(start), finish));
		this->StateFilter = NULL;
		return move(ret);
	}
	virtual ~ISearch() {}
protected:

	FilterFunction StateFilter;
	MapControl &map;
	unique_ptr<IHeuristic> heuristic;
};

