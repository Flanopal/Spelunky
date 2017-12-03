#pragma once

#include <functional>
#include <vector>
#include <iostream>

using namespace std;

enum class ActionState { waiting, runnig, finished, terminated };
enum class LeaveDirection { stay = 0, right, left };

struct Coordinates
{
public:
	Coordinates(double x, double y) :x(x), y(y) {}
	double x;
	double y;
	bool operator==(const Coordinates& target)
	{
		if (target.x == x && target.y == y) return true;
		return false;
	}
};

class Command
{
public:
	Command(int id, function<void()> func) :identifier(id), callback(func) {}
	int identifier;
	function<void()> callback;
};

class ActionHandler
{
public:
	ActionHandler()
	{
		state = ActionState::waiting;
		parentCallback = nullptr;
	}
	virtual ActionState GetState()
	{
		return state;
	}
	virtual bool Start()
	{
		return true;
	}
	virtual void Stop()
	{
	}
	virtual void registrCallback(unique_ptr<function<void()>> callback)
	{
		this->parentCallback = move(callback);
	}
	virtual void callParentCallback()
	{
		if (parentCallback != nullptr)
			(*parentCallback)();
	}
	virtual ~ActionHandler() {}	
protected:
	virtual void Finish()
	{
		Stop();
		state = ActionState::finished;
	}
	bool EqualValues(double val1, double val2)
	{
		double dif = val1 - val2;
		if (dif > 0.2 || dif < -0.2)
			return false;
		return true;
	}
	unique_ptr<function<void()>> parentCallback;
	ActionState state=ActionState::waiting;
};
