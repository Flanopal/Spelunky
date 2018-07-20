#pragma once
#include <functional>

using namespace std;

class ToolsActClasses
{
public:
	class SetRopeInDirection;
};

#include "DataStructures.h"
#include "FrameworkLibrary.h"

class ToolsActClasses::SetRopeInDirection :public ActionHandler
{
public:
	SetRopeInDirection(FrameworkLibrary* lib, IBotAPI* bot, LeaveDirection dir) :lib(lib), bot(bot), dir(dir) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~SetRopeInDirection() {}
private:
	void MyCallback(bool stopped);
	bool StartCrouching(); // for left and right direction is crouching needed
	bool PlaceRope();
	bool atFinalPossition = false; // at right possition for releasing
	int crouchTicks = 10; // how long will crouching last
	int ticksDone = 0; // in the middle of crouching must release the rope
	unique_ptr<ActionHandler> action;
	LeaveDirection dir; // to which direction will be roped placed
	IBotAPI* bot;
	FrameworkLibrary* lib;
};