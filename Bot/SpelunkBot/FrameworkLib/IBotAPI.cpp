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
void IBotAPI::SetDuck(bool value)
{
	_duck = value;
}
void IBotAPI::SetLookUp(bool value)
{
	_lookUp = value;
}