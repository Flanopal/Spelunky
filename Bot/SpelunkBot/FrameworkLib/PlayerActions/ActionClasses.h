#pragma once
#include <functional>
#include <memory>
#include <set>

using namespace std;

class ActionClasses
{
public:
	class ExecutePath;
	class GoToNode;
};

#include "DataStructures.h"
#include "MovementExecutingWrapper.h"
#include "FrameworkLibrary.h"

class ActionClasses::ExecutePath :public ActionHandler
{
public:
	ExecutePath(FrameworkLibrary* lib, IBotAPI* bot, vector<unique_ptr<ActionHandlerFactory>> path) :lib(lib),bot(bot), path(move(path)) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~ExecutePath() {}
private:
	void MyCallback(bool stopped);
	bool ExecuteNextAction();
	bool CoordControl();
	size_t currentActionIndex = 0;
	unique_ptr<ActionHandler> currentAction;

	FrameworkLibrary* lib;
	IBotAPI* bot;
	vector<unique_ptr<ActionHandlerFactory>> path;
};

/*
class ActionClasses::GoToNode :public ActionHandler
{
public:
	GoToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target):lib(lib),bot(bot), target(target) {}
	virtual bool Start();
	virtual void Stop();
	virtual ~GoToNode() {}
private:
	void MyCallback(bool stopped);
	bool SetTerminatedState();
	bool ReplanPath();

	struct State;

	unique_ptr<ActionHandler> pathExecutor;
	set<State> terminatedStates;
	Coordinates target;
	IBotAPI* bot;
	FrameworkLibrary* lib;

	struct State
	{
		State(int x, int y) :x(x), y(y) {}
		State(Coordinates coords) { State(coords.x, coords.y); }
		int x;
		int y;
		bool operator==(const State& a) const { return a.x == x && a.y == y; }
		bool operator<(const State& a) const { return x < a.x; }
	};
};*/