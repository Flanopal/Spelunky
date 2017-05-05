#include <vector>
#include <memory>
#pragma once

#include "DataStructures.h"
#include "IBot.h"

using namespace std;

class EnemyControl
{
public:
	EnemyControl(IBot* bot) :bot(bot) {}
	vector<unique_ptr<Coordinates>> GetEnemiesInDistance(int distance);
	Coordinates EnemyInArea(int nodeX, int nodeY); // if not -> Coordinates type == 0
	void DestroyEnemy(int nodeX, int nodeY);
private:
	IBot* bot;
};
