#pragma once
#include <functional>
#include <memory>

using namespace std;

class BaseMVClasses
{
public:
	class MoveFor;
	class SideMoveAt;
	class Jump;
	class JumpToSpot;
	class ClimbToNodeLevel;
	class LeaveClimbing;
	class Wait;
	class ActionList;
};

#include "MovementExecutingWrapper.h"
#include "DataStructures.h"
#include "FrameworkLibrary.h"

class BaseMVClasses::MoveFor:public ActionHandler
{
public:
	MoveFor(MovementExecutingWrapper* wrapper, int dist, bool left) :wrapper(wrapper), dist(dist), left(left)
	{
	}
	virtual bool Start();
	virtual void Stop();
	virtual ~MoveFor() {}
private:
	void MyCallback(bool stopped);
	int dist;
	bool left;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::SideMoveAt :public ActionHandler
{
public:
	SideMoveAt(FrameworkLibrary* lib, IBotAPI* bot, MovementExecutingWrapper* wrapper, double coordX) : lib(lib),  bot(bot), wrapper(wrapper), coordX(coordX)
	{
		controler = make_unique<MovingController>(lib,bot,5);
	}
	virtual bool Start();
	virtual void Stop();
	virtual ~SideMoveAt() {}
private:
	void MyCallback(bool stopped);
	bool left;
	double coordX;
	unique_ptr<MovingController> controler;
	FrameworkLibrary* lib;
	MovementExecutingWrapper* wrapper;
	IBotAPI* bot;
};

class BaseMVClasses::Jump :public ActionHandler
{
public:
	Jump(FrameworkLibrary* lib,int ticks) :lib(lib),ticks(ticks) {}
	virtual bool Start();
	virtual ~Jump() {}
private:
	FrameworkLibrary* lib;
	int ticks;
};

class BaseMVClasses::JumpToSpot :public ActionHandler
{
public:
	JumpToSpot(FrameworkLibrary* lib,IBotAPI* bot, Coordinates target):lib(lib),bot(bot),target(target)
	{ }
	virtual bool Start();
	virtual void Stop();
	virtual ActionState GetState();
	virtual ~JumpToSpot() {}
private:
	void MyCallback(bool stopped);
	int dx;
	Coordinates target;
	unique_ptr<ActionHandler> horizontalMove;
	FrameworkLibrary* lib;
	IBotAPI* bot;
};

class BaseMVClasses::ClimbToNodeLevel :public ActionHandler
{
public:
	ClimbToNodeLevel(FrameworkLibrary* lib, IBotAPI* bot, MovementExecutingWrapper* wrapper,  double targetLvl)
		:lib(lib),bot(bot),wrapper(wrapper), targetLvl(targetLvl)
	{ }
	virtual bool Start();
	virtual void Stop();
	virtual ~ClimbToNodeLevel() {}
private:
	void MyCallback(bool stopped);
	double targetLvl;
	double targetX;
	bool up;
	FrameworkLibrary* lib;
	IBotAPI* bot;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::LeaveClimbing :public ActionHandler
{
public:
	LeaveClimbing(FrameworkLibrary* lib, IBotAPI* bot, MovementExecutingWrapper* wrapper,LeaveDirection dir)
		:lib(lib),bot(bot),wrapper(wrapper),dir(dir)
	{
		controler = make_unique<MovingController>(lib,bot, 3);
	}
	virtual bool Start();
	virtual void Stop();
	virtual ~LeaveClimbing() {}
private:
	void MyCallback(bool stopped);
	LeaveDirection dir;
	unique_ptr<MovingController> controler;
	unique_ptr<ActionHandler> horizontalMove;
	FrameworkLibrary* lib;
	IBotAPI* bot;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::Wait :public ActionHandler
{
public:
	Wait(MovementExecutingWrapper* wrapper, int time):wrapper(wrapper), waitTime(time) {}
	virtual bool Start();
	virtual void Stop();
private:
	void MyCallback(bool stopped);
	int waitTime;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::ActionList :public ActionHandler
{
public:
	ActionList(FrameworkLibrary* lib):lib(lib) {}
	void AddAction(unique_ptr<ActionHandler> action);
	void AddAction(unique_ptr<ActionHandlerFactory> action);
	virtual bool Start();
	virtual void Stop();
	virtual ActionState GetState();
	virtual ~ActionList() {}
private:
	void MyCallback(bool stopped);
	size_t index = -1;
	vector<unique_ptr<ActionHandler>> actions;
	FrameworkLibrary* lib;
};