#include "stdafx.h"
#include "MovementExecuting.h"

void MovementExecuting::Update()
{
	size_t size = actions.size();
	for (int i = 0; i < size; ++i)
	{		
		actions[i]->callback();
	}
}

void MovementExecuting::MoveRightStart()
{
	if (!_right)
	{
		actions.push_back(&*moveRight);
		_right = true;
	}
}
void MovementExecuting::MoveRightStop()
{
	if (_right)
	{
		RemoveCallback(Identifier::MoveRight);
		_right = false;
	}
}
bool MovementExecuting::IsMovingRight()
{
	if (_right)
		return true;
	return false;
}

void MovementExecuting::MoveLeftStart()
{
	if (!_left)
	{
		actions.push_back(&*moveLeft);
		_left = true;
	}
}
void MovementExecuting::MoveLeftStop()
{
	if (_left)
	{
		RemoveCallback(Identifier::MoveLeft);
		_left = false;
	}
}
bool MovementExecuting::IsMovingLeft()
{
	if (_left)
		return true;
	return false;
}

void MovementExecuting::JumpStart()
{
	if (!_jump)
	{
		actions.push_back(&*jump);
		_jump = true;
	}
}
void MovementExecuting::JumpStop()
{
	if (_jump)
	{
		RemoveCallback(Identifier::Jump);
		_jump = false;
	}
}
bool MovementExecuting::IsJumping()
{
	if (_jump)
		return true;
	return false;
}

void MovementExecuting::RemoveCallback(Identifier id)
{
	size_t size = actions.size();
	for (int i = 0; i < size; ++i)
	{
		if (actions[i]->identifier == (int)id)
			actions.erase(actions.begin() + i); // remove action from vector
	}
}

void MovementExecuting::MoveRight()
{
	bot->SetRightMove(true);
}
void MovementExecuting::MoveLeft()
{
	bot->SetLeftMove(true);
}
void MovementExecuting::Jump()
{
	bot->SetJump(true);
}