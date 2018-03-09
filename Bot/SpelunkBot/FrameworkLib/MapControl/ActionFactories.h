#pragma once

#include "FrameworkLibrary.h"

using namespace std;

class MoveToActionFactory :public ActionHandlerFactory
{
public:
	MoveToActionFactory(double finalX):finalX(finalX) { }
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib);
private:
	double finalX;
};