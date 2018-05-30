#include "stdafx.h"
#include "ActionFactories.h"

void ActionListFactory::AddActionFactory(unique_ptr<ActionHandlerFactory> factory)
{
	finalCoords = factory->GetFinalCoords();
	factories.push_back(move(factory));
}

unique_ptr<ActionHandler> ActionListFactory::GetAction(FrameworkLibrary* lib)
{
	unique_ptr<BaseMVClasses::ActionList> list = lib->playerActions->movements->GetActionList();
	size_t size = factories.size();
	if (size > 0)
	{
		for (size_t i = 0; i < size; ++i)
		{
			list->AddAction(move(factories[i]));
		}
	}
	cout << "Final x " << finalCoords.x << " final y " << finalCoords.y << endl;
	return list;
}

unique_ptr<ActionHandler> MoveToActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->SideMoveAt(finalCoords.x);
}

unique_ptr<ActionHandler> MoveToAndLandActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->MoveAtAndLand(finalCoords.x);
}

unique_ptr<ActionHandler> MoveForActionFactory::GetAction(FrameworkLibrary* lib)
{
	if (ticks > 0) return lib->playerActions->movements->MoveRightFor(ticks);
	else return lib->playerActions->movements->MoveLeftFor(-1*ticks);
}

unique_ptr<ActionHandler> LongJumpActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->LongJump();
}

unique_ptr<ActionHandler> ShortJumpActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->ShortJump();
}

unique_ptr<ActionHandler> JumpToActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->JumpTo(finalCoords);
}

unique_ptr<ActionHandler>GetOnClimbingActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->GetOnClimbing();
}

unique_ptr<ActionHandler> ClimbToActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->ClimbToLevel(finalCoords.y);
}

unique_ptr<ActionHandler> LeaveClimbingActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->LeaveClimable(LeaveDirection::stay);
}

unique_ptr<ActionHandler> WaitActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->Wait(waitTime);
}

unique_ptr<ActionHandler> WaitForLandingActionFactory::GetAction(FrameworkLibrary* lib)
{
	return lib->playerActions->movements->WaitForLanding(finalCoords.x);
}