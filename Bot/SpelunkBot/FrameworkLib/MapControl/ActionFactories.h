#pragma once

class MoveToActionFactory;
class JumpActionFactory;
class ClimbToActionFactory;
	
#include "FrameworkLibrary.h"

using namespace std;

class MoveToActionFactory :public ActionHandlerFactory
{
public:
	MoveToActionFactory(double finalX,double finalY) :ActionHandlerFactory(finalX,finalY) 
	{ actionDescription = "MF"; } //move from
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class JumpActionFactory :public ActionHandlerFactory
{
public:
	JumpActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{	actionDescription = "JT";	}
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};

class ClimbToActionFactory :public ActionHandlerFactory
{
public:
	ClimbToActionFactory(double finalX, double finalY) :ActionHandlerFactory(finalX, finalY)
	{ actionDescription = "CT"; }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
};