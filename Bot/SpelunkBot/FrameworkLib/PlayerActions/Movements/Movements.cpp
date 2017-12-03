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
void Movements::Jump()
{
	movementWrapper->Jump(3);
}