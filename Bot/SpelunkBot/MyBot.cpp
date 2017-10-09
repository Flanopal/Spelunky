#include "stdafx.h"
#include "MyBot.h"

void MyBot::Update()
{
	int j = 0;
	lib->Update(&j);
	if (k < -20)
	{
		lib->playerActions->movements->StartMovingLeft(make_unique<function<bool()>>(bind(&MyBot::right, this)));
		lib->playerActions->movements->Jump();
	}
	else if (k > 20)
	{
		lib->playerActions->movements->StartMovingRight(make_unique<function<bool()>>(bind(&MyBot::left, this)));
		lib->playerActions->movements->Jump();
	}
	
}