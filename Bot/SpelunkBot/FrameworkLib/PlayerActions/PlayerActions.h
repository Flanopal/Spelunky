#pragma once
#include <vector>
#include <memory>

class PlayerActions;

#include "FrameworkLibrary.h"
#include "IBotAPI.h"
#include "DataStructures.h"
#include "Movements.h"
#include "ToolsActions.h"

using namespace std;

class PlayerActions
{
public:
	PlayerActions(IBotAPI* bot,FrameworkLibrary* parent):bot(bot),parentLib(parent)
	{
		movements = make_unique<Movements>(bot, parentLib);
		toolsActions = make_unique<ToolsActions>(bot, parentLib);
	}
	unique_ptr<Movements> movements;
	unique_ptr<ToolsActions> toolsActions;

private:
	FrameworkLibrary* parentLib;
	IBotAPI* bot;
};