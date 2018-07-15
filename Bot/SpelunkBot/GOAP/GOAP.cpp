#include "stdafx.h"
#include "GOAP.h"

GOAP::GOAP(FrameworkLibrary* lib) :lib(lib) {}
bool GOAP::ReachGoal(Coordinates target)
{
	return true;
}
bool GOAP::IsGoapRunning()
{
	return currentActionIndex >= 0;
}
bool GOAP::IsGoalReached()
{
	return true;
}
void GOAP::StopReaching()
{
	if (currentActionIndex < 0) return;
	actions[currentActionIndex]->StopAction();
	currentActionIndex = -1;
}
void GOAP::AddAction(unique_ptr<IGOAPAction> action)
{
	StopReaching();
	currentActionIndex = GetActionIndex(action->GetActionID());
	if (currentActionIndex < 0) actions.push_back(move(action));
}
void GOAP::RemoveAction(GOAPActionID ID)
{
	StopReaching();
	currentActionIndex = GetActionIndex(ID);
	if (currentActionIndex < 0) return;
	actions.erase(actions.begin() + currentActionIndex);
}
int GOAP::GetActionIndex(GOAPActionID ID)
{
	size_t size = actions.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (actions[i]->GetActionID() == ID) return i;
	}
	return -1;
}
