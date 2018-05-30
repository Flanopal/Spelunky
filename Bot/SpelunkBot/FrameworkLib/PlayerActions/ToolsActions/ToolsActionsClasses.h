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
	bool StartCrouching();
	bool PlaceRope();
	bool atFinalPossition = false;
	unique_ptr<ActionHandler> action;
	LeaveDirection dir;
	IBotAPI* bot;
	FrameworkLibrary* lib;
};