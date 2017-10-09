#pragma once
#include <vector>
#include <functional>

#include "IBotAPI.h"
#include "FrameworkLibrary.h"

using namespace std;

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
		lib->playerActions->movements->StartMovingLeft(make_unique<function<bool()>>(bind(&MyBot::right, this)));
	}
	bool left() 
	{ 
		if (k < -20)
			return false;
		--k;
		return true; 
	}
	bool right() 
	{ 
		if (k > 20) 
			return false;
		++k;
		return true; 
	}
	void Update() override;
	~MyBot() { }
private:
	int k = 0;
	unique_ptr<FrameworkLibrary> lib;
	int moveCount = 0;
};