#pragma once

#include <vector>
#include <memory>
#include <functional>

class ToolsActions;

#include "FrameworkLibrary.h"
#include "IBotAPI.h"
#include "DataStructures.h"

using namespace std;

class ToolsActions
{
public:
	ToolsActions(IBotAPI* bot, FrameworkLibrary* parent) :bot(bot), parentLib(parent) {}
	void AttackStart();
	void AttackStop();

private:
	// used to indikate callback
	enum class Identifier { Attack=0 };
	vector<Command*> actions;

	void Attack();
	void RemoveCallback(Identifier id);

	// binded functions for faster adding into action vector
	unique_ptr<Command> attack = make_unique<Command>((int)Identifier::Attack, bind(&ToolsActions::Attack, this));

	bool _attack = false;

	FrameworkLibrary* parentLib;
	IBotAPI* bot;
};
