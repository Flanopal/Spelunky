#include "stdafx.h"
#include "ActionFactories.h"

unique_ptr<ActionHandler> MoveToActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->SideMoveAt(finalCoords.x);
}

unique_ptr<ActionHandler> JumpActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->JumpTo(finalCoords);
}

unique_ptr<ActionHandler> ClimbToActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->ClimbToLevel(finalCoords.y);
}