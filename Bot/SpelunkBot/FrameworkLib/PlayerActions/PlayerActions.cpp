#include "stdafx.h"
#include "PlayerActions.h"

PlayerActions::PlayerActions(FrameworkLibrary* lib, IBotAPI* bot) :bot(bot), lib(lib)
{
	movements = make_unique<Movements>(bot, lib);
	toolsActions = make_unique<ToolsActions>(bot, lib);
}
unique_ptr<ActionHandler> PlayerActions::ExecutePath(vector<unique_ptr<ActionHandlerFactory>> path)
{
	return make_unique<ActionClasses::ExecutePath>(lib, bot, move(path));
}
unique_ptr<ActionClasses::GoDirectlyToNode> PlayerActions::GoDirectlyToNode(Coordinates target)
{
	return make_unique<ActionClasses::GoDirectlyToNode>(lib, bot, target);
}
unique_ptr<ActionClasses::GoDirectlyToNode> PlayerActions::GoDirectlyToNode(Coordinates target, int lifes, int ropes)
{
	return make_unique<ActionClasses::GoDirectlyToNode>(lib, bot, target,lifes, ropes);
}
unique_ptr<ActionClasses::ExploreToNode> PlayerActions::ExploreToNode(Coordinates target)
{
	return make_unique<ActionClasses::ExploreToNode>(lib, bot, target);
}
unique_ptr<ActionClasses::ExploreToNode> PlayerActions::ExploreToNode(Coordinates target, int lifes, int ropes)
{
	return make_unique<ActionClasses::ExploreToNode>(lib, bot, target, lifes, ropes);
}