#include "stdafx.h"
#include "BaseMovementClasses.h"


// ------ MOVE FOR ------
bool BaseMVClasses::MoveFor::Start()
{
	bool started;
	if (dist <= 0) return false;
	if (left)
		started = wrapper->StartMovingLeft(make_unique<function<void(bool)>>(bind(&MoveFor::MyCallback, this, placeholders::_1)));
	else
		started = wrapper->StartMovingRight(make_unique<function<void(bool)>>(bind(&MoveFor::MyCallback, this, placeholders::_1)));
		
	if (started)
	{
		state = ActionState::runnig;
		cout << "Starting moving\n";
		return true;
	}
	else
	{
		state = ActionState::terminated;
		cout << "Moving start failed\n";
		return false;
	}
}

void BaseMVClasses::MoveFor::Stop()
{
	if (!CanBeStopped()) return;
	wrapper->StopHorizontalMoving();
}

void BaseMVClasses::MoveFor::MyCallback(bool stopped)
{
	if (!StopControl(stopped))
	{
		if (dist <= 1)
		{
			Finish();
		}
		--dist;
	}
	callParentCallback(stopped);
}


// ------ SIDE MOVE AT ------
bool BaseMVClasses::SideMoveAt::Start()
{
	double positionX = bot->GetPlayerPositionXNode();
	if (EqualValues(positionX, coordX))
	{
		state = ActionState::finished;
		return true;
	}
	if (positionX < coordX)	left = false;
	else left = true;
	
	bool started;
	if (left)
	{
		coordX += 0.2;
		started = wrapper->StartMovingLeft(make_unique<function<void(bool)>>(bind(&SideMoveAt::MyCallback, this, placeholders::_1)));
	}
	else
	{
		coordX -= 0.2;
		started = wrapper->StartMovingRight(make_unique<function<void(bool)>>(bind(&SideMoveAt::MyCallback, this, placeholders::_1)));
	}

	if (started)
	{
		state = ActionState::runnig;
		cout << "Gonna get to position " << coordX << '\n';
		return true;
	}
	else
	{
		state = ActionState::terminated;
		return false;
	}
}
void BaseMVClasses::SideMoveAt::Stop()
{	
	if (!CanBeStopped()) return;
	wrapper->StopHorizontalMoving();
}
void BaseMVClasses::SideMoveAt::MyCallback(bool stopped)
{
	if (!StopControl(stopped))
	{
		Coordinates curPos = bot->GetPlayerCoordinates();
		if (EqualValues(curPos.x, coordX))
		{
			cout << "Here we are at " << coordX << '\n';
			Finish();
		}
		else
		{
			if (!controler->Update())
			{
				Stop();
			}
		}
	}
	callParentCallback(stopped);
}


// ------ JUMP ------
bool BaseMVClasses::Jump::Start()
{
	cout << "Jump started\n";
	state = ActionState::finished;
	lib->playerActions->movements->Jump(ticks);
	return true;
}

// ------ JUMP TO SPOT ------
bool BaseMVClasses::JumpToSpot::Start()
{
	double playerX = bot->GetPlayerPositionXNode();
	double playerY = bot->GetPlayerPositionYNode();
	if (playerX - target.x < 0) dx = 1;
	else dx = -1;
	double dy = playerY - target.y;
	if (dy > 2.5) return false;
	//if (!lib->mapControl->NodeIsTerrain(playerX, playerY + 1)) return false;
	// controls done

	horizontalMove = lib->playerActions->movements->SideMoveAt(target.x);
	horizontalMove->registrCallback(make_unique<function<void(bool)>>(bind(&JumpToSpot::MyCallback, this, placeholders::_1)));
	if (!horizontalMove->Start())
	{
		cout << "Cannot jump\n";
		return false;
	}
	else
	{
		cout << "lets jump\n";
		lib->playerActions->movements->Jump(3);
		return true;
	}
}

void BaseMVClasses::JumpToSpot::Stop()
{
	horizontalMove->Stop();
}
ActionState BaseMVClasses::JumpToSpot::GetState()
{
	if (horizontalMove == nullptr) return ActionState::terminated;
	return horizontalMove->GetState();
}
void BaseMVClasses::JumpToSpot::MyCallback(bool stopped)
{
	if (!stopped)
	{
		double Y = bot->GetPlayerPositionYNode();
		double X = bot->GetPlayerPositionXNode();
		if (!lib->mapControl->NodeIsTerrain(X, Y + 1))
			if (lib->mapControl->NodeIsTerrain(X + dx, Y))
				lib->playerActions->movements->Jump(3);
	}
	callParentCallback(stopped);
}


