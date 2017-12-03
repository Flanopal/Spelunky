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
	PlayerActions(FrameworkLibrary* lib, IBotAPI* bot):bot(bot),lib(lib)
	{
		movements = make_unique<Movements>(bot, lib);
		toolsActions = make_unique<ToolsActions>(bot, lib);
	}
	unique_ptr<Movements> movements;
	unique_ptr<ToolsActions> toolsActions;

private:
	FrameworkLibrary* lib;
	IBotAPI* bot;
};