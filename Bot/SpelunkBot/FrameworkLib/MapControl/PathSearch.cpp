#include "stdafx.h"
#include "PathSearch.h"

PathSearch::PathSearch(MapControl &map, IBotAPI* bot) :map(map),bot(bot)
{
	searcher = make_unique<AStar>(map);
}

void PathSearch::FindPath(Coordinates start, Coordinates finish)
{
	SearchCoords initial = GetInitialState(start);
	path = searcher->FindPath(move(initial), finish);
}

void PathSearch::FindPathToUnknown(Coordinates start, Coordinates finish)
{
	SearchCoords initial = GetInitialState(start);
	path = searcher->FindPath(move(initial), finish, make_unique<function<bool(SearchCoords*)>>(bind(&PathSearch::UnknownStatesFilter, this, placeholders::_1)));
}
vector<unique_ptr<ActionHandlerFactory>> PathSearch::FindAndGetPath(Coordinates start, Coordinates finish)
{
	SearchCoords initial = GetInitialState(start);
	return searcher->FindPath(move(initial), finish);
}
vector<unique_ptr<ActionHandlerFactory>> PathSearch::FindAndGetPathToUnknown(Coordinates start, Coordinates finish)
{
	SearchCoords initial = GetInitialState(start);
	return searcher->FindPath(move(initial), finish, make_unique<function<bool(SearchCoords*)>>(bind(&PathSearch::UnknownStatesFilter, this, placeholders::_1)));
}

unique_ptr<ActionHandlerFactory> PathSearch::GetNextMilestone()
{
	if (path.size() == 0) return nullptr;
	unique_ptr<ActionHandlerFactory> ret = move(path.back());
	path.pop_back();
	return move(ret);
}

vector<unique_ptr<ActionHandlerFactory>> PathSearch::GetCurrentPath()
{
	vector<unique_ptr<ActionHandlerFactory>> ret = move(path);
	vector<unique_ptr<ActionHandlerFactory>> newVector;
	path = move(newVector);
	return move(ret);
}

SearchCoords PathSearch::GetInitialState(Coordinates start)
{
	SearchCoords initial;
	initial.SetCoords(start.x, start.y);
	if (lifeCount<1) initial.spelunkerState.lifeCount = bot->GetLifeCount();
	else initial.spelunkerState.lifeCount = lifeCount;
	if (ropeCount < 1) initial.spelunkerState.ropeCount = bot->GetRopeCount();
	else  initial.spelunkerState.ropeCount = ropeCount;
	initial.notToSide = bot->IsClimbing();
	lifeCount = -1;
	ropeCount = -1;
	return move(initial);
}

bool PathSearch::UnknownStatesFilter(SearchCoords* state)
{
	return map.NodeIsUnknown(*state);
}