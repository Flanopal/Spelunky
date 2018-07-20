#pragma once
#include <vector>
#include <functional>

#include "IBotAPI.h"
#include "FrameworkLibrary.h"
#include "GOAP.h"

using namespace std;

class MyBot : public IBotAPI
{
public:
	MyBot():action(nullptr)
	{ 
		// WARNING! DO NOT USE PLANNING IN CONSTRUCTOR - PLAYER COORDINATES ARE NOT SET YET !!!!!

		lib = make_unique<FrameworkLibrary>(this);
		goap = make_unique<GOAP>(&*lib);
	}
	///Method with whole bot logic
	void Update() override;
	~MyBot() { }
private:
	bool switched = false;
	bool finish = false;
	unique_ptr<FrameworkLibrary> lib;
	unique_ptr<GOAP> goap;
	unique_ptr<ActionHandler> action;
};