// ------ CLIMB TO NODE LEVEL ------
bool BaseMVClasses::ClimbToNodeLevel::Start()
{
	double Y = bot->GetPlayerPositionYNode();
	targetX = bot->GetPlayerPositionXNode();
	if (Y == targetLvl)
	{
		state = ActionState::finished;
		return true;
	}
	if (Y > targetLvl) up = true;
	else up = false;

	if (!lib->mapControl->NodeIsClimable(targetX, Y))
	{
		state = ActionState::terminated;
		return false;
	}

	bool started;
	//wrapper->Jump(1);
	if (up)
		started = wrapper->StartLookingUp(make_unique<function<void(bool)>>(bind(&ClimbToNodeLevel::MyCallback, this, placeholders::_1)));
	else
		started = wrapper->StartCrouching(make_unique<function<void(bool)>>(bind(&ClimbToNodeLevel::MyCallback, this, placeholders::_1)));

	if (started)
	{
		cout << "Climbing\n";
		state = ActionState::runnig;
		return true;
	}
	else
	{
		cout << "Climbing failed\n";
		state = ActionState::terminated;
		return false;
	}
}

void BaseMVClasses::ClimbToNodeLevel::MyCallback(bool stopped)
{
	if (!StopControl(stopped))
	{
		double Y = bot->GetPlayerPositionYNode();
		if (EqualValues(Y, targetLvl))
			Finish();
		double dif = 1;
		if (up)
			dif = -1;
		if (!lib->mapControl->NodeIsClimable(targetX, Y + dif))
			Stop();
	}
	callParentCallback(stopped);
}
void BaseMVClasses::ClimbToNodeLevel::Stop()
{
	if (!CanBeStopped()) return;
	cout << "Stop climbing\n";
	wrapper->StopVerticalMoving();
}


// ------ LEAVE CLIMBING ------
bool BaseMVClasses::LeaveClimbing::Start()
{
	double coordX = bot->GetPlayerPositionXNode();
	state = ActionState::terminated; // if control won´t pass, remmains terminated

	if (dir == LeaveDirection::left) coordX -= 0.6;
	else if (dir == LeaveDirection::right) coordX += 0.6;
	if (lib->mapControl->NodeIsTerrain(coordX, bot->GetPlayerPositionYNode())) return false;
	horizontalMove = lib->playerActions->movements->SideMoveAt(coordX);
	if (!horizontalMove->Start()) return false;
	if (!wrapper->RegisterUpdateCallback(make_unique<function<void(bool)>>(bind(&LeaveClimbing::MyCallback, this, placeholders::_1))))
	{
		horizontalMove->Stop();
		return false;
	}
	wrapper->Jump(1);
	state = ActionState::runnig;
	return true;
}

void BaseMVClasses::LeaveClimbing::Stop()
{
	if (!CanBeStopped()) return;
	horizontalMove->Stop();
	wrapper->RemoveUpdateCallback();
}

void BaseMVClasses::LeaveClimbing::MyCallback(bool stopped)
{
	if (!StopControl(stopped))
	{
		if (state == ActionState::runnig)
			if (!controler->Update())
			{
				Finish();
			}
	}
	callParentCallback(stopped);
}
 

// ------ WAIT ------
bool BaseMVClasses::Wait::Start()
{
	if (wrapper->SetWaiting(waitTime, make_unique<function<void(bool)>>(bind(&Wait::MyCallback, this, placeholders::_1))))
	{
		state = ActionState::runnig;
		return true;
	}
	state = ActionState::terminated;
	return false;
}
void BaseMVClasses::Wait::Stop()
{
	if (!CanBeStopped()) return;
	wrapper->StopWaiting();
}
void BaseMVClasses::Wait::MyCallback(bool stopped)
{
	if (!StopControl(stopped))
	{
		--waitTime;
		if (waitTime == 0)
		{
			state = ActionState::finished;
		}
	}
	callParentCallback(stopped);
}


// ------ ACTION LIST ------
void BaseMVClasses::ActionList::AddAction(unique_ptr<ActionHandler> action)
{
	cout << "Add action\n";
	if (action == nullptr) return;
	actions.push_back(move(action));
	if (index == -1) index = 0;
}
void BaseMVClasses::ActionList::AddAction(unique_ptr<ActionHandlerFactory> action)
{
	cout << "Add action factory\n";
	if (action == nullptr) return;
	AddAction(action->GetAction(lib));
}
bool BaseMVClasses::ActionList::Start()
{
	cout << "Action list started\n";
	if (index >= 0 && GetState() == ActionState::waiting)
	{
		actions[index]->registrCallback(make_unique<function<void(bool)>>(bind(&ActionList::MyCallback, this, placeholders::_1)));
		return actions[index]->Start();
	}
	return false;
}
void BaseMVClasses::ActionList::Stop()
{
	actions[index]->Stop();
}
ActionState BaseMVClasses::ActionList::GetState()
{
	return actions[index]->GetState();
}
void BaseMVClasses::ActionList::MyCallback(bool stopped)
{
	if (!StopControl(stopped))
	{
		if (GetState() == ActionState::finished)
		{
			++index;
			if (index < actions.size())
			{
				cout << "next action\n";
				actions[index]->registrCallback(make_unique<function<void(bool)>>(bind(&ActionList::MyCallback, this, placeholders::_1)));
				actions[index]->Start();
			}
			else
			{
				cout << "Last action finished\n";
				--index;
			}
		}
	}
	callParentCallback(stopped);
}


