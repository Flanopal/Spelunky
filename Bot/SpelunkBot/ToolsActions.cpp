#include "stdafx.h"
#include "ToolsActions.h"

void ToolsActions::AttackStart()
{
	if (!_attack)
	{
		actions.push_back(&*attack);
		_attack = true;
	}
}
void ToolsActions::AttackStop()
{
	if (_attack)
	{
		RemoveCallback(Identifier::Attack);
		_attack = false;
	}
}
void ToolsActions::Attack()
{
	bot->SetAttack(true);
}

void ToolsActions::RemoveCallback(Identifier id)
{
	size_t size = actions.size();
	for (int i = 0; i < size; ++i)
	{
		if (actions[i]->identifier == (int)id)
			actions.erase(actions.begin() + i); // remove action from vector
	}
}