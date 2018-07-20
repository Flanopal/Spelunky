#include "stdafx.h"
#include "MyBot.h"

// BOT UPDATING - INTENDED FOR TESTING MAPS
void MyBot::Update()
{
	// update state of library and GOAP
	lib->Update();
	goap->Update();

	// testing code for aa.explore
	if (lib->mapControl->ExitIsFound() && !finish)
	{
		goap->ReachGoal(make_unique<Goals::GoToNode>(&*lib, lib->mapControl->GetExitPos(), -1, 0));
		finish = true;
		switched = true;
		
	}
	else
		if (!switched)
		{
			goap->ReachGoal(make_unique<Goals::GoToNode>(&*lib, Coordinates(38, 5),1, 0));
			switched = true;
		}
}

// ------ OLD TESTING AGENTS -------

/*Coordinates exit = lib->mapControl->GetExitPos();
Coordinates coords = GetPlayerCoordinates();
if (!switched && action->GetState() == ActionState::finished)
{
switched = true;
lib->pathSearch->FindPath(coords, exit);
action = lib->pathSearch->GetNextMilestone()->GetAction(&*lib);
action->Start();
}
else
if (action->GetState() == ActionState::finished)
{
if (cd > 3)
{
unique_ptr<ActionHandlerFactory> fact = lib->pathSearch->GetNextMilestone();
if (fact != nullptr)
{
action = fact->GetAction(&*lib);
if (action->Start()) cout << "Next action started" << endl;
else cout << "Next action failed to start" << endl;
}
else
{
if (!(exit.EqualCoordinates(coords))) switched = false;
}
cd = 0;
}
else
++cd;
}*/

/*
if (action->GetState() == ActionState::finished && count == -1)
{
lib->mapControl->CoutFrame();
lib->mapControl->CoutMap();
count++;
}*/

/*
if (climbing) cout << GetIsHanging() << endl;
if (count > -1)
{
if (count > 0)
--count;
else
{
if (action == nullptr)
cout << "null \n";
if (!action->Start())
{
cout << "start failed\n";
count = 20;
}
count = -1;
}
return;
}
if (switched && action->GetState() == ActionState::runnig)
{
return;
}
Coordinates exit = lib->mapControl->GetExitPos();
if (exit.x == _playerPositionXNode && exit.y == _playerPositionYNode) return;
if (lib->mapControl->ExitIsFound() && action == nullptr)
{
cout << exit.x << "\n";
action = lib->playerActions->movements->SideMoveAt(exit.x+0.5);
action->Start();
cout << GetIsHanging() << endl;
}
else if (lib->mapControl->NodeIsClimable(_playerPositionXNode, _playerPositionYNode))
{
if (lib->mapControl->NodeIsClimable(_playerPositionXNode, _playerPositionYNode + 1.0))
{
if (!climbing)
{
action->Stop();
cout << "Start climbing\n";
action = lib->playerActions->movements->ClimbToLevel(10.0);
count = 10;
climbing = true;
return;
}
}
else if (lib->mapControl->NodeIsClimable(_playerPositionXNode, _playerPositionYNode - 1.0))
{
if (!climbing)
{
action->Stop();
cout << "Start climbing\n";
action = lib->playerActions->movements->ClimbToLevel(0);
count = 10;
climbing = true;
return;
}
}
}
else
{
double coef = 1;
if (lib->mapControl->GetExitPos().x < _playerPositionXNode)
coef = -1;
if (!switched && lib->mapControl->NodeIsTerrain(_playerPositionXNode + coef, _playerPositionYNode))
{
cout << "my y: " << _playerPositionYNode << "     my x: " << _playerPositionXNode << "\n";
cout << "Jump, " << _playerPositionXNode + coef << "  " << _playerPositionYNode << "\n";
action->Stop();
Coordinates coord(_playerPositionXNode + coef * 2, _playerPositionYNode - 1);
action = lib->playerActions->movements->JumpTo(coord);
action->Start();
switched = true;
}
}
if (climbing && action->GetState() != ActionState::runnig)
{
action->Stop();
cout << "Leaving ladder\n";
count = 10;
action = lib->playerActions->movements->LeaveClimable(LeaveDirection::right);
action->Start();
climbing = false;
return;
}
if (action->GetState() == ActionState::terminated || action->GetState() == ActionState::finished)
{
switched = false;
action = nullptr;
}*/

