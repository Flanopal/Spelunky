#pragma once
#include <vector>
#include <functional>

#include "IBotAPI.h"
#include "FrameworkLibrary.h"

using namespace std;

class MyBot : public IBotAPI
{
public:
	MyBot():action(nullptr)
	{ 
#if WithTrace
		cout << "\n\nMy bot constructed\n";
		cout << "--------Framework Output-----------\n";
#endif
		lib = make_unique<FrameworkLibrary>(this);
		//action = lib->playerActions->movements->SideMoveAt(30);
		action = lib->playerActions->movements->MoveRightFor(3);
		//action = lib->playerActions->movements->LookUpFor(50);
		action->Start();
		//lib->mapControl->CoutFrame();
		//lib->mapControl->CoutMap();

		/*action = lib->playerActions->movements->SideMoveAt(10);
		action->Start();*/
	}
	void Update() override;
	~MyBot() { }
private:
	bool switched = false;
	int cd = 0;
	unique_ptr<FrameworkLibrary> lib;
	unique_ptr<ActionHandler> action;
};