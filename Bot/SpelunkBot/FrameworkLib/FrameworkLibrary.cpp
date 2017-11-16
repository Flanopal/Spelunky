#include "stdafx.h"
#include "FrameworkLibrary.h"

FrameworkLibrary::FrameworkLibrary(IBotAPI* bot):previousPossition(0,0)
{
	this->bot = bot;
	//this->enemyControl = make_unique<EnemyControl>(bot,this);
	this->mapControl = make_unique<MapControl>(bot);
	this->playerActions = make_unique<PlayerActions>(bot,this);
}

bool FrameworkLibrary::Update(int* error)
{
	playerActions->movements->Update();
	UpdatePrevisousPossition();
	return true;
}

void FrameworkLibrary::UpdatePrevisousPossition()
{
	previousPossition.x = bot->GetPlayerPositionXNode();
	previousPossition.y = bot->GetPlayerPositionYNode();
}