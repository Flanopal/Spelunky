#include <memory>

#pragma once

#include "EnemyControl.h"

using namespace std;

class FrameworkLibrary
{
public:
	FrameworkLibrary(IBot* bot) 
	{
		this->bot = bot;
		this->enemyControl = make_unique<EnemyControl>(bot);
	}
	unique_ptr<EnemyControl> enemyControl;
private:
	IBot* bot;
};
