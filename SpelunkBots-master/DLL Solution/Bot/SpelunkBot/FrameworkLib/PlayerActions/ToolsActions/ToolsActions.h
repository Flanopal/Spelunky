#pragma once

#include <vector>
#include <memory>
#include <functional>

class ToolsActions;

#include "FrameworkLibrary.h"
#include "IBotAPI.h"
#include "DataStructures.h"
#include "ToolsActionsClasses.h"

using namespace std;

class ToolsActions
{
public:
	ToolsActions(IBotAPI* bot, FrameworkLibrary* parent) :bot(bot), lib(parent) {}
	// methods for immediate action
	void Attack();
	bool SetRope();

	// creates action object with action
	unique_ptr<ActionHandler> SetRopeInDirection(LeaveDirection dir);
private:
	FrameworkLibrary* lib;
	IBotAPI* bot;
};
