#pragma once

#include <vector>
#include <memory>


enum class GOAPActionID { GoToDirectly = 0, ExploreTo, GoToNode,GetRope,ExploreCave,Preserve};

class IGOAPGoal;
class Goals
{
public:
	class GoToDirectly;
	class ExploreToTarget;
	class GoToNode;
	class GetRope;
	class ExploreCave;
};

#include "FrameworkLibrary.h"

class IGOAPGoal
{
public:
	/// Say if this action can be executed, or tranformed
	virtual bool ActionExecutable() = 0;
	/// Say if goal will fail
	virtual bool WillFail(vector<GOAPActionID> const providedGoals) = 0;
	virtual void ExecuteAction() {}
	virtual void StopAction() {}
	virtual ActionState GetState() { return ActionState::waiting; }
	virtual int GetLifes()=0; // not initialized until WillFail is called
	virtual int GetRopes()=0;// not initialized until WillFail is called
	virtual GOAPActionID GetActionID() = 0;
	/// Say if goal will be still in plan, after tranforming into subgoals(actions)
	virtual bool Preserve() { return false; }
	/// Transform this goal into subgoals, that leads to succeding it
	virtual vector<unique_ptr<IGOAPGoal>> TransformIntoSubGoals(vector<GOAPActionID> const providedGoals)
	{
		if (ActionExecutable()) return vector<unique_ptr<IGOAPGoal>>();
	}
	virtual ~IGOAPGoal() {}
protected:
	unique_ptr<IGOAPGoal> GetGoal(GOAPActionID ID, FrameworkLibrary* lib, Coordinates target, int life, int rope);
	unique_ptr<IGOAPGoal> GetGoal(GOAPActionID ID, FrameworkLibrary* lib, Coordinates target);
};

class Goals::GoToDirectly :public IGOAPGoal
{
public: 
	GoToDirectly(FrameworkLibrary* lib, Coordinates target, int life, int rope);
	GoToDirectly(FrameworkLibrary* lib, Coordinates target);
	virtual bool ActionExecutable() { return true; }
	virtual bool WillFail(vector<GOAPActionID> const providedGoals);
	virtual void ExecuteAction();
	virtual void StopAction();
	virtual ActionState GetState() { return action->GetState(); }
	virtual int GetLifes() { return life; } 
	virtual int GetRopes() { return rope; }
	virtual GOAPActionID GetActionID() { return GOAPActionID::GoToDirectly; }
	virtual ~GoToDirectly() {}
private:
	int life = -1;
	int rope = -1;
	unique_ptr<ActionClasses::GoDirectlyToNode> action;
};

class Goals::ExploreToTarget :public IGOAPGoal
{
public:
	ExploreToTarget(FrameworkLibrary* lib, Coordinates target, int life, int rope);
	ExploreToTarget(FrameworkLibrary* lib, Coordinates target);
	virtual bool ActionExecutable() { return true; }
	virtual bool WillFail(vector<GOAPActionID> const providedGoals);
	virtual void ExecuteAction();
	virtual void StopAction();
	virtual ActionState GetState() { return action->GetState(); }
	virtual int GetLifes() { return life; }
	virtual int GetRopes() { return rope; }
	virtual GOAPActionID GetActionID() { return GOAPActionID::ExploreTo; }
	virtual ~ExploreToTarget() {}
private:
	int life = -1;
	int rope = -1;
	unique_ptr<ActionClasses::ExploreToNode> action;
	FrameworkLibrary* lib;
};

class Goals::GoToNode :public IGOAPGoal
{
public:
	GoToNode(FrameworkLibrary* lib, Coordinates target, int life, int rope);
	GoToNode(FrameworkLibrary* lib, Coordinates target);
	virtual bool ActionExecutable() { return false; }
	virtual bool WillFail(vector<GOAPActionID> const providedGoals);
	virtual int GetLifes() { return life; }
	virtual int GetRopes() { return rope; }
	virtual bool Preserve();
	virtual GOAPActionID GetActionID() { return GOAPActionID::GoToNode; }
	virtual vector<unique_ptr<IGOAPGoal>> TransformIntoSubGoals(vector<GOAPActionID> const providedGoals);
	virtual ~GoToNode() {}
private:

	// vectors describing tranformation of this goal
	vector<vector<GOAPActionID>> subGoals = {
		{GOAPActionID::GoToDirectly},
		{GOAPActionID::ExploreTo,GOAPActionID::Preserve},
		{GOAPActionID::GetRope,GOAPActionID::Preserve},
	};
	FrameworkLibrary * lib;
	Coordinates target;
	int life = -1;
	int rope = -1;
	bool transformed = false;
	bool preserve = true;
	vector<unique_ptr<IGOAPGoal>> transform;
	vector<Coordinates> visited;
};

class Goals::GetRope :public IGOAPGoal
{
public:
	GetRope(FrameworkLibrary* lib);
	virtual bool ActionExecutable() { return true; }
	virtual bool WillFail(vector<GOAPActionID> const providedGoals);
	virtual void ExecuteAction();
	virtual void StopAction();
	virtual ActionState GetState();
	virtual int GetLifes() { return life; }
	virtual int GetRopes() { return rope; }
	virtual GOAPActionID GetActionID() { return GOAPActionID::GetRope; }
	virtual ~GetRope() {}
private:
	/// Prepare list of action for succeding this goal
	void AssignAction();
	bool actionAssigned = false;
	int life = -1;
	int rope = -1;
	unique_ptr<ActionHandler> action;
	FrameworkLibrary* lib;
};

class Goals::ExploreCave :public IGOAPGoal
{
public:
	ExploreCave(FrameworkLibrary* lib, int life, int rope);
	ExploreCave(FrameworkLibrary* lib);
	virtual bool ActionExecutable() { return false; }
	virtual bool WillFail(vector<GOAPActionID> const providedGoals);
	virtual int GetLifes() { return life; }
	virtual int GetRopes() { return rope; }
	virtual bool Preserve();
	virtual GOAPActionID GetActionID() { return GOAPActionID::ExploreCave; }
	virtual vector<unique_ptr<IGOAPGoal>> TransformIntoSubGoals(vector<GOAPActionID> const providedGoals);
	virtual ~ExploreCave() {}
private:
	// Exploring is started into every corner
	Coordinates corners[4] = { Coordinates(5,5),Coordinates(38,5),Coordinates(38,28),Coordinates(5,28) };
	/// Set action to reach corner
	void SetGoToCorner(vector<GOAPActionID> const providedGoals);
	unique_ptr<IGOAPGoal> goToCorner;
	/// Return closest corner to current position
	int GetClosestCorner();
	int life = -1;
	int rope = -1;
	FrameworkLibrary* lib;
};