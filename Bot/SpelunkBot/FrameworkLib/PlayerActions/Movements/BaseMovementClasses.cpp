#include "stdafx.h"
#include "BaseMovementClasses.h"


// ------ MOVE FOR ------
bool BaseMVClasses::MoveFor::Start()
{
	cout << ">> MOVE FOR" << endl;
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
	if (!StopPrepare()) return;
	wrapper->StopHorizontalMoving();
}
void BaseMVClasses::MoveFor::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	if (dist <= 1)
	{
		Finish();
	}
	--dist;
	callParentCallback(stopped);
}


// ------ LOOK UP FOR ------
bool BaseMVClasses::LookUpFor::Start()
{
	cout << ">> LOOK UP FOR "  << ticks << " TICKS" << endl;
	if (ticks <= 0) return false;
	if (wrapper->StartLookingUp(make_unique<function<void(bool)>>(bind(&LookUpFor::MyCallback, this, placeholders::_1))))
	{
		state = ActionState::runnig;
		return true;
	}
	state = ActionState::terminated;
	return false;
}
void BaseMVClasses::LookUpFor::Stop()
{
	if (!StopPrepare()) return;
	wrapper->StopVerticalMoving();
}
void BaseMVClasses::LookUpFor::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	if (ticks <= 1) Finish();
	--ticks;
	callParentCallback(stopped);
}


// ------ SIDE MOVE AT ------
bool BaseMVClasses::SideMoveAt::Start()
{
	cout << ">> SIDE MOVE TO POSSITION " << coordX << '\n';
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
		return true;
	}
	else
	{
		cout << "Side move failed" << endl;
		state = ActionState::terminated;
		return false;
	}
}
void BaseMVClasses::SideMoveAt::Stop()
{
	if (!StopPrepare()) return;
	wrapper->StopHorizontalMoving();
}
void BaseMVClasses::SideMoveAt::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

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
	callParentCallback(stopped);
}


// ------ JUMP ------
bool BaseMVClasses::Jump::Start()
{
	cout << ">> JUMP STARTED" << endl;
	state = ActionState::finished;
	lib->playerActions->movements->Jump(ticks);
	return true;
}

// ------ JUMP TO SPOT ------
bool BaseMVClasses::JumpToSpot::Start()
{
	cout << ">> JUMP TO ACTION STARTING" << endl;
	double playerX = bot->GetPlayerPositionXNode();
	double playerY = bot->GetPlayerPositionYNode();
	if (playerX - target.x < 0) dx = 1;
	else dx = -1;
	double dy = playerY - target.y;
	if (dy > 2.5) return false;
	// controls done

	horizontalMove = lib->playerActions->movements->SideMoveAt(target.x);
	horizontalMove->registrCallback(make_unique<function<void(bool)>>(bind(&JumpToSpot::MyCallback, this, placeholders::_1)));
	if (!horizontalMove->Start())
	{
		return false;
	}
	else
	{
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
	if (horizontalMove == nullptr) return ActionState::waiting;
	return horizontalMove->GetState();
}
void BaseMVClasses::JumpToSpot::MyCallback(bool stopped)
{
	if (stopped)
	{
		callParentCallback(stopped);
		return;
	}

	if (!terminated && GetState() == ActionState::terminated) // jump failed, agent is hanging
	{
		terminated = true;
		double Y = bot->GetPlayerPositionYNode();
		double X = bot->GetPlayerPositionXNode();
		if (!lib->mapControl->NodeIsTerrain(X, Y + 1) && lib->mapControl->NodeIsTerrain(X + dx, Y))
		{
			horizontalMove = lib->playerActions->movements->SideMoveAt(target.x);
			horizontalMove->registrCallback(make_unique<function<void(bool)>>(bind(&JumpToSpot::MyCallback, this, placeholders::_1)));
			if (horizontalMove->Start())
				lib->playerActions->movements->Jump(3);
		}
	}
	callParentCallback(stopped);
}


// ------ GET ON CLIMBING ------
bool BaseMVClasses::GetOnClimbing::Start()
{
	Coordinates coords = bot->GetPlayerCoordinates();
	cout << ">> GET ON CLIMBING " << coords.x << endl;
	if (!lib->mapControl->NodeIsClimable(coords))
	{
		return false;
	}
	if (bot->IsClimbing()) return true;
	int x = (int)coords.x;
	middleX = x + 0.5;
	targetX = middleX;
	if (coords.x < middleX) targetX = middleX + 0.2;
	else targetX = middleX - 0.2;
	horizontalMove = lib->playerActions->movements->SideMoveAt(targetX);
	lookUp = lib->playerActions->movements->LookUpFor(50);
	lookUp->registrCallback(make_unique<function<void(bool)>>(bind(&GetOnClimbing::MyCallback, this, placeholders::_1)));
	if (!horizontalMove->Start()) return false;
	if (!lookUp->Start())
	{
		horizontalMove->Stop();
		return false;
	}
	state = ActionState::runnig;	
	return true;
}
void BaseMVClasses::GetOnClimbing::Stop()
{
	if (!StopPrepare()) return;
	horizontalMove->Stop();
	lookUp->Stop();
}
void BaseMVClasses::GetOnClimbing::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;
	if (abs(middleX - bot->GetPlayerPositionXNode()) < 0.1)
	{
		middleTick++;
		if (middleTick == 2)
			lib->playerActions->movements->Jump(1);
		
	}
	if (middleTick == 10)
	{
		cout << "Finished" << endl;
		Finish();
		bot->SetStartClimbing();
	}
	else if (middleTick > 0 && abs(targetX - bot->GetPlayerPositionXNode()) > 0.5)
	{
		cout << "Stopped " << endl;
		Stop(); // action failed
	}
	callParentCallback(stopped);
}

