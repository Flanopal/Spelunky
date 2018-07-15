#pragma once

#include<vector>

class GOAP;

#include "FrameworkLibrary.h"
#include "GOAPActions.h"


class GOAP
{
public:
	GOAP(FrameworkLibrary* lib);
	bool ReachGoal(Coordinates target);
	bool IsGoapRunning();
	bool IsGoalReached();
	void StopReaching();
	void AddAction(unique_ptr<IGOAPAction> action);
	void RemoveAction(GOAPActionID ID);
private:
	int currentActionIndex = -1;
	int GetActionIndex(GOAPActionID);
	vector<unique_ptr<IGOAPAction>> actions;
	FrameworkLibrary* lib;
};