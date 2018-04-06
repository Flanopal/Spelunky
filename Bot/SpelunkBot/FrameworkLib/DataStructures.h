#pragma once

#include <functional>
#include <vector>
#include <iostream>
#include <memory>

class FrameworkLibrary;

using namespace std;

enum class ActionState { waiting, runnig, finished, terminated };
enum class LeaveDirection { stay = 0, right, left };

typedef unique_ptr<function<void(bool)>> WrapperCallback;

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
	bool EqualCoordinates(const Coordinates& target)
	{
		if (EqualValues(target.x, x) && EqualValues(target.y, y)) return true;
		return false;
	}
private:
	bool EqualValues(double val1, double val2)
	{
		double dif = val1 - val2;
		if (dif > 0.2 || dif < -0.2)
			return false;
		return true;
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
	virtual void registrCallback(WrapperCallback callback)
	{
		this->parentCallback = move(callback);
	}
	virtual void callParentCallback(bool stopped)
	{
		if (parentCallback != nullptr)
			(*parentCallback)(stopped);
	}
	virtual ~ActionHandler() {}	
protected:
	virtual void Finish()
	{
		Stop();
		state = ActionState::finished;
	}
	virtual bool StopControl(bool stopped)
	{
		if (stopped)
		{
			state = ActionState::terminated;
			return true;
		}
		return false;
	}
	bool EqualValues(double val1, double val2)
	{
		double dif = val1 - val2;
		if (dif > 0.2 || dif < -0.2)
			return false;
		return true;
	}
	bool CanBeStopped() { return GetState() == ActionState::runnig; }
	WrapperCallback parentCallback;
	ActionState state=ActionState::waiting;
};

class ActionHandlerFactory
{
public:
	ActionHandlerFactory(double finalX, double finalY) :finalCoords(finalX, finalY) {}
	ActionHandlerFactory(Coordinates finalCoords) :finalCoords(finalCoords) {}
	virtual unique_ptr<ActionHandler> GetAction(FrameworkLibrary* lib) = 0;
	virtual string GetActionDescrition() { return actionDescription; }
	Coordinates GetFinalCoords() { return finalCoords; }
protected:
	string actionDescription;
	Coordinates finalCoords;
};

struct AdditionalInfo
{
	int lifeCount = 1;
	int ropeCount = 0;
	int bombCount = 0;
};

struct SearchCoords
{
	int x = 0;
	int y = 0;

	SearchCoords* previousState = nullptr;
	unique_ptr<ActionHandlerFactory> action;

	int completePrice = 0;
	int currentDistance = INT32_MAX;

	AdditionalInfo spelunkerState;

	void SetCoords(int x, int y) { this->x = x; this->y = y; }

	void Visit(SearchCoords* prevState, int actionCount, unique_ptr<ActionHandlerFactory> action, AdditionalInfo state)
	{
		previousState = prevState;
		currentDistance = actionCount;
		completePrice = 0;
		this->action = move(action);
		spelunkerState = state;
	}

	bool IsEqualCoords(const SearchCoords& a) const
	{
		if (a.x == x && a.y == y) return true;
		return false;
	}
	bool IsEqualCoords(const SearchCoords* const a) const
	{
		if (a->x == x && a->y == y) return true;
		return false;
	}

	bool operator<(const SearchCoords& a) const
	{
		if (a.completePrice > completePrice) return true;
		return false;
	}
	bool operator<(const SearchCoords* const a) const
	{
		if (a->completePrice > completePrice) return true;
		return false;
	}
	bool operator>(const SearchCoords& a) const
	{
		if (a.completePrice < completePrice) return true;
		return false;
	}
	bool operator>(const SearchCoords* const a) const
	{
		if (a->completePrice < completePrice) return true;
		return false;
	}
	bool operator==(const SearchCoords& a) const
	{
		if (a.completePrice == completePrice) return true;
		return false;
	}
	bool operator==(const SearchCoords* const a) const
	{
		if (a->completePrice == completePrice) return true;
		return false;
	}
};