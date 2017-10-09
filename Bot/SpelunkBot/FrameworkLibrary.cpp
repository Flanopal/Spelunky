#include "stdafx.h"
#include "FrameworkLibrary.h"

FrameworkLibrary::FrameworkLibrary(IBotAPI* bot)
{
	this->bot = bot;
	//this->enemyControl = make_unique<EnemyControl>(bot,this);
	this->playerActions = make_unique<PlayerActions>(bot,this);
}

bool FrameworkLibrary::Update(int* error)
{
	playerActions->movements->Update();
	return true;
}