#pragma once
#include <vector>

#include "IBotAPI.h"
#include "FrameworkLibrary.h"

class MyBot : public IBotAPI
{
public:
	MyBot() 
	{ 
#if WithTrace
		cout << "\n\nMy bot constructed\n";
		cout << "--------Framework Output-----------\n";
#endif
		lib = make_unique<FrameworkLibrary>(this);
		lib->playerActions->movements->MoveLeftStart();
	}
	void Update() override;
	~MyBot() { }
private:
	unique_ptr<FrameworkLibrary> lib;
	int moveCount = 0;
};