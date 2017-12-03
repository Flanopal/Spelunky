#include "stdafx.h"
#include "MovementExecutingWrapper.h"

void MovementExecutingWrapper::Update()
{
	executor->Update();
	if (executor->IsJumping())
	{
		if (jumpLength == 0)
			executor->JumpStop();
		else
			--jumpLength;
	}
	MoveCallback();
}
bool MovementExecutingWrapper::StartMovingRight(unique_ptr<function<void()>> callback)
{
	if (horizontalMovingCallback != nullptr) return false;
	cout << "Right move start\n";
	horizontalMovingCallback = move(callback);
	executor->MoveRightStart();
	return true;
}
bool MovementExecutingWrapper::StartMovingLeft(unique_ptr<function<void()>> callback)
{
	if (horizontalMovingCallback != nullptr) return false;
	cout << "Left move start\n";
	horizontalMovingCallback = move(callback);
	executor->MoveLeftStart();
	return true;
}
bool MovementExecutingWrapper::StartLookingUp(unique_ptr<function<void()>> callback)
{
	if (verticalMovingCallback != nullptr) return false;
	cout << "Looking up start\n";
	verticalMovingCallback = move(callback);
	executor->LookUpStart();
	return true;
}
bool MovementExecutingWrapper::StartCrouching(unique_ptr<function<void()>> callback)
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
	verticalMovingCallback = nullptr;
	cout << "Vertical movement stopped\n";
}

bool MovementExecutingWrapper::RegisterUpdateCallback(unique_ptr<function<void()>> callback)
{
	if (updateCallback != nullptr) return false;
	else updateCallback = move(callback);
	return true;
}
void MovementExecutingWrapper::RemoveUpdateCallback()
{
	updateCallback = nullptr;
}

void MovementExecutingWrapper::Jump(int ticks)
{
	cout << "Jump\n";
	jumpLength = ticks;
	executor->JumpStart();
}

void MovementExecutingWrapper::MoveCallback()
{
	if (horizontalMovingCallback != nullptr)
		(*horizontalMovingCallback)();
	if (verticalMovingCallback != nullptr)
		(*verticalMovingCallback)();
	if (updateCallback != nullptr)
		(*updateCallback)();
}