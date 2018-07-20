#pragma once
#include <functional>
#include <memory>

using namespace std;

class BaseMVClasses
{
public:
	class MoveFor;
	class LookUpFor;
	class CrouchFor;
	class SideMoveAt;
	class Jump;
	class JumpToSpot;
	class GetOnClimbing;
	class ClimbToNodeLevel;
	class LeaveClimbing;
	class Wait;
	class WaitForLanding;
	class ActionList;
};

#include "MovementExecutingWrapper.h"
#include "DataStructures.h"
#include "FrameworkLibrary.h"

class BaseMVClasses::MoveFor:public ActionHandler
{
public:
	MoveFor(MovementExecutingWrapper* wrapper, int dist, bool left) :wrapper(wrapper), dist(dist), left(left) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~MoveFor() {}
private:
	void MyCallback(bool stopped);
	int dist;
	bool left;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::LookUpFor :public ActionHandler
{
public:
	LookUpFor(MovementExecutingWrapper* wrapper, int ticks) :wrapper(wrapper),ticks(ticks) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~LookUpFor() {}
private:
	void MyCallback(bool stopped);
	int ticks;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::CrouchFor :public ActionHandler
{
public:
	CrouchFor(MovementExecutingWrapper* wrapper, int ticks) :wrapper(wrapper), ticks(ticks) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~CrouchFor() {}
private:
	void MyCallback(bool stopped);
	int ticks;
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
	double jumpPos;
	bool terminated = false;
	Coordinates target;
	unique_ptr<ActionHandler> horizontalMove;
	FrameworkLibrary* lib;
	IBotAPI* bot;
};

class BaseMVClasses::GetOnClimbing :public ActionHandler
{
public:
	GetOnClimbing(FrameworkLibrary* lib, IBotAPI* bot, MovementExecutingWrapper* wrapper) :lib(lib), bot(bot), wrapper(wrapper) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~GetOnClimbing() {}
private:
	void MyCallback(bool stopped);

	double targetX;
	double middleX;
	int middleTick = 0;
	unique_ptr<ActionHandler> horizontalMove;
	unique_ptr<ActionHandler> lookUp;
	FrameworkLibrary* lib;
	IBotAPI* bot;
	MovementExecutingWrapper* wrapper;
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
	bool StartClimbing();
	void MyCallback(bool stopped);
	void StartClimbingCallback(bool stopped);

	unique_ptr<ActionHandler> startClimbing;
	double targetLvl;
	double targetX;
	bool up;
	bool started = false;
	FrameworkLibrary* lib;
	IBotAPI* bot;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::LeaveClimbing :public ActionHandler
{
public:
	LeaveClimbing(FrameworkLibrary* lib, IBotAPI* bot, LeaveDirection dir)
		:lib(lib),bot(bot),dir(dir)
	{}
	virtual bool Start();
	virtual void Stop();
	virtual ActionState GetState();
	virtual ~LeaveClimbing() {}
private:
	void MyCallback(bool stopped);
	LeaveDirection dir;
	unique_ptr<ActionList> action;
	FrameworkLibrary* lib;
	IBotAPI* bot;
};

class BaseMVClasses::Wait :public ActionHandler
{
public:
	Wait(MovementExecutingWrapper* wrapper, int time):wrapper(wrapper), waitTime(time) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~Wait() {}
private:
	void MyCallback(bool stopped);
	int waitTime;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::WaitForLanding :public ActionHandler
{
public:
	WaitForLanding(FrameworkLibrary* lib, IBotAPI* bot, MovementExecutingWrapper* wrapper, double x) :lib(lib), bot(bot), wrapper(wrapper),targetX(x) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~WaitForLanding() {}
private:
	void MyCallback(bool stopped);
	double targetX;
	unique_ptr<ActionHandler> horizontalMove;
	FrameworkLibrary* lib;
	IBotAPI* bot;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::ActionList :public ActionHandler
{
public:
	ActionList(FrameworkLibrary* lib):lib(lib) {}
	// adding action into list
	void AddAction(unique_ptr<ActionHandler> action);
	void AddAction(unique_ptr<ActionHandlerFactory> action);

	virtual bool Start();
	virtual void Stop();
	virtual ActionState GetState();
	virtual ~ActionList() {}
private:
	void MyCallback(bool stopped);
	void StartNextActionInList();
	size_t index = -1; // index of currently using action
	vector<unique_ptr<ActionHandler>> actions;
	FrameworkLibrary* lib;
};