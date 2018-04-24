#include "stdafx.h"
#include "ToolsActions.h"

void ToolsActions::Attack()
{
	bot->SetAttack(true);
}

bool ToolsActions::SetRope()
{
	if (bot->GetRopeCount() == 0) return false;
	bot->SetRope();
	return true;
}