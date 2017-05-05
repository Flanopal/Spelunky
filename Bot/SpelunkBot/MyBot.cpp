#include "stdafx.h"
#include "MyBot.h"

void MyBot::Update()
{
	if (moveCount < 15)
	{
		_goRight = true;
		++moveCount;
	}
	else
	{
		_goLeft = true;
		++moveCount;
		if (moveCount == 20)
			moveCount = 10;
	}
	vector<unique_ptr<Coordinates>> k = lib->enemyControl->GetEnemiesInDistance(10);
	if (k.size() > 0)
		_jump = true;
}