#include "stdafx.h"
#include "ToolsActionsClasses.h"

// ----- SET ROPE IN DIRECTION ------
bool ToolsActClasses::SetRopeInDirection::Start()
{
	cout << ">> SET ROPE IN DIRECTION" << endl;
	if (bot->GetRopeCount() <= 0) return false;
	switch (dir)
	{
	case LeaveDirection::stay: return PlaceRope();
	case LeaveDirection::left: action = lib->playerActions->movements->MoveLeftFor(1); break; // heading correction
	case LeaveDirection::right: action = lib->playerActions->movements->MoveRightFor(1); break; // heading correction
	default: return false;
	}
	if (!action->Start())
	{
		state = ActionState::terminated;
		return false;
	}
	// horizontal move will run -> set callback
	action->registrCallback(make_unique<function<void(bool)>>(bind(&SetRopeInDirection::MyCallback, this, placeholders::_1)));
	state = ActionState::runnig;
	return true;
}
void ToolsActClasses::SetRopeInDirection::Stop()
{
	if (!StopPrepare()) return;
	action->Stop();
}
void ToolsActClasses::SetRopeInDirection::MyCallback(bool stopped)
{
	if (StopControl(stopped)) return;

	if (action->GetState() == ActionState::finished && !atFinalPossition) // heading correction finished
	{
		StartCrouching();
	}
	if (atFinalPossition) // good heading and crouching
	{
		++ticksDone;
		if (ticksDone == crouchTicks / 2)
			PlaceRope(); // in the middle of crouching -> place rope
		else if(action->GetState() == ActionState::finished) // placing finished
			state = ActionState::finished;
	}
	callParentCallback(stopped);
}
bool ToolsActClasses::SetRopeInDirection::StartCrouching()
{
	atFinalPossition = true;
	action = lib->playerActions->movements->CrouchFor(crouchTicks);
	if (!action->Start())
	{
		state = ActionState::terminated;
		return false;
	}
	action->registrCallback(make_unique<function<void(bool)>>(bind(&SetRopeInDirection::MyCallback, this, placeholders::_1)));
	state = ActionState::runnig;
	return true;
}
bool ToolsActClasses::SetRopeInDirection::PlaceRope()
{
	if (lib->playerActions->toolsActions->SetRope())
	{
		lib->mapControl->EmplaceRope(dir);
		state = ActionState::finished;
		return true;
	}
	state = ActionState::terminated;
	return false;
}
