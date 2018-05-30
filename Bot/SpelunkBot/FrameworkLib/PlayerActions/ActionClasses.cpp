#include "stdafx.h"
#include"ActionClasses.h"

// ------ EXECUTE PATH ------
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
}
void ActionClasses::ExecutePath::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	ActionState actionState = currentAction->GetState();
	if (actionState == ActionState::finished)
	{
		if (!CoordControl()) Stop(); // agent is at wrong coordinates
		else ExecuteNextAction();
	}
	else if (actionState == ActionState::terminated) Stop();

	callParentCallback(stopped);
}
bool ActionClasses::ExecutePath::ExecuteNextAction()
{
	while (true)
	{
		if (currentActionIndex >= path.size()) // plan is finished
		{
			state = ActionState::finished;
			return true;
		}
		if (path[currentActionIndex] == nullptr)
		{
			++currentActionIndex;
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
			++currentActionIndex;
			continue;
		}
		currentAction->registrCallback(make_unique<function<void(bool)>>(bind(&ExecutePath::MyCallback, this, placeholders::_1)));
		return true;
	}
}
bool ActionClasses::ExecutePath::CoordControl()
{
	Coordinates current = bot->GetPlayerCoordinates();
	Coordinates target = path[currentActionIndex]->GetFinalCoords();
	int dx=0; 
	int dy=0;
	// in some cases could be one of target coordinates irrelevant
	if (target.x > 0) dx = abs((int)current.x - (int)target.x);
	if (target.y > 0) dy = abs((int)current.y - (int)target.y);
	return (dx == 0 && dy == 0);
}


/*
// ------ GO TO NODE ------
bool ActionClasses::GoToNode::Start()
{
	SetTerminatedState();
	if (ReplanPath())
	{
		state = ActionState::runnig;
		return true;
	}
	return false;
}
void ActionClasses::GoToNode::Stop()
{
	if (!StopPrepare()) return;
	pathExecutor->Stop();
}
void ActionClasses::GoToNode::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	ActionState executorState = pathExecutor->GetState();
	if (executorState == ActionState::finished)
	{
		if (!SetTerminatedState()) Finish(); // closest point - was visited and finished in
		else if (!ReplanPath()) Stop(); // new plan, we could go closer by exploring
	}
	else if (executorState == ActionState::terminated)
	{
		if (!SetTerminatedState() || !ReplanPath()) Stop(); // bad state visited twice - error cycle
	}

	callParentCallback(stopped);
}
bool ActionClasses::GoToNode::SetTerminatedState()
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
}
bool ActionClasses::GoToNode::ReplanPath()
{
	Coordinates currentPossition = bot->GetPlayerCoordinates();
	vector<unique_ptr<ActionHandlerFactory>> path = lib->pathSearch->FindPathAndGetPath(currentPossition, target);
	pathExecutor = lib->playerActions->ExecutePath(move(path));
	if (!pathExecutor->Start())
	{
		state = ActionState::terminated;
		return false;
	}
	pathExecutor->registrCallback(make_unique<function<void(bool)>>(bind(&GoToNode::MyCallback, this, placeholders::_1)));
	return true;
}*/