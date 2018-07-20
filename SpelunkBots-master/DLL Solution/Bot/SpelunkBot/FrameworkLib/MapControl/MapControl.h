#pragma once
#include <vector>

class MapControl;

#include "IBotAPI.h"
#include "DataStructures.h"
#include "FrameworkLibrary.h"

using namespace std;

class MapControl
{
public:
	MapControl(FrameworkLibrary* lib, IBotAPI* bot) :lib(lib), bot(bot), exitPos(-1,-1),exitFound(false)
	{
		top = left = bottom = right = -1;
		SaveMap();
	}
	void Update();
	FrameworkLibrary* GetCurrentLibrary() { return lib; }
	bool NodeIsTerrain(int x, int y);
	bool NodeIsTerrain(Coordinates coords);
	bool NodeIsTerrain(SearchCoords& coords);

	bool NodeIsClimable(int x, int y);
	bool NodeIsClimable(Coordinates coords);
	bool NodeIsClimable(SearchCoords& coords);

	bool NodeIsUnknown(int x, int y);
	bool NodeIsUnknown(Coordinates coords);
	bool NodeIsUnknown(SearchCoords& coords);

	bool NodeIsEmpty(int x, int y);
	bool NodeIsEmpty(Coordinates coords);
	bool NodeIsEmpty(SearchCoords& coords);

	void EmplaceRope(LeaveDirection dir);

	bool ExitIsFound();
	vector<Coordinates> GetRopesAndCrates();
	void RemoveRopeOrChest();
	void SaveMap();
	Coordinates GetExitPos();
private:
	void SavePosition(int x, int y);
	void NodeExitCheck(int x, int y);
	void UpdateCaveMap();

	bool VerticalBorderControl(int dx);
	bool HorizontalBorderControl(int dy);
	void VerticalBorderUpdate(int& line);
	void HorizontalBorderUpdate(int& line);

	bool CoordControl(int x, int y);
	vector<Coordinates> ropesAndCrates;
	int top, left, bottom, right;
	double cave[42][34];
	bool exitFound;
	Coordinates exitPos;
	FrameworkLibrary* lib;
	IBotAPI* bot;
};


// methods used for debuging
/*void CoutFrame()
{
cout << left << ":" << top << " " << right << ":" << bottom << "\n";
}

void CoutMap()
{
for (int i = 0; i < 34; ++i)
{
for (int j = 0; j < 42; ++j)
{
if (cave[j][i]!=-1)
cout << cave[j][i] << " ";
else
cout << "x ";
}
cout << "\n";
}
}
void CoutNode(int x, int y)
{
cout << cave[x][y] << endl;
}*/
