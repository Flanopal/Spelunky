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
unique_ptr<ActionHandler> Movements::MoveRightFor(int distance)
{
	return make_unique<BaseMVClasses::MoveFor>(&*movementWrapper, distance, false);
}
unique_ptr<ActionHandler> Movements::SideMoveAt(double coordX)
{
	return make_unique<BaseMVClasses::SideMoveAt>(lib, bot, &*movementWrapper, coordX);
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
unique_ptr<ActionHandler> Movements::ClimbToLevel(double lvl)
{
	return make_unique<BaseMVClasses::ClimbToNodeLevel>(lib, bot, &*movementWrapper,lvl);
}
unique_ptr<ActionHandler> Movements::LeaveClimable(LeaveDirection direction)
{
	return make_unique<BaseMVClasses::LeaveClimbing>(lib, bot, &*movementWrapper, direction);
}
unique_ptr<ActionHandler> Movements::Wait(int time)
{
	return make_unique <BaseMVClasses::Wait>(&*movementWrapper, time);
}
unique_ptr<BaseMVClasses::ActionList> Movements::GetActionList()
{
	return make_unique<BaseMVClasses::ActionList>(lib);
}
void Movements::Jump(int ticks)
{
	movementWrapper->Jump(ticks);
}