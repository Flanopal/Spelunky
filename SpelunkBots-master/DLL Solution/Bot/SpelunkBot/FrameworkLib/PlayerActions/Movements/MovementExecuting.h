#pragma once
#include <vector>
#include <memory>
#include <functional>

class MovementExecuting;

#include "IBotAPI.h"
#include "DataStructures.h"

using namespace std;

class MovementExecuting
{
public:
	MovementExecuting(IBotAPI* boty) :bot(boty)
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

	void LookUpStart();
	void LookUpStop();
	bool IsLookingUp();

	void CrouchStart();
	void CrouchStop();
	bool IsCrouching();
private:
	// used to indikate callback
	enum class Identifier { MoveRight = 0, MoveLeft = 1, Jump = 2 , Crouch=3, Up = 4};
	vector<Command*> actions;

	void RemoveCallback(Identifier id); // remove currently active move by ID

	void MoveRight();
	void MoveLeft();
	void Jump();
	void Crouch();
	void LookUp();

	// binded functions for adding into action vector
	unique_ptr<Command> moveRight = make_unique<Command>((int)Identifier::MoveRight, bind(&MovementExecuting::MoveRight, this));
	unique_ptr<Command> moveLeft = make_unique<Command>((int)Identifier::MoveLeft, bind(&MovementExecuting::MoveLeft, this));
	unique_ptr<Command> jump = make_unique<Command>((int)Identifier::Jump, bind(&MovementExecuting::Jump, this));
	unique_ptr<Command> crouch = make_unique<Command>((int)Identifier::Crouch, bind(&MovementExecuting::Crouch, this));
	unique_ptr<Command> lookUp = make_unique<Command>((int)Identifier::Up, bind(&MovementExecuting::LookUp, this));

	bool _right = false;
	bool _left = false;
	bool _jump = false;
	bool _up = false;
	bool _crouch = false;

	IBotAPI* bot;
};

