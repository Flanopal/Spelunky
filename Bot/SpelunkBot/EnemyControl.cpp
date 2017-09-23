#include "stdafx.h"
/*#include "EnemyControl.h"

vector<unique_ptr<Coordinates>> EnemyControl::GetEnemiesInDistance(int distance)
{
	vector<unique_ptr<Coordinates>> enemies;
	int playerX = bot->GetPlayerPositionXNode();
	int playerY = bot->GetPlayerPositionYNode(); 
	int nodeValue;
	for (int i = 0; i < distance; ++i)
	{
		for (int j = playerX - i; j < playerX + i; ++j)
		{
			nodeValue = bot->GetIDOfEnemyInNode(0, j, playerY+i, 0);
			if (nodeValue > 0)
			{
				enemies.push_back(make_unique<Coordinates>(j, playerY + i, nodeValue));
			}
			nodeValue = bot->GetIDOfEnemyInNode(0, j, playerY - i, 0);
			if (nodeValue > 0)
			{
				enemies.push_back(make_unique<Coordinates>(j, playerY - i, nodeValue));
			}
		}
		for (int j = playerY - i + 1; j < playerY + i - 1; ++j)
		{
			nodeValue = bot->GetIDOfEnemyInNode(0, playerX+i, j, 0);
			if (nodeValue > 0)
			{
				enemies.push_back(make_unique<Coordinates>(playerX + i, j, nodeValue));
			}
			nodeValue = bot->GetIDOfEnemyInNode(0, playerX - i, j, 0);
			if (nodeValue > 0)
			{
				enemies.push_back(make_unique<Coordinates>(playerX - i, j, nodeValue));
			}
		}
	}
	return enemies;
}
Coordinates EnemyControl::EnemyInArea(int nodeX, int nodeY)
{
	Coordinates finalCoordinates;
	int nodeValue;
	nodeValue = bot->GetIDOfEnemyInNode(0, nodeX, nodeY, 0);
	return finalCoordinates;
}*/
