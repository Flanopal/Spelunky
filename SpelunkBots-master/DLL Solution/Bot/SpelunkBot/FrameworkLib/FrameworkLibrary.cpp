#include "stdafx.h"
#include "FrameworkLibrary.h"

FrameworkLibrary::FrameworkLibrary(IBotAPI* bot):previousPossition(0,0)
{
	this->bot = bot;
	this->mapControl = make_unique<MapControl>(this,bot);
	this->pathSearch = make_unique<PathSearch>(*mapControl,bot);
	this->playerActions = make_unique<PlayerActions>(this,bot);
}

bool FrameworkLibrary::Update()
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