#include "stdafx.h"
#include "MyBot.h"

void MyBot::Update()
{
	int k = 0;
	lib->Update(&k);
	if (action->GetState() == ActionState::finished && count == -1)
	{
		lib->mapControl->CoutFrame();
		lib->mapControl->CoutMap();
		count++;
	}
	/*if (count > -1)
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
}
