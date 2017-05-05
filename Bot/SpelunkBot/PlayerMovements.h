#pragma once

#include "DataStructures.h"
#include "IBot.h"

class PlayerMovements
{
public:
	PlayerMovements(IBot* bot):bot(bot){}
	void MoveToNode(int nodeX, int nodeY);
private:
	IBot* bot;
};
