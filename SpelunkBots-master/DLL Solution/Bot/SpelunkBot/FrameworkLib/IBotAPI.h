#pragma once
#include "IBot.h"
#include "DataStructures.h"

class IBotAPI :public IBot
{
public:
	virtual void SetRightMove(bool value);
	virtual void SetLeftMove(bool value);

	virtual void SetJump(bool value);
	virtual void SetAttack(bool value);

	virtual void SetDuck(bool value);
	virtual void SetLookUp(bool value);

	virtual void SetStartClimbing();
	virtual void SetStopClimbing();

	virtual void SetRope();

	bool IsClimbing();
	int GetLifeCount();
	int GetRopeCount();

	virtual Coordinates GetPlayerCoordinates();

protected:
	bool isClimbing = false;
	int lifeCount = 4;
	int ropeCount = 4;
};