#include "stdafx.h"
#include "FrameworkLibrary.h"

FrameworkLibrary::FrameworkLibrary(IBotAPI* bot):previousPossition(0,0)
{
	this->bot = bot;
	//this->enemyControl = make_unique<EnemyControl>(bot,this);
	this->mapControl = make_unique<MapControl>(this,bot);
	this->playerActions = make_unique<PlayerActions>(this,bot);
}

bool FrameworkLibrary::Update(int* error)
{
	mapControl->Update();
	playerActions->movements->Update();
	UpdatePrevisousPossition();
	return true;
}

void FrameworkLibrary::UpdatePrevisousPossition()
{
	previousPossition.x = bot->GetPlayerPositionXNode();
	previousPossition.y = bot->GetPlayerPositionYNode();
}