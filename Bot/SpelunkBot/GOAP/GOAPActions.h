#pragma once

#include <vector>
#include <memory>


enum class GOAPActionID { GoToDirectly = 0, ExploreTo, GoToNode,GetRope,ExploreCave };

class IGOAPAction;
class GoToDirectly;
class ExploreToTarget;
class GoToNode;
class GetRope;
class ExploreCave;

#include "FrameworkLibrary.h"

class IGOAPAction
{
public:
	virtual bool ActionExecutable() = 0;
	virtual bool WillFail() = 0;
	virtual void ExecuteAction() = 0;
	virtual void StopAction() = 0;
	virtual GOAPActionID GetActionID() = 0;
	virtual vector<unique_ptr<IGOAPAction>> TransformIntoSubGoals(vector<GOAPActionID> const providedGoals, unique_ptr<IGOAPAction> self)
	{
		if (ActionExecutable()) return vector<unique_ptr<IGOAPAction>>();
	}
	virtual ~IGOAPAction() {}
};

class GoToDirectly :public IGOAPAction
{
public: 
	GoToDirectly(FrameworkLibrary* lib, Coordinates target, int life, int rope);
	GoToDirectly(FrameworkLibrary* lib, Coordinates target);
	virtual bool ActionExecutable() { return true; }
	virtual bool WillFail();
	virtual void ExecuteAction();
	virtual void StopAction();
	virtual GOAPActionID GetActionID() { return GOAPActionID::GoToDirectly; }
	virtual ~GoToDirectly() {}
private:
	unique_ptr<ActionClasses::GoDirectlyToNode> action;
};

class ExploreToTarget :public IGOAPAction
{
public:
	ExploreToTarget(FrameworkLibrary* lib, Coordinates target, int life, int rope);
	ExploreToTarget(FrameworkLibrary* lib, Coordinates target);
	virtual bool ActionExecutable() { return true; }
	virtual bool WillFail();
	virtual void ExecuteAction();
	virtual void StopAction();
	virtual GOAPActionID GetActionID() { return GOAPActionID::ExploreTo; }
	virtual ~ExploreToTarget() {}
private:
	unique_ptr<ActionClasses::ExploreToNode> action;
	FrameworkLibrary* lib;
};
/*
class GoToNode :public IGOAPAction
{
public:
	GoToNode(FrameworkLibrary* lib);
	virtual bool ActionExecutable() { return false; }
	virtual bool WillFail();
	virtual void ExecuteAction();
	virtual void StopAction();
	virtual GOAPActionID GetActionID() { return GOAPActionID::GoToNode; }
	virtual vector<unique_ptr<IGOAPAction>> TransformIntoSubGoals(vector<GOAPActionID>& const providedGoals, unique_ptr<IGOAPAction> self);
	virtual ~GoToNode() {}
};

class GetRope :public IGOAPAction
{
public:
	GetRope(FrameworkLibrary* lib);
	virtual bool ActionExecutable() { return true; }
	virtual bool WillFail();
	virtual void ExecuteAction();
	virtual void StopAction();
	virtual GOAPActionID GetActionID() { return GOAPActionID::GetRope; }
	virtual ~GetRope() {}
};

class ExploreCave :public IGOAPAction
{
public:
	ExploreCave(FrameworkLibrary* lib);
	virtual bool ActionExecutable() { return false; }
	virtual bool WillFail();
	virtual void ExecuteAction();
	virtual void StopAction();
	virtual GOAPActionID GetActionID() { return GOAPActionID::ExploreCave; }
	virtual vector<unique_ptr<IGOAPAction>> TransformIntoSubGoals(vector<GOAPActionID>& const providedGoals, unique_ptr<IGOAPAction> self);
	virtual ~ExploreCave() {}
};*/