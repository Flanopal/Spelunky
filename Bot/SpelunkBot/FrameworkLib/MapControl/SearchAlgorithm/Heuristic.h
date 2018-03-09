#pragma once

#include "SearchAlgorithmInterfaces.h"

class DistanceHeuristic : public IHeuristic
{
public:
	DistanceHeuristic(SearchCoords& const finish) :IHeuristic(finish) {}
	virtual int GetStatePrice(SearchCoords& state);
	virtual ~DistanceHeuristic() {}
};