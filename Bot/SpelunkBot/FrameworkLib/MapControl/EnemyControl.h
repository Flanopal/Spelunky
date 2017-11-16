/*#pragma once
#include <vector>
#include <memory>

class EnemyControl;

#include "FrameworkLibrary.h"
#include "IBotAPI.h"
#include "DataStructures.h"

using namespace std;

class EnemyControl
{
public:
	EnemyControl(IBot* bot, FrameworkLibrary* parent) :bot(bot), parentLib(parent) {}
	vector<unique_ptr<Coordinates>> GetEnemiesInDistance(int distance);
	Coordinates EnemyInArea(int nodeX, int nodeY); // if not -> Coordinates type == 0
private:
	FrameworkLibrary* parentLib;
	IBot* bot;
};*/
