#include "stdafx.h"
#include "AStarSearch.h"

vector<unique_ptr<ActionHandlerFactory>> AStar::FindPath(SearchCoords start, Coordinates finish)
{
	// initialization
	SearchCoords buffer[42][34]; //map of reachable states and actions leading to them with path length
	SearchCoords* target = &buffer[(int)finish.x] [(int)finish.y];
	target->SetCoords((int)finish.x, (int)finish.y);
	SearchCoords* myPos = &buffer[(int)start.x] [(int)start.y]; // initialize start position
	*myPos = move(start);
	if (myPos->IsEqualCoords(target)) // already in finish
	{
		target->action = make_unique<MoveToActionFactory>(target->x + 0.5, target->y);
		return CompletePath(buffer, target);
	}

	heuristic = make_unique<DistanceHeuristic>(target); // heuristic function for h* evaluating
	priority_queue<SearchCoords*, std::vector<SearchCoords*>, std::greater<SearchCoords*>> frontier; // holds Search possitions ordered by price

	int targetDistance = heuristic->GetStatePrice(myPos);
	myPos->completePrice = targetDistance;
	myPos->currentDistance = 0;

	// searching

	SearchCoords* bestEvaluated = myPos; // place closest to target by h*
	SearchCoords* actual; // actual state to expand
	vector<SearchCoords*> nextStates;
	if (myPos->notToSide) // starting as climbing case
	{
		nextStates = SearchActions::ClimbLadder(map, buffer).GetNextNodes(myPos);
		EvaluateStatesToTarget(nextStates);
		size_t size = nextStates.size();
		for (size_t i = 0; i < size; ++i)
		{
			frontier.push(nextStates[i]);
		}
		nextStates.clear();
	}
	else frontier.push(myPos);
	do
	{
		actual = frontier.top();
		frontier.pop();
		bestEvaluated = GetBetterState(bestEvaluated, actual);
		if (actual->IsEqualCoords(target)) break;

		nextStates = GetNextStates(actual, buffer);

		EvaluateStatesToTarget(nextStates);
		size_t size = nextStates.size();
		for (size_t i = 0; i < size; ++i)
		{
			frontier.push(nextStates[i]);
		}
		nextStates.clear();
	} while (frontier.size() > 0);
	//Return closes state
	finalState = actual->spelunkerState;

	ShowBuffer(buffer); // debuging info about buffer structure
	cout << "Best spot x " << bestEvaluated->x << " y " << bestEvaluated->y << endl;
	return CompletePath(buffer, bestEvaluated);
}

vector<SearchCoords*> AStar::GetNextStates(SearchCoords* state, SearchCoords(&buffer)[42][34])
{
	vector<SearchCoords*> ret = SearchActions::SideMove(map,buffer).GetNextNodes(state);
	vector<SearchCoords*> help = SearchActions::Jump(map, buffer).GetNextNodes(state);
	ret.insert(ret.end(), help.begin(), help.end()); // append states obtained by jumping
	if (state->spelunkerState.ropeCount > 0 && map.NodeIsEmpty(*state) && state->climbing==false) // if ropes available, use them
	{
		help = SearchActions::ClimbRope(map, buffer).GetNextNodes(state);
		ret.insert(ret.end(), help.begin(), help.end());
	}
	return ret;
}

void AStar::EvaluateStatesToTarget(vector<SearchCoords*> states)
{
	size_t size = states.size();
	int targetDistance = 0;
	for (size_t i = 0; i < size; ++i)
	{
		SearchCoords* state = states[i];
		state->targetDistance = heuristic->GetStatePrice(states[i]);
		state->completePrice += state->targetDistance;
	}
}

SearchCoords* AStar::GetBetterState(SearchCoords* currentBest, SearchCoords* newState)
{
	if (StateFilter != NULL && !(*StateFilter)(newState)) return currentBest;
	if (newState->targetDistance <= currentBest->targetDistance)
		return newState;
	return currentBest;
}

vector<unique_ptr<ActionHandlerFactory>> AStar::CompletePath(SearchCoords(&buffer)[42][34], SearchCoords* target)
{
	vector<unique_ptr<ActionHandlerFactory>> ret;
	while (target->previousState != nullptr)
	{
		ret.push_back(move(target->action));
		target = target->previousState;
	}
	return ret;
}

void AStar::ShowBuffer(SearchCoords(&buffer)[42][34])
{
	cout << "..";
	for (int i = 0; i < 20; ++i)
	{
		cout << "|";
		cout << "..";
		if (i < 10) cout << ".";
		cout << i;
		cout << "...";
	}
	cout << endl;
	for (int j = 0; j < 15; ++j)
	{
		if (j < 10) cout << ".";
		cout << j;
		cout << "|";
		for (int i = 0; i < 20; ++i)
		{
			if (buffer[i][j].action == nullptr) cout << "......."; //no action
			else
			{
				SearchCoords* node = &buffer[i][j];
				std::cout << node->action->GetActionDescrition();
				int x = node->previousState->x;
				int y = node->previousState->y;
				if (x < 10) cout << ".";
				std::cout << x;
				std::cout << ",";
				if (y < 10) cout << ".";
				std::cout << y;
			}
			cout << "|";
		}
		cout << endl;
	}
}