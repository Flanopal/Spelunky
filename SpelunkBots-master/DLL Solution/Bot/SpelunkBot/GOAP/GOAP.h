#pragma once

#include<vector>

class GOAP;

#include "FrameworkLibrary.h"
#include "GOAPActions.h"


class GOAP
{
public:
	GOAP(FrameworkLibrary* lib);
	///Update state of GOAP - his actions are updated automaticly via library
	void Update();
	bool ReachGoal(unique_ptr<IGOAPGoal> goal);
	bool IsGoapRunning();
	bool IsGoalReached();
	bool RunFailed();
	void StopReaching();
	///Add action, with GOAP can operate
	void AddAction(unique_ptr<IGOAPGoal> action);
	///Remove action, with GOAP can operate
	void RemoveAction(GOAPActionID ID);
private:
	///Take next executable action from "goals" and runs it
	void StartNextAction();
	int currentActionIndex = -1;
	///Get index of action for operating in "actions"
	int GetActionIndex(GOAPActionID);
	vector<unique_ptr<IGOAPGoal>> goals;
	
	//Default actions for GOAL reaching
	vector<GOAPActionID> actions = 
	{ 
		GOAPActionID::GoToDirectly, 
		GOAPActionID::ExploreTo, 
		GOAPActionID::GoToNode 
	};
	FrameworkLibrary* lib;
};