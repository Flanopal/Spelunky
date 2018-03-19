#include "stdafx.h"
#include "BaseMovementClasses.h"

bool BaseMVClasses::MoveFor::Start()
{
	if (dist <= 0) return false;
	cout << "Starting moving\n";
	state = ActionState::runnig;
	if (left)
		return wrapper->StartMovingLeft(make_unique<function<void()>>(bind(&MoveFor::MyCallback, this)));
	else
		return wrapper->StartMovingRight(make_unique<function<void()>>(bind(&MoveFor::MyCallback, this)));
}

void BaseMVClasses::MoveFor::Stop()
{
	if (!CanBeStopped()) return;
	wrapper->StopHorizontalMoving();
	state = ActionState::terminated;
}

void BaseMVClasses::MoveFor::MyCallback()
{
	if (dist <= 1)
	{
		Finish();
	}
	--dist;
	callParentCallback();
}

bool BaseMVClasses::SideMoveAt::Start()
{
	cout << "Gonna get to position " << coordX << '\n';
	double positionX = bot->GetPlayerPositionXNode();
	if (EqualValues(positionX, coordX))
	{
		state = ActionState::finished;
		return true;
	}
	if (positionX < coordX)	left = false;
	else left = true;
	
	state = ActionState::runnig;
	if (left)
	{
		coordX += 0.2;
		return wrapper->StartMovingLeft(make_unique<function<void()>>(bind(&SideMoveAt::MyCallback, this)));
	}
	else
	{
		coordX -= 0.2;
		return wrapper->StartMovingRight(make_unique<function<void()>>(bind(&SideMoveAt::MyCallback, this)));
	}
}

void BaseMVClasses::SideMoveAt::Stop()
{	
	if (!CanBeStopped()) return;
	wrapper->StopHorizontalMoving();
	state=ActionState::terminated;
}

void BaseMVClasses::SideMoveAt::MyCallback()
{
	Coordinates curPos(bot->GetPlayerPositionXNode(), bot->GetPlayerPositionYNode());
	if (EqualValues(curPos.x, coordX))
	{
		cout << "Here we are at " << coordX << '\n';
		Finish();
	}
	else
	{
		if (!controler->Update())
			Stop();
	}
	callParentCallback();
}

bool BaseMVClasses::JumpToSpot::Start()
{
	double playerX = bot->GetPlayerPositionXNode();
	double playerY = bot->GetPlayerPositionYNode();
	double dx = playerX - target.x;
	double dy = playerY - target.y;
	if (dy > 2) return false;
	if (!lib->mapControl->NodeIsTerrain(playerX, playerY + 1)) return false;
	// controls done
	cout << "lets jump\n";
	horizontalMove = lib->playerActions->movements->SideMoveAt(target.x);
	horizontalMove->registrCallback(make_unique<function<void()>>(bind(&JumpToSpot::MyCallback, this)));
	if (!horizontalMove->Start()) return false;
	lib->playerActions->movements->Jump();
	return true;
}

void BaseMVClasses::JumpToSpot::Stop()
{
	horizontalMove->Stop();
}
ActionState BaseMVClasses::JumpToSpot::GetState()
{
	return horizontalMove->GetState();
}
void BaseMVClasses::JumpToSpot::MyCallback()
{
	double Y = bot->GetPlayerPositionYNode();
	double X = bot->GetPlayerPositionXNode();
	if (!lib->mapControl->NodeIsTerrain(X, Y + 1))
		if (lib->mapControl->NodeIsTerrain(X + 1, Y))
			lib->playerActions->movements->Jump();
	callParentCallback();
}

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

	if (!lib->mapControl->NodeIsClimable(targetX, Y)) return false;

	state = ActionState::runnig;
	//wrapper->Jump(1);
	if (up)
		return wrapper->StartLookingUp(make_unique<function<void()>>(bind(&ClimbToNodeLevel::MyCallback, this)));
	else
		return wrapper->StartCrouching(make_unique<function<void()>>(bind(&ClimbToNodeLevel::MyCallback, this)));
}

void BaseMVClasses::ClimbToNodeLevel::MyCallback()
{
	double Y = bot->GetPlayerPositionYNode();
	if (EqualValues(Y, targetLvl))
		Finish();
	double dif = 1;
	if (up)
		dif = -1;
	if (!lib->mapControl->NodeIsClimable(targetX, Y+dif))
		Stop();
	callParentCallback();
}
void BaseMVClasses::ClimbToNodeLevel::Stop()
{
	if (!CanBeStopped()) return;
	cout << "Stop climbing\n";
	wrapper->StopVerticalMoving();
	state = ActionState::terminated;
}

bool BaseMVClasses::LeaveClimbing::Start()
{
	double coordX = bot->GetPlayerPositionXNode();
	if (dir == LeaveDirection::left) coordX -= 0.6;
	else if (dir == LeaveDirection::right) coordX += 0.6;
	if (lib->mapControl->NodeIsTerrain(coordX, bot->GetPlayerPositionYNode())) return false;
	horizontalMove = lib->playerActions->movements->SideMoveAt(coordX);
	if (!horizontalMove->Start()) return false;
	if (!wrapper->RegisterUpdateCallback(make_unique<function<void()>>(bind(&LeaveClimbing::MyCallback, this))))
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
	state = ActionState::terminated;
	cout << "Succesfully leaved\n";
}

void BaseMVClasses::LeaveClimbing::MyCallback()
{
	if (state == ActionState::runnig)
		if (!controler->Update())
		{
			Finish();
		}
	callParentCallback();
}
 
bool BaseMVClasses::ClimbToNodeLevelAndEscapeLadder::Start()
{
	return climb->Start();
}
void BaseMVClasses::ClimbToNodeLevelAndEscapeLadder::Stop()
{
	if (climb->GetState() == ActionState::runnig) climb->Stop();
	else escape->Stop();
}
ActionState BaseMVClasses::ClimbToNodeLevelAndEscapeLadder::GetState()
{
	ActionState state = climb->GetState();
	if (state == ActionState::finished) state = escape->GetState();
	return state;
}
void BaseMVClasses::ClimbToNodeLevelAndEscapeLadder::MyCallback()
{
	if (climb->GetState() == ActionState::finished && escape->GetState() == ActionState::waiting)
		escape->Start();
	callParentCallback();
}


