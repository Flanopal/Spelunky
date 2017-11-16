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
	return make_unique<BaseMVClasses::SideMoveAt>(&*movementWrapper,bot,coordX);
}
unique_ptr<ActionHandler> Movements::JumpTo(Coordinates coords)
{
	return make_unique<BaseMVClasses::JumpToSpot>(lib, bot, coords);
}
unique_ptr<ActionHandler> Movements::ClimbToLevel(double lvl)
{
	return make_unique<BaseMVClasses::ClimbToNodeLevel>(lib, bot, &*movementWrapper,lvl);
}
void Movements::Jump()
{
	movementWrapper->Jump(3);
}