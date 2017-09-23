#include "stdafx.h"
#include "MyBot.h"

void MyBot::Update()
{
	int k = 0;
	lib->Update(&k);
	//_lookUp = true;
	//vector<unique_ptr<Coordinates>> k = lib->enemyControl->GetEnemiesInDistance(10);
	/*if (k.size() > 0)*/
	//	_jump = true;
}