#pragma once
#include <functional>

using namespace std;

class ToolsActClasses
{
public:
	class SetRope;
};

#include "DataStructures.h"
#include "FrameworkLibrary.h"

class ToolsActClasses::SetRope :public ActionHandler
{
public:
	SetRope(FrameworkLibrary* lib, IBotAPI* bot, LeaveDirection dir) :lib(lib),bot(bot),dir(dir) {}
	virtual bool Start();
	virtual void Stop();
	virtual ActionState GetState();
	virtual ~SetRope() {}
private:
	void MyCallback(bool stopped);
	unique_ptr<ActionHandler> horizontalMove;
	LeaveDirection dir;
	FrameworkLibrary* lib;
	IBotAPI* bot;
};