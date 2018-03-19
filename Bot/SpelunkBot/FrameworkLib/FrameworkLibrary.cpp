#include "stdafx.h"
#include "FrameworkLibrary.h"

FrameworkLibrary::FrameworkLibrary(IBotAPI* bot):previousPossition(0,0)
{
	this->bot = bot;
	//this->enemyControl = make_unique<EnemyControl>(bot,this);
	this->mapControl = make_unique<MapControl>(previousPossition,bot);
	this->pathSearch = make_unique<PathSearch>(*mapControl);
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