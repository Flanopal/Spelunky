#pragma once
#include <functional>
#include <memory>
using namespace std;

class BaseMVClasses
{
public:
	class MoveFor;
	class SideMoveAt;
	class JumpToSpot;
	class ClimbToNodeLevel;
	class LeaveClimbing;
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
	void MyCallback();
	int dist;
	bool left;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::SideMoveAt :public ActionHandler
{
public:
	SideMoveAt(MovementExecutingWrapper* wrapper, IBotAPI* bot, double coordX) : wrapper(wrapper), bot(bot), coordX(coordX), previousPos(0,0)
	{
	}
	virtual bool Start();
	virtual void Stop();
	virtual ~SideMoveAt() {}
private:
	void MyCallback();
	bool left;
	double coordX;
	Coordinates previousPos;
	int samePlaceTime = 0;
	const int samePlaceTimeLimit = 5;
	MovementExecutingWrapper* wrapper;
	IBotAPI* bot;
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
	void MyCallback();
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
	void MyCallback();
	double targetLvl;
	double targetX;
	bool up;
	FrameworkLibrary* lib;
	IBotAPI* bot;
	MovementExecutingWrapper* wrapper;
};

class BaseMVClasses::LeaveClimbing
{

};