#pragma once
#include <vector>
#include <memory>
#include <iostream>

class FrameworkLibrary;

#include "IBotAPI.h"
#include "DataStructures.h"
#include "EnemyControl.h"
#include "PlayerActions.h"

using namespace std;

class FrameworkLibrary
{
public:
	FrameworkLibrary(IBotAPI* bot);
	bool Update(int* error);

	//unique_ptr<EnemyControl> enemyControl;
	unique_ptr<PlayerActions> playerActions;
private:	
	IBotAPI* bot;
};
