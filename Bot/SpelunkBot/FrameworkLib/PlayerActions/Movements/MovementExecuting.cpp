#include "stdafx.h"
#include "MovementExecuting.h"

void MovementExecuting::Update()
{
	size_t size = actions.size();
	for (size_t i = 0; i < size; ++i)
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
	return _right;
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
	return _left;
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
	return _jump;
}

void MovementExecuting::LookUpStart()
{
	if (!_up)
	{
		actions.push_back(&*lookUp);
		_up = true;
	}
}
void MovementExecuting::LookUpStop()
{
	if (_up)
	{
		RemoveCallback(Identifier::Up);
		bot->SetLookUp(false);
		_up = false;
	}
}
bool MovementExecuting::IsLookingUp()
{
	return _up;
}

void MovementExecuting::CrouchStart()
{
	if (!_crouch)
	{
		actions.push_back(&*crouch);
		_crouch = true;
	}
}
void MovementExecuting::CrouchStop()
{
	if (_crouch)
	{
		RemoveCallback(Identifier::Crouch);
		bot->SetDuck(false);
		_crouch = false;
	}
}
bool MovementExecuting::IsCrouching()
{
	return _crouch;
}

void MovementExecuting::RemoveCallback(Identifier id)
{
	size_t size = actions.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (actions[i]->identifier == (int)id)
		{
			actions.erase(actions.begin() + i); // remove action from vector
			break;
		}
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
void MovementExecuting::Crouch()
{
	bot->SetDuck(true);
}
void MovementExecuting::LookUp()
{
	bot->SetLookUp(true);
}