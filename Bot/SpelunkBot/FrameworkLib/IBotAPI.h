#pragma once
#include "IBot.h"

class IBotAPI :public IBot
{
public:
	virtual void SetRightMove(bool value);
	virtual void SetLeftMove(bool value);

	virtual void SetJump(bool value);
	virtual void SetAttack(bool value);

	virtual void SetDuck(bool value);
	virtual void SetLookUp(bool value);
};