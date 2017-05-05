#include <vector>
#pragma once
#include "IBot.h"
#include "FrameworkLibrary.h"

class MyBot : public IBot
{
public:
	MyBot() 
	{ 
		lib = make_unique<FrameworkLibrary>(this);
	}
	void Update() override;
	~MyBot() { }
private:
	unique_ptr<FrameworkLibrary> lib;
	int moveCount = 0;
};