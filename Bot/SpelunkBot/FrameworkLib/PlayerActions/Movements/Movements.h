#pragma once
#include <vector>
#include <memory>

class Movements;

#include "DataStructures.h"
#include "IBotAPI.h"
#include "BaseMovementClasses.h"
#include "MovementExecutingWrapper.h"
#include "FrameworkLibrary.h"

using namespace std;

class Movements
{
public:
	Movements(IBotAPI* bot, FrameworkLibrary* parent) :bot(bot), lib(parent)
	{
		movementWrapper = make_unique<MovementExecutingWrapper>(bot);
	}
	void Update();
	unique_ptr<ActionHandler> MoveLeftFor(int distance);
	unique_ptr<ActionHandler> LookUpFor(int ticks);
	unique_ptr<ActionHandler> CrouchFor(int ticks);
	unique_ptr<ActionHandler> MoveRightFor(int distance);
	unique_ptr<ActionHandler> SideMoveAt(double coordX);
	unique_ptr<ActionHandler> MoveAtAndLand(double finalX);
	unique_ptr<ActionHandler> LongJump();
	unique_ptr<ActionHandler> ShortJump();
	unique_ptr<ActionHandler> JumpTo(Coordinates coords);
	unique_ptr<ActionHandler> GetOnClimbing();
	unique_ptr<ActionHandler> ClimbToLevel(double lvl);
	unique_ptr<ActionHandler> LeaveClimable(LeaveDirection direction);
	unique_ptr<ActionHandler> Wait(int time);
	unique_ptr<ActionHandler> WaitForLanding(double x);
	unique_ptr<BaseMVClasses::ActionList> GetActionList();
	void Jump(int ticks);
	// wrapper stop moving
private:
	unique_ptr<MovementExecutingWrapper> movementWrapper;
	FrameworkLibrary* lib;
	IBotAPI* bot;
};