#include "stdafx.h"
#include "ToolsActionsClasses.h"

bool ToolsActClasses::SetRope::Start()
{
	if (bot->GetRopeCount() == 0) return false;
	int x = (int)bot->GetPlayerPositionXNode();
	double finalX = x;
	if (dir == LeaveDirection::left) finalX -= 0.2;
	else if (dir == LeaveDirection::right) finalX += 1.2;
	else finalX += 0.5;
	
	horizontalMove = lib->playerActions->movements->SideMoveAt(finalX);
	horizontalMove->registrCallback(make_unique<function<void(bool)>>(bind(&SetRope::MyCallback, this, placeholders::_1)));
	return horizontalMove->Start();
}

void ToolsActClasses::SetRope::Stop()
{
	horizontalMove->Stop();
}

ActionState ToolsActClasses::SetRope::GetState()
{
	return horizontalMove->GetState();
}

void ToolsActClasses::SetRope::MyCallback(bool stopped)
{
	if (!stopped)
	{
		if (GetState() == ActionState::finished)
		{
			if (dir != LeaveDirection::stay)
			{
				bot->SetDuck(true);
			}
			bot->SetRope();
		}
	}
	callParentCallback(stopped);
}