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
unique_ptr<ActionHandler> PlayerActions::GoToNodeAction(Coordinates target)
{
	return make_unique<ActionClasses::GoToNode>(lib, bot, target);
}