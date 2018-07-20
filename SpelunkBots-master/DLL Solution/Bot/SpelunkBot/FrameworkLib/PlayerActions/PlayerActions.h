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

	//Action objects provided from this class

	/// pick rope or open crate at actual position, where is this action started
	unique_ptr<ActionHandler> PickRopeOrOpenCrate();
	unique_ptr<ActionHandler> ExecutePath(vector<unique_ptr<ActionHandlerFactory>> path);
	unique_ptr<ActionClasses::GoDirectlyToNode> GoDirectlyToNode(Coordinates target);
	unique_ptr<ActionClasses::GoDirectlyToNode> GoDirectlyToNode(Coordinates target, int lifes, int ropes);
	unique_ptr<ActionClasses::ExploreToNode> ExploreToNode(Coordinates target);
	unique_ptr<ActionClasses::ExploreToNode> ExploreToNode(Coordinates target, int lifes, int ropes);

private:
	FrameworkLibrary* lib;
	IBotAPI* bot;
};