#include "stdafx.h"
#include"ActionClasses.h"

// ------ EXECUTE PATH ------
ActionClasses::ExecutePath::ExecutePath(FrameworkLibrary* lib, IBotAPI* bot, vector<unique_ptr<ActionHandlerFactory>> path) :lib(lib), bot(bot), path(move(path))
{
	// action in path planned by planner starts at last element
	currentActionIndex = this->path.size();
}
bool ActionClasses::ExecutePath::Start()
{
	state = ActionState::runnig;
	if(ExecuteNextAction())
	{
		return true;
	}
	return false;
}
void ActionClasses::ExecutePath::Stop()
{
	if (!StopPrepare()) return;
	currentAction->Stop();
	cout << "Execution stopped" << endl;
}
void ActionClasses::ExecutePath::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	ActionState actionState = currentAction->GetState();
	if (actionState == ActionState::finished)
	{
		if (!CoordControl())
		{
			cout << "Wrong place" << endl;
			Stop(); // agent is at wrong coordinates
		}
		else ExecuteNextAction();
	}
	else if (actionState == ActionState::terminated)
	{
		cout << "Terminated " << endl;
		Stop();
	}

	callParentCallback(stopped);
}
bool ActionClasses::ExecutePath::ExecuteNextAction()
{
	while (true)
	{
		--currentActionIndex;
		if (currentActionIndex < 0) // plan is finished
		{
			cout << "Finished" << endl;
			state = ActionState::finished;
			return true;
		}
		if (path[currentActionIndex] == nullptr)
		{
			continue;
		}
		currentAction = path[currentActionIndex]->GetAction(lib);
		if (!currentAction->Start() || currentAction->GetState() == ActionState::terminated)
		{
			state = ActionState::terminated;
			return false;
		}
		if (currentAction->GetState() == ActionState::finished)
		{
			continue;
		}
		currentAction->registrCallback(make_unique<function<void(bool)>>(bind(&ExecutePath::MyCallback, this, placeholders::_1)));
		return true;
	}
}
bool ActionClasses::ExecutePath::CoordControl()
{
	double tollerance = 0.4;
	if (bot->IsClimbing()) tollerance = 4; // climbing is not so precise
	Coordinates current = bot->GetPlayerCoordinates();
	Coordinates target = path[currentActionIndex]->GetFinalCoords();
	int dx=0; 
	int dy=0;
	// in some cases could be one of target coordinates irrelevant - not defined
	if (target.x > 0) dx = abs((int)current.x - (int)target.x);
	if (target.y > 0) dy = abs((int)current.y - (int)target.y);
	return (dx < tollerance && dy < tollerance);
}



// ------ GO DIRECTLY TO NODE ------
bool ActionClasses::GoDirectlyToNode::Start()
{
	if ((pathExecutor != NULL || PlanPath()) && pathExecutor->Start())
	{
		pathExecutor->registrCallback(make_unique<function<void(bool)>>(bind(&GoDirectlyToNode::MyCallback, this, placeholders::_1)));
		state = ActionState::runnig;
		return true;
	}
	state = ActionState::terminated;
	return false;
}
void ActionClasses::GoDirectlyToNode::Stop()
{
	if (!StopPrepare()) return;
	pathExecutor->Stop();
}
void ActionClasses::GoDirectlyToNode::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	ActionState executorState = pathExecutor->GetState();
	if (executorState != ActionState::runnig)
	{
		Stop();
		state = executorState;
	}

	callParentCallback(stopped);
}
/*bool ActionClasses::GoDirectlyToNode::SetTerminatedState()
{
	Coordinates currentPossition = bot->GetPlayerCoordinates();
	State state(currentPossition);
	if (terminatedStates.find(state) == terminatedStates.end())
	{
		// state was not visited
		terminatedStates.insert(state);
		return true;
	}
	return false;
}*/
bool ActionClasses::GoDirectlyToNode::PlanPath()
{
	Coordinates currentPossition = bot->GetPlayerCoordinates();
	if (life > 0)
	{
		lib->pathSearch->SetLifesForSearch(life);
		lib->pathSearch->SetRopesForSearch(rope);
	}
	vector<unique_ptr<ActionHandlerFactory>> path = lib->pathSearch->FindAndGetPath(currentPossition, target);
	Coordinates finish = path[0]->GetFinalCoords();
	if (!NearbyCoords(finish, target))
	{
		cout << "There is no Direct way " << endl;
		return false;
	}
	pathExecutor = lib->playerActions->ExecutePath(move(path));
	return true;
}
bool ActionClasses::GoDirectlyToNode::NearbyCoords(Coordinates first, Coordinates second)
{
	// control if there is path in space to target (closest possible point)
	// first match x coord, then fix y coord
	if (lib->mapControl->NodeIsUnknown(second)) return false;
	int dx = 1;
	if (first.x > second.x) dx = -1;
	while (abs(first.x - second.x) > 1)
	{
		first.x += dx;
		if (!lib->mapControl->NodeIsEmpty(first)) return false; // obstacle - there is a better possition
	}
	int dy = 1;
	if (first.y < second.y) dy = -1;
	while (abs(first.y - second.y) > 1)
	{
		first.y += dy;
		if (!lib->mapControl->NodeIsEmpty(first)) return false;
	}
	return true; // is free space path 
}


//------ EXPLORE TO NODE ------
bool ActionClasses::ExploreToNode::Start()
{
	if ((pathExecutor != NULL || PlanPath()) && pathExecutor->Start())
	{
		pathExecutor->registrCallback(make_unique<function<void(bool)>>(bind(&ExploreToNode::MyCallback, this, placeholders::_1)));
		state = ActionState::runnig;
		return true;
	}
	state = ActionState::terminated;
	return false;
}
void ActionClasses::ExploreToNode::Stop()
{
	if (!StopPrepare()) return;
	pathExecutor->Stop();
}
void ActionClasses::ExploreToNode::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	ActionState executorState = pathExecutor->GetState();
	if (executorState != ActionState::runnig)
	{
		Stop();
		state = executorState;
	}

	callParentCallback(stopped);
}
bool ActionClasses::ExploreToNode::PlanPath()
{
	Coordinates currentPossition = bot->GetPlayerCoordinates();
	if (lib->mapControl->NodeIsUnknown(target) || lib->pathSearch->FindAndGetPathToUnknown(target, currentPossition).size() != 0)
	{
		if (life > 0)
		{
			lib->pathSearch->SetLifesForSearch(life);
			lib->pathSearch->SetRopesForSearch(rope);
		}
		vector<unique_ptr<ActionHandlerFactory>> path = lib->pathSearch->FindAndGetPathToUnknown(currentPossition, target);
		if (path.size() == 0) return false;
		pathExecutor = lib->playerActions->ExecutePath(move(path));
		return true;
	}
	return false; // cant do exploration
}