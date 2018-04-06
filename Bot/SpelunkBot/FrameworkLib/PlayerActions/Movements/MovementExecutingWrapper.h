#pragma once
#include <vector>
#include <memory>
#include <functional>

class MovementExecutingWrapper;

#include "MovementExecuting.h"
#include "IBotAPI.h"
#include "DataStructures.h"

using namespace std;

class MovementExecutingWrapper
{
public:
	MovementExecutingWrapper(IBotAPI* bot)
	{ 
		executor = make_unique<MovementExecuting>(bot);
	}
	void Update();

	bool StartMovingRight(WrapperCallback callback);
	bool StartMovingLeft(WrapperCallback callback);
	bool StartLookingUp(WrapperCallback callback);
	bool StartCrouching(WrapperCallback callback);
	void StopHorizontalMoving();
	void StopVerticalMoving();
	
	bool RegisterUpdateCallback(WrapperCallback callback);
	void RemoveUpdateCallback();
	bool SetWaiting(int time, WrapperCallback callback);
	void StopWaiting();
	void Jump(int ticks);
private:
	void CallMoveCallback();
	void CallUpdateCallback();
	bool IsWaiting();

	int jumpLength = 0;
	int waitTime = 0;

	WrapperCallback horizontalMovingCallback;
	WrapperCallback verticalMovingCallback;
	WrapperCallback updateCallback;
	WrapperCallback waitCallback;
	unique_ptr<MovementExecuting> executor;
};