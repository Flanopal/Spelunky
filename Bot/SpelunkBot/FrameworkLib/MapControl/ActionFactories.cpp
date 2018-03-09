#include "stdafx.h"
#include "ActionFactories.h"

unique_ptr<ActionHandler> MoveToActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->SideMoveAt(finalX);
}