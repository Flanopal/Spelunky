#include "stdafx.h"
#include "GOAP.h"

GOAP::GOAP(FrameworkLibrary* lib) :lib(lib)
{}
void GOAP::Update()
{
	if (currentActionIndex > 0 && goals[currentActionIndex]->GetState() == ActionState::finished)
	{
		// last action was finished - starting a new one
		goals.pop_back();
		StartNextAction();
	}
}
bool GOAP::ReachGoal(unique_ptr<IGOAPGoal> goal)
{
	StopReaching();
	goals.clear(); // clear goal stack
	goals.push_back(move(goal));
	currentActionIndex = 0;
	StartNextAction();
	return currentActionIndex >= 0;
}
bool GOAP::IsGoapRunning()
{
	return currentActionIndex >= 0;
}
bool GOAP::IsGoalReached()
{
	if (goals.size() > 0)
	{
		return goals[0]->GetState() == ActionState::finished;
	}
	return true; // none GOAL -> by default reached
}
void GOAP::StopReaching()
{
	if (currentActionIndex < 0) return;
	goals[currentActionIndex]->StopAction();
	currentActionIndex = -1;
}
bool GOAP::RunFailed()
{
	return currentActionIndex < 0 && goals.size()>0;
}
void GOAP::AddAction(unique_ptr<IGOAPGoal> action)
{
	StopReaching();
	currentActionIndex = GetActionIndex(action->GetActionID());
	if (currentActionIndex < 0) goals.push_back(move(action));
}
void GOAP::RemoveAction(GOAPActionID ID)
{
	StopReaching();
	currentActionIndex = GetActionIndex(ID);
	if (currentActionIndex < 0) return;
	goals.erase(goals.begin() + currentActionIndex);
}
void GOAP::StartNextAction()
{
	if (currentActionIndex < 0 && goals.size()>0) return;
	currentActionIndex = goals.size() - 1;
	if (currentActionIndex < 0) return;
	while (!goals[currentActionIndex]->ActionExecutable())
	{
		// action is not executable
		if (goals[currentActionIndex]->WillFail(actions))
		{
			cout << "GOAP action failed" << endl;
			currentActionIndex = -1; // GAOP failed
			return;
		}
		// tranform into subgoals
		auto subGoals = goals[currentActionIndex]->TransformIntoSubGoals(actions);
		if (!goals[currentActionIndex]->Preserve())
		{
			// actual goal will be poped -> not preserved
			goals.pop_back();
		}
		goals.insert(goals.end(), make_move_iterator(subGoals.begin()), make_move_iterator(subGoals.end()));
		currentActionIndex = goals.size() - 1;
	}
	if (goals[currentActionIndex]->WillFail(actions))
	{
		cout << "GOAP executable action failed" << endl;
		currentActionIndex = -1;
		return;
	}
	goals[currentActionIndex]->ExecuteAction();
}

int GOAP::GetActionIndex(GOAPActionID ID)
{
	size_t size = actions.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (goals[i]->GetActionID() == ID) return i;
	}
	return -1;
}
