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
	MovementExecutingWrapper(IBotAPI* boty, FrameworkLibrary* parent)
	{ 
		executor = make_unique<MovementExecuting>(boty, parent);
	}
	void Update();

	bool StartMovingRight(unique_ptr<function<bool()>> callback);
	bool StartMovingLeft(unique_ptr<function<bool()>> callback);
	void Jump();
private:
	void MoveCallback();
	bool CheckCallback();

	unique_ptr<function<bool()>> movingCallback;
	unique_ptr<MovementExecuting> executor;
};