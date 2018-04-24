#include "stdafx.h"
#include "Movements.h"

void Movements::Update()
{
	movementWrapper->Update();
}
unique_ptr<ActionHandler> Movements::MoveLeftFor(int distance)
{
	return make_unique<BaseMVClasses::MoveFor>(&*movementWrapper, distance, true);
}
unique_ptr<ActionHandler> Movements::LookUpFor(int ticks)
{
	return make_unique<BaseMVClasses::LookUpFor>(&*movementWrapper, ticks);
}
unique_ptr<ActionHandler> Movements::MoveRightFor(int distance)
{
	return make_unique<BaseMVClasses::MoveFor>(&*movementWrapper, distance, false);
}
unique_ptr<ActionHandler> Movements::SideMoveAt(double coordX)
{
	return make_unique<BaseMVClasses::SideMoveAt>(lib, bot, &*movementWrapper, coordX);
}
unique_ptr<ActionHandler> Movements::MoveAtAndLand(double finalX)
{
	unique_ptr<BaseMVClasses::ActionList> action = GetActionList();
	action->AddAction(SideMoveAt(finalX));
	action->AddAction(WaitForLanding());
	return move(action);
}
unique_ptr<ActionHandler> Movements::LongJump()
{
	return make_unique<BaseMVClasses::Jump>(lib,3);
}
unique_ptr<ActionHandler> Movements::ShortJump()
{
	return make_unique<BaseMVClasses::Jump>(lib,1);
}
unique_ptr<ActionHandler> Movements::JumpTo(Coordinates coords)
{
	return make_unique<BaseMVClasses::JumpToSpot>(lib, bot, coords);
}
unique_ptr<ActionHandler> Movements::GetOnClimbing()
{
	return make_unique<BaseMVClasses::GetOnClimbing>(lib, bot,&*movementWrapper);
}
unique_ptr<ActionHandler> Movements::ClimbToLevel(double lvl)
{
	return make_unique<BaseMVClasses::ClimbToNodeLevel>(lib, bot, &*movementWrapper,lvl);
}
unique_ptr<ActionHandler> Movements::LeaveClimable(LeaveDirection direction)
{
	return make_unique<BaseMVClasses::LeaveClimbing>(lib, bot, direction);
}
unique_ptr<ActionHandler> Movements::Wait(int time)
{
	return make_unique<BaseMVClasses::Wait>(&*movementWrapper, time);
}
unique_ptr<ActionHandler> Movements::WaitForLanding()
{
	return make_unique<BaseMVClasses::WaitForLanding>(bot, &*lib->mapControl, &*movementWrapper);
}
unique_ptr<BaseMVClasses::ActionList> Movements::GetActionList()
{
	return make_unique<BaseMVClasses::ActionList>(lib);
}
void Movements::Jump(int ticks)
{
	bot->SetStopClimbing();
	movementWrapper->Jump(ticks);
}