#include "stdafx.h"
#include "MovementExecutingWrapper.h"

void MovementExecutingWrapper::Update()
{	
	if (IsWaiting()) return; // waiting case - no updates
	executor->Update();
	if (executor->IsJumping())
	{
		if (jumpLength == 0)
		{
			executor->JumpStop();
		}
		else
		{
			--jumpLength;
		}
	}
	CallMoveCallback();
	CallUpdateCallback();
}
bool MovementExecutingWrapper::StartMovingRight(WrapperCallback callback)
{
	if (horizontalMovingCallback != nullptr) return false;
	cout << "Right move start\n";
	horizontalMovingCallback = move(callback);
	executor->MoveRightStart();
	return true;
}
bool MovementExecutingWrapper::StartMovingLeft(WrapperCallback callback)
{
	if (horizontalMovingCallback != nullptr) return false;
	cout << "Left move start\n";
	horizontalMovingCallback = move(callback);
	executor->MoveLeftStart();
	return true;
}
bool MovementExecutingWrapper::StartLookingUp(WrapperCallback callback)
{
	if (verticalMovingCallback != nullptr) return false;
	cout << "Looking up start\n";
	verticalMovingCallback = move(callback);
	executor->LookUpStart();
	return true;
}
bool MovementExecutingWrapper::StartCrouching(WrapperCallback callback)
{
	if (verticalMovingCallback != nullptr) return false;
	cout << "Crouch start\n";
	verticalMovingCallback = move(callback);
	executor->CrouchStart();
	return true;
}
void MovementExecutingWrapper::StopHorizontalMoving()
{
	if (executor->IsMovingRight())
	{
			executor->MoveRightStop();
	}
	else if (executor->IsMovingLeft())
	{
			executor->MoveLeftStop();
	}
	if (horizontalMovingCallback != nullptr) (*horizontalMovingCallback)(true);
	horizontalMovingCallback = nullptr;
	cout << "Horizontal movement stopped\n";
}
void MovementExecutingWrapper::StopVerticalMoving()
{
	if (executor->IsLookingUp())
	{
		executor->LookUpStop();
	}
	else if (executor->IsCrouching())
	{
		executor->CrouchStop();
	}
	if (verticalMovingCallback != nullptr) (*verticalMovingCallback)(true);
	verticalMovingCallback = nullptr;
	cout << "Vertical movement stopped\n";
}

bool MovementExecutingWrapper::RegisterUpdateCallback(WrapperCallback callback)
{
	if (updateCallback != nullptr) return false;
	else updateCallback = move(callback);
	return true;
}
void MovementExecutingWrapper::RemoveUpdateCallback()
{
	if (updateCallback != nullptr) (*updateCallback)(true);
	updateCallback = nullptr;
}
bool MovementExecutingWrapper::SetWaiting(int time, WrapperCallback callback)
{
	if (waitCallback != nullptr) return false;
	waitCallback = move(callback);
	waitTime = time;
	return true;
}
void MovementExecutingWrapper::StopWaiting()
{
	waitTime = 0;
	if (waitCallback != nullptr) (*waitCallback)(true);
	waitCallback = nullptr;
}
void MovementExecutingWrapper::Jump(int ticks)
{
	jumpLength = ticks;
	executor->JumpStart();
}

void MovementExecutingWrapper::CallMoveCallback()
{
	if (horizontalMovingCallback != nullptr)
		(*horizontalMovingCallback)(false);
	if (verticalMovingCallback != nullptr)
		(*verticalMovingCallback)(false);
	
}
void MovementExecutingWrapper::CallUpdateCallback()
{
	if (updateCallback != nullptr)
		(*updateCallback)(false);
	if (waitCallback != nullptr)
		(*waitCallback)(false);
}

bool MovementExecutingWrapper::IsWaiting()
{
	if (waitTime > 0)
	{
		--waitTime;
		CallUpdateCallback();
		if (waitTime == 0) waitCallback = nullptr;
		return true;
	}
	return false;
}