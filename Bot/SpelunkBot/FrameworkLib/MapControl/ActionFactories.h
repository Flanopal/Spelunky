#pragma once

class ActionListFactory;
class MoveToActionFactory;
class MoveForActionFactory;
class LongJumpActionFactory;
class ShortJumpActionFactory;
class JumpToActionFactory;
class GetOnClimbingActionFactory;
class ClimbToActionFactory;
class LeaveClimbingActionFactory;
class WaitActionFactory;
class SetRopeActionFactory;

#include "FrameworkLibrary.h"

using namespace std;

class ActionListFactory :public ActionHandlerFactory
{
public:
	ActionListFactory(unique_ptr<ActionHandlerFactory> factory)
		:ActionHandlerFactory(factory->GetFinalCoords()) 
	{ 
		factories.push_back(move(factory));
		actionDescription = "AL";
	}
	void AddActionFactory(unique_ptr<ActionHandlerFactory> factory);
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
private:
	vector<unique_ptr<ActionHandlerFactory>> factories;
};

class MoveToActionFactory :public ActionHandlerFactory
{
public:
	MoveToActionFactory(double finalX,double finalY) :ActionHandlerFactory(finalX,finalY) 
	{ actionDescription = "MF"; } //move from
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class MoveToAndLandActionFactory :public ActionHandlerFactory
{
public:
	MoveToAndLandActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "MF"; } //move from
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class MoveForActionFactory :public ActionHandlerFactory
{
public:
	MoveForActionFactory(double finalX, double finalY, int ticks) :ActionHandlerFactory(finalX, finalY),ticks(ticks)
	{ actionDescription = "FM"; } //for move
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
private:
	int ticks;
};

class LongJumpActionFactory :public ActionHandlerFactory
{
public:
	LongJumpActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "LJ"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class ShortJumpActionFactory :public ActionHandlerFactory
{
public:
	ShortJumpActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "SJ"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class JumpToActionFactory :public ActionHandlerFactory
{
public:
	JumpToActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{	actionDescription = "JT";	}
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class GetOnClimbingActionFactory :public ActionHandlerFactory
{
public:
	GetOnClimbingActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "GC"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class ClimbToActionFactory :public ActionHandlerFactory
{
public:
	ClimbToActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "CT"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class LeaveClimbingActionFactory :public ActionHandlerFactory
{
public:
	LeaveClimbingActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "LC"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class WaitActionFactory :public ActionHandlerFactory
{
public:
	WaitActionFactory(double finalX, double finalY,int waitTime) :ActionHandlerFactory(finalX, finalY),waitTime(waitTime)
	{ actionDescription = "WT"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
private:
	int waitTime;
};

class WaitForLandingActionFactory :public ActionHandlerFactory
{
public:
	WaitForLandingActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "WL"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class SetRopeActionFactory :public ActionHandlerFactory
{
public:
	SetRopeActionFactory(double finalX, double finalY, LeaveDirection dir) :dir(dir), ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "SR"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
private:
	LeaveDirection dir;
};