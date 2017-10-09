#include "stdafx.h"
#include "MovementExecutingWrapper.h"

void MovementExecutingWrapper::Update()
{
	executor->Update();
	if (executor->IsJumping()) executor->JumpStop();
	MoveCallback();
}
bool MovementExecutingWrapper::StartMovingRight(unique_ptr<function<bool()>> callback)
{
	if (movingCallback != nullptr) return false;
	cout << "Right move start\n";
	movingCallback = move(callback);
	executor->MoveRightStart();
}
bool MovementExecutingWrapper::StartMovingLeft(unique_ptr<function<bool()>> callback)
{
	if (movingCallback != nullptr) return false;
	cout << "Left move start\n";
	movingCallback = move(callback);
	executor->MoveLeftStart();
}
void MovementExecutingWrapper::Jump()
{
	executor->JumpStart();
}

void MovementExecutingWrapper::MoveCallback()
{
	if (executor->IsMovingRight())
	{
		if (!CheckCallback())
		{
			executor->MoveRightStop();
		}
	}
	else if (executor->IsMovingLeft())
	{
		if (!CheckCallback())
		{
			executor->MoveLeftStop();
		}
	}
}

bool MovementExecutingWrapper::CheckCallback()
{
	if (movingCallback == nullptr || !(*movingCallback)())
	{
		movingCallback = nullptr;
		return false;
	}
	else
		return true;
}