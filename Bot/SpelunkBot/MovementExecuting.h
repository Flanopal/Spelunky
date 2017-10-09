#pragma once
#include <vector>
#include <memory>
#include <functional>

class MovementExecuting;

#include "FrameworkLibrary.h"
#include "IBotAPI.h"
#include "DataStructures.h"

using namespace std;

class MovementExecuting
{
public:
	MovementExecuting(IBotAPI* boty, FrameworkLibrary* parent) :bot(boty), parentLib(parent) 
	{ }
	void Update();

	void MoveRightStart();
	void MoveRightStop();
	bool IsMovingRight();

	void MoveLeftStart();
	void MoveLeftStop();
	bool IsMovingLeft();

	void JumpStart();
	void JumpStop();
	bool IsJumping();
private:
	// used to indikate callback
	enum class Identifier { MoveRight = 0, MoveLeft = 1, Jump = 2 };
	vector<Command*> actions;

	void RemoveCallback(Identifier id);

	void MoveRight();
	void MoveLeft();
	void Jump();

	// binded functions for faster adding into action vector
	unique_ptr<Command> moveRight = make_unique<Command>((int)Identifier::MoveRight, bind(&MovementExecuting::MoveRight, this));
	unique_ptr<Command> moveLeft = make_unique<Command>((int)Identifier::MoveLeft, bind(&MovementExecuting::MoveLeft, this));
	unique_ptr<Command> jump = make_unique<Command>((int)Identifier::Jump, bind(&MovementExecuting::Jump, this));

	bool _right = false;
	bool _left = false;
	bool _jump = false;

	FrameworkLibrary* parentLib;
	IBotAPI* bot;
};

