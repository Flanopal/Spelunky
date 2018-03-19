#include "stdafx.h"
#include "PathSearch.h"

PathSearch::PathSearch(MapControl &map) :map(map)
{
	searcher = make_unique<AStar>(map);
}

void PathSearch::FindPath(Coordinates start, Coordinates finish)
{
	path = searcher->FindPath(start, finish);
}

unique_ptr<ActionHandlerFactory> PathSearch::GetNextMilestone()
{
	if (path.size() == 0) return nullptr;
	unique_ptr<ActionHandlerFactory> ret = move(path.back());
	path.pop_back();
	return move(ret);
}