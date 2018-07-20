#pragma once
#include <functional>
#include <memory>
#include <set>

using namespace std;

class ActionClasses
{
public:
	class PickRopeOrOpenCrate;
	class ExecutePath;
	class GoDirectlyToNode;
	class ExploreToNode;
};

#include "DataStructures.h"
#include "MovementExecutingWrapper.h"
#include "FrameworkLibrary.h"

class ActionClasses::PickRopeOrOpenCrate :public ActionHandler
{
public:
	PickRopeOrOpenCrate(FrameworkLibrary* lib, IBotAPI* bot):lib(lib),bot(bot)
	{}
	virtual bool Start();
	virtual void Stop();
	virtual ActionState GetState();
	virtual ~PickRopeOrOpenCrate() {}
private:
	void MyCallback(bool stopped);
	int crateTimer = -1; // crate will be opened after some time of heading up
	FrameworkLibrary* lib;
	IBotAPI* bot;
	unique_ptr<ActionHandler> action;
};

class ActionClasses::ExecutePath :public ActionHandler
{
public:
	ExecutePath(FrameworkLibrary* lib, IBotAPI* bot, vector<unique_ptr<ActionHandlerFactory>> path);
	virtual bool Start();
	virtual void Stop();
	virtual ~ExecutePath() {}
private:
	void MyCallback(bool stopped);
	bool ExecuteNextAction(); // next action in the path
	bool CoordControl(); // control coordinates for checking right possition on the path
	int currentActionIndex;
	unique_ptr<ActionHandler> currentAction;

	FrameworkLibrary* lib;
	IBotAPI* bot;
	vector<unique_ptr<ActionHandlerFactory>> path;
};


class ActionClasses::GoDirectlyToNode :public ActionHandler
{
public:
	GoDirectlyToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target):lib(lib),bot(bot), target(target) {}
	GoDirectlyToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target, int life, int rope) :lib(lib), bot(bot), target(target),life(life),rope(rope) {}
	
	/// plan path to reach target
	bool PlanPath();
	int FinalRopeCount();
	int FinalLifeCount();
	virtual bool Start();
	virtual void Stop();
	virtual ~GoDirectlyToNode() {}
private:
	void MyCallback(bool stopped);

	/// decides if the real final coords are near of desired coords
	bool NearbyCoords(Coordinates first, Coordinates second);

	int life = -1;
	int rope;
	unique_ptr<ActionHandler> pathExecutor;
	Coordinates target;
	IBotAPI* bot;
	FrameworkLibrary* lib;
};

class ActionClasses::ExploreToNode : public ActionHandler
{
public:
	ExploreToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target) :lib(lib), bot(bot), target(target) {}
	ExploreToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target, int life, int rope) :lib(lib), bot(bot), target(target), life(life), rope(rope) {}
	bool PlanPath();
	int FinalRopeCount();
	int FinalLifeCount();
	virtual bool Start();
	virtual void Stop();
	virtual ~ExploreToNode() {}
private:
	void MyCallback(bool stopped);

	int life = -1;
	int rope;
	unique_ptr<ActionHandler> pathExecutor;
	Coordinates target;
	IBotAPI* bot;
	FrameworkLibrary* lib;
};