// ------ CLIMB TO NODE LEVEL ------
bool BaseMVClasses::ClimbToNodeLevel::Start()
{
	cout << ">> CLIMB TO NODE " << targetLvl << endl;
	double Y = bot->GetPlayerPositionYNode();
	targetX = bot->GetPlayerPositionXNode();
	if (Y == targetLvl)
	{
		state = ActionState::finished;
		return true;
	}
	if (Y > targetLvl) up = true;
	else up = false;
	if (!bot->IsClimbing())
	{
		startClimbing = lib->playerActions->movements->GetOnClimbing();
		startClimbing->registrCallback(make_unique<function<void(bool)>>(bind(&ClimbToNodeLevel::StartClimbingCallback, this, placeholders::_1)));
		if (startClimbing->Start())
		{
			state = ActionState::runnig;
			return true;
		}
		state = ActionState::terminated;
		return false;
	}
	else return StartClimbing();
}
bool BaseMVClasses::ClimbToNodeLevel::StartClimbing()
{
	if (up)
		started = wrapper->StartLookingUp(make_unique<function<void(bool)>>(bind(&ClimbToNodeLevel::MyCallback, this, placeholders::_1)));
	else
		started = wrapper->StartCrouching(make_unique<function<void(bool)>>(bind(&ClimbToNodeLevel::MyCallback, this, placeholders::_1)));
	if (started)
	{
		state = ActionState::runnig;
		return true;
	}
	else
	{
		state = ActionState::terminated;
		return false;
	}
}
void BaseMVClasses::ClimbToNodeLevel::Stop()
{
	if (!StopPrepare()) return;
	if (startClimbing != nullptr) startClimbing->Stop();
	if (started) wrapper->StopVerticalMoving();
}
void BaseMVClasses::ClimbToNodeLevel::StartClimbingCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	if (startClimbing->GetState() == ActionState::finished)
		StartClimbing();
	else if (startClimbing->GetState() == ActionState::terminated) Stop();
	callParentCallback(stopped);
}
void BaseMVClasses::ClimbToNodeLevel::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;
	if (GetState() == ActionState::finished) cout << "Why finished? " << endl;
	double Y = bot->GetPlayerPositionYNode();
	if (EqualValues(Y, targetLvl))
	{
		cout << "climbing finished" << endl;
		Finish();
	}
	double dif = 0.5;
	if (up) dif = -0.5;
	if (!lib->mapControl->NodeIsClimable(targetX, Y + dif))
	{
		Stop();
	}
	callParentCallback(stopped);
}


// ------ LEAVE CLIMBING ------
bool BaseMVClasses::LeaveClimbing::Start()
{
	cout << ">> LEAVE CLIMBING" << endl;
	double coordX = bot->GetPlayerPositionXNode();
	state = ActionState::terminated; // if control won´t pass, remmains terminated

	if (dir == LeaveDirection::left) coordX -= 0.6;
	else if (dir == LeaveDirection::right) coordX += 0.6;
	if (lib->mapControl->NodeIsTerrain(coordX, bot->GetPlayerPositionYNode())) return false;

	action = lib->playerActions->movements->GetActionList();
	action->AddAction(lib->playerActions->movements->SideMoveAt(coordX));
	action->AddAction(lib->playerActions->movements->WaitForLanding());
	action->registrCallback(make_unique<function<void(bool)>>(bind(&LeaveClimbing::MyCallback, this, placeholders::_1)));
	if(!action->Start()) return false;
	lib->playerActions->movements->Jump(1);
	return true;
}
void BaseMVClasses::LeaveClimbing::Stop()
{
	if (!StopPrepare()) return;
	action->Stop();
}
ActionState BaseMVClasses::LeaveClimbing::GetState()
{
	return action->GetState();
}
void BaseMVClasses::LeaveClimbing::MyCallback(bool stopped)
{
	callParentCallback(stopped);
}


// ------ WAIT ------
bool BaseMVClasses::Wait::Start()
{
	cout << ">> WAIT FOR " << waitTime << " TICKS" << endl;
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
	if (!StopPrepare()) return;
	wrapper->StopWaiting();
}
void BaseMVClasses::Wait::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return; 

	--waitTime;
	if (waitTime == 0)
	{
		state = ActionState::finished;
	}
	callParentCallback(stopped);
}


// ------ WAIT FOR LANDING ------
bool BaseMVClasses::WaitForLanding::Start()
{
	cout << ">> WAIT FOR LANDING" << endl;
	Coordinates coords = bot->GetPlayerCoordinates();
	if (wrapper->RegisterUpdateCallback(make_unique<function<void(bool)>>(bind(&WaitForLanding::MyCallback, this, placeholders::_1))))
	{
		state = ActionState::runnig;
		return true;
	}
	state = ActionState::terminated;
	return false;

}
void BaseMVClasses::WaitForLanding::Stop()
{
	if (!StopPrepare()) return;
	wrapper->RemoveUpdateCallback();
}
void BaseMVClasses::WaitForLanding::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;
	Coordinates coords = bot->GetPlayerCoordinates();
	if (map->NodeIsClimable(coords)) Finish();
	else
	{
		coords.y++;
		if (map->NodeIsTerrain(coords))
		{
			Finish();
		}
	}
	callParentCallback(stopped);
}

// ------ ACTION LIST ------
void BaseMVClasses::ActionList::AddAction(unique_ptr<ActionHandler> action)
{
	if (action == nullptr) return;
	actions.push_back(move(action));
	if (index == -1) index = 0;
}
void BaseMVClasses::ActionList::AddAction(unique_ptr<ActionHandlerFactory> action)
{
	cout << "Add action factory " << action->GetActionDescrition() << "\n";
	if (action == nullptr) return;
	AddAction(action->GetAction(lib));
}
bool BaseMVClasses::ActionList::Start()
{
	cout << ">> ACTION LIST STARTED\n";
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
	if (StopControl(stopped)) return;

	if (GetState() == ActionState::finished)
	{
		++index;
		if (index < actions.size())
		{
			if (actions[index] == nullptr) cout << "NULL " << endl;
			cout << "next action\n";
			actions[index]->registrCallback(make_unique<function<void(bool)>>(bind(&ActionList::MyCallback, this, placeholders::_1)));
			actions[index]->Start();
		}
		else
		{
			cout << "Last action finished "<< index << "\n";
			--index;
		}
	}
	callParentCallback(stopped);
}


