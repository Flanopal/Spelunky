#pragma once
#include <vector>
#include <memory>

class PlayerActions;

#include "FrameworkLibrary.h"
#include "IBotAPI.h"
#include "DataStructures.h"
#include "Movements.h"
#include "ToolsActions.h"
#include "ActionClasses.h"

using namespace std;

class PlayerActions
{
public:
	PlayerActions(FrameworkLibrary* lib, IBotAPI* bot);

	unique_ptr<Movements> movements;
	unique_ptr<ToolsActions> toolsActions;

	unique_ptr<ActionHandler> ExecutePath(vector<unique_ptr<ActionHandlerFactory>> path);
	unique_ptr<ActionHandler> GoToNodeAction(Coordinates target);

private:
	FrameworkLibrary* lib;
	IBotAPI* bot;
};