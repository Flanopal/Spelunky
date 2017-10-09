#include "stdafx.h"
#include "IBotAPI.h"

void IBotAPI::SetRightMove(bool value)
{
	_goRight = value;
}
void IBotAPI::SetLeftMove(bool value)
{
	_goLeft = value;
}
void IBotAPI::SetJump(bool value)
{
	_jump = value;
}
void IBotAPI::SetAttack(bool value)
{
	_attack = value;
}