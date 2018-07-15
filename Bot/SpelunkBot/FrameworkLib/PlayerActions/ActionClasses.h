#pragma once
#include <functional>
#include <memory>
#include <set>

using namespace std;

class ActionClasses
{
public:
	class ExecutePath;
	class GoDirectlyToNode;
	class ExploreToNode;
};

#include "DataStructures.h"
#include "MovementExecutingWrapper.h"
#include "FrameworkLibrary.h"

class ActionClasses::ExecutePath :public ActionHandler
{
public:
	ExecutePath(FrameworkLibrary* lib, IBotAPI* bot, vector<unique_ptr<ActionHandlerFactory>> path);
	virtual bool Start();
	virtual void Stop();
	virtual ~ExecutePath() {}
private:
	void MyCallback(bool stopped);
	bool ExecuteNextAction();
	bool CoordControl();
	int currentActionIndex;
	unique_ptr<ActionHandler> currentAction;

	FrameworkLibrary* lib;
	IBotAPI* bot;
	vector<unique_ptr<ActionHandlerFactory>> path;
};


class ActionClasses::GoDirectlyToNode :public ActionHandler
{
public:
	GoDirectlyToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target):lib(lib),bot(bot), target(target) 
	{
		cout << " targ x " << target.x << " y " << target.y << endl;
	}
	GoDirectlyToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target, int life, int rope) :lib(lib), bot(bot), target(target),life(life),rope(rope) {}
	bool PlanPath();
	virtual bool Start();
	virtual void Stop();
	virtual ~GoDirectlyToNode() {}
private:
	void MyCallback(bool stopped);

	bool NearbyCoords(Coordinates first, Coordinates second);

	int life = -1;
	int rope;
	unique_ptr<ActionHandler> pathExecutor;
	Coordinates target;
	IBotAPI* bot;
	FrameworkLibrary* lib;

	/*struct State
	{
		State(int x, int y) :x(x), y(y) {}
		State(Coordinates coords) { State(coords.x, coords.y); }
		int x;
		int y;
		bool operator==(const State& a) const { return a.x == x && a.y == y; }
		bool operator<(const State& a) const { return x < a.x; }
	};*/
};

class ActionClasses::ExploreToNode : public ActionHandler
{
public:
	ExploreToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target) :lib(lib), bot(bot), target(target) {}
	ExploreToNode(FrameworkLibrary* lib, IBotAPI* bot, Coordinates target, int life, int rope) :lib(lib), bot(bot), target(target), life(life), rope(rope) {}
	bool PlanPath();
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