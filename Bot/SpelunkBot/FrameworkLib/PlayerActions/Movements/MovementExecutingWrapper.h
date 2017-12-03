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

	bool StartMovingRight(unique_ptr<function<void()>> callback);
	bool StartMovingLeft(unique_ptr<function<void()>> callback);
	bool StartLookingUp(unique_ptr<function<void()>> callback);
	bool StartCrouching(unique_ptr<function<void()>> callback);
	void StopHorizontalMoving();
	void StopVerticalMoving();
	
	bool RegisterUpdateCallback(unique_ptr<function<void()>> callback);
	void RemoveUpdateCallback();
	void Jump(int ticks);
private:
	void MoveCallback();
	int jumpLength = 0;

	unique_ptr<function<void()>> horizontalMovingCallback;
	unique_ptr<function<void()>> verticalMovingCallback;
	unique_ptr<function<void()>> updateCallback;
	unique_ptr<MovementExecuting> executor;
};