#include "stdafx.h"
#include"ActionClasses.h"


// ------ PICK ROPE OR OPEN CRATE ------
bool ActionClasses::PickRopeOrOpenCrate::Start()
{
	cout << ">> PICK ROPE OR GET CHEST STARTED " << endl;
	Coordinates pos = bot->GetPlayerCoordinates();
	if (bot->GetIDOfCollectableInNode(spCrate, pos.x, pos.y, 0) > 0.5) // crate case 
	{
		crateTimer = 0;
		action = lib->playerActions->movements->LookUpFor(40);
		if (action->Start())
		{
			action->registrCallback(make_unique<function<void(bool)>>(bind(&PickRopeOrOpenCrate::MyCallback, this, placeholders::_1)));
			return true;
		}
		return false;
	}
	if (bot->GetIDOfCollectableInNode(spRope, pos.x, pos.y, 0) > 0.5) // rope case
	{
		action = lib->playerActions->movements->CrouchFor(20);
		if (action->Start())
		{
			action->registrCallback(make_unique<function<void(bool)>>(bind(&PickRopeOrOpenCrate::MyCallback, this, placeholders::_1)));
			return true;
		}
		return false;
	}
	state = ActionState::terminated;
	return false;	
}
void ActionClasses::PickRopeOrOpenCrate::Stop()
{
	if (!StopPrepare()) return;
	action->Stop();
}
ActionState ActionClasses::PickRopeOrOpenCrate::GetState()
{
	if (action != nullptr) return action->GetState();
	return state;
}
void ActionClasses::PickRopeOrOpenCrate::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	if (crateTimer >= 0) // crate case requireds attack while looking up
	{
		++crateTimer;
		if (crateTimer > 8 && crateTimer < 12)
			lib->playerActions->toolsActions->Attack();
	}
	if (action->GetState() == ActionState::finished)
	{
		lib->mapControl->RemoveRopeOrChest();
	}
	callParentCallback(stopped);
}



// ------ EXECUTE PATH ------
ActionClasses::ExecutePath::ExecutePath(FrameworkLibrary* lib, IBotAPI* bot, vector<unique_ptr<ActionHandlerFactory>> path) :lib(lib), bot(bot), path(move(path))
{
	// action in path planned by planner starts at last element
	currentActionIndex = this->path.size();
}
bool ActionClasses::ExecutePath::Start()
{
	cout << ">> EXECUTE PATH" << endl;
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
		if (!CoordControl())
		{
			cout << "Wrong place" << endl;
			Coordinates current = bot->GetPlayerCoordinates();
			Coordinates target = path[currentActionIndex]->GetFinalCoords();
			cout << "current " << current.x << " " << current.y << endl;
			cout << "wanted " << target.x << " " << target.y << endl;
			Stop(); // agent is at wrong coordinates
		}
		else ExecuteNextAction();
	}
	else if (actionState == ActionState::terminated)
	{
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
			state = ActionState::finished;
			return true;
		}
		if (path[currentActionIndex] == nullptr) // empty action
		{
			continue;
		}
		currentAction = path[currentActionIndex]->GetAction(lib);
		if (!currentAction->Start() || currentAction->GetState() == ActionState::terminated)
		{
			state = ActionState::terminated;
			return false;
		}
		if (currentAction->GetState() == ActionState::finished) // action finished immediately 
		{
			continue;
		}
		currentAction->registrCallback(make_unique<function<void(bool)>>(bind(&ExecutePath::MyCallback, this, placeholders::_1)));
		return true;
	}
}
bool ActionClasses::ExecutePath::CoordControl()
{
	double tollerance = 0.9;
	if (bot->IsClimbing()) tollerance = 4; // climbing is not so precise -> bigger tolerance
	Coordinates current = bot->GetPlayerCoordinates();
	Coordinates target = path[currentActionIndex]->GetFinalCoords();
	double dx=0; 
	double dy=0;
	// in some cases could be one of target coordinates irrelevant - not defined
	if (target.x > 0) dx = abs(current.x - target.x);
	if (target.y > 0) dy = abs(current.y - target.y);
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
bool ActionClasses::GoDirectlyToNode::PlanPath()
{
	if (pathExecutor != NULL && state == ActionState::waiting) return true; // don´t want to plan more times
	if (state == ActionState::finished) return true;
	if (state == ActionState::terminated) return false;

	Coordinates currentPossition = bot->GetPlayerCoordinates();
	if (life > 0)
	{
		lib->pathSearch->SetLifesForSearch(life);
		lib->pathSearch->SetRopesForSearch(rope);
	}
	vector<unique_ptr<ActionHandlerFactory>> path = lib->pathSearch->FindAndGetPath(currentPossition, target);
	if (path.size() == 0)
	{
		Finish(); // already in the spot
		return true;
	}
	Coordinates finish = path[0]->GetFinalCoords();
	if (!NearbyCoords(finish, target))
	{
		cout << "There is no Direct way " << endl;
		state = ActionState::terminated;
		return false;
	}
	life = path[0]->GetAgentStateAfterExecuting().lifeCount;
	rope = path[0]->GetAgentStateAfterExecuting().ropeCount;
	pathExecutor = lib->playerActions->ExecutePath(move(path));
	return true;
}
int ActionClasses::GoDirectlyToNode::FinalLifeCount()
{
	return life;
}
int ActionClasses::GoDirectlyToNode::FinalRopeCount()
{
	return rope;
}
bool ActionClasses::GoDirectlyToNode::NearbyCoords(Coordinates first, Coordinates second)
{
	// control if there is path in space to target (closest possible point)
	// first fix x coord, then fix y coord
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
	while (abs(first.y - second.y) > 1 )
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
	return false;
}
void ActionClasses::ExploreToNode::Stop()
{
	if (!StopPrepare()) return;
	pathExecutor->Stop();
}
bool ActionClasses::ExploreToNode::PlanPath()
{
	if (pathExecutor != NULL && state == ActionState::waiting) return true; // don´t want to plan more times
	if (state == ActionState::terminated) return false;

	Coordinates currentPossition = bot->GetPlayerCoordinates();
	if (lib->mapControl->NodeIsUnknown(target) || lib->pathSearch->FindAndGetPathToUnknown(target, currentPossition).size() != 0)
	{
		if (life > 0)
		{
			lib->pathSearch->SetLifesForSearch(life);
			lib->pathSearch->SetRopesForSearch(rope);
		}
		vector<unique_ptr<ActionHandlerFactory>> path = lib->pathSearch->FindAndGetPathToUnknown(currentPossition, target);
		if (path.size() < 2)
		{
			state = ActionState::terminated;
			return false;
		}
		path[0] = nullptr; // last action is ignored - can be dangerous to step into unknown node
		life = path[1]->GetAgentStateAfterExecuting().lifeCount;
		rope = path[1]->GetAgentStateAfterExecuting().ropeCount;
		pathExecutor = lib->playerActions->ExecutePath(move(path));
		return true;
	}
	state = ActionState::terminated;
	return false; // cant do exploration
}
int ActionClasses::ExploreToNode::FinalLifeCount()
{
	return life;
}
int ActionClasses::ExploreToNode::FinalRopeCount()
{
	return rope;
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
