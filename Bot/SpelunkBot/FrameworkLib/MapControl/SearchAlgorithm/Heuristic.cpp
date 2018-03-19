#include "stdafx.h"
#include "Heuristic.h"

int DistanceHeuristic::GetStatePrice(SearchCoords* state)
{
	int val = abs(finish->x - state->x);
	val += abs(finish->y - state->y);
	return val;
}