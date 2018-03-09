#pragma once

class MapControl;

using namespace std;

#include "IBotAPI.h"
#include "FrameworkLibrary.h"
#include "DataStructures.h"
#include "PathSearch.h"

class MapControl
{
public:
	MapControl(FrameworkLibrary* lib, IBotAPI* bot) :lib(lib), bot(bot), exitPos(-1,-1),exitFound(false)
	{
		top = left = bottom = right = -1;
		SaveMap();
	}
	void Update();
	bool NodeIsTerrain(int x, int y);
	bool NodeIsTerrain(Coordinates coords);
	bool NodeIsTerrain(SearchCoords& coords);

	bool NodeIsClimable(int x, int y);
	bool NodeIsClimable(Coordinates coords);
	bool NodeIsClimable(SearchCoords& coords);

	bool NodeIsUnknown(SearchCoords& coords);
	void CoutFrame()
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

	bool ExitIsFound();

	void SaveMap();
	Coordinates GetExitPos();
	//unique_ptr<PathSearch> pathSearch;
private:
	void NodeExitCheck(int x, int y);
	void UpdateCaveMap();

	bool VerticalBorderControl(int dx);
	bool HorizontalBorderControl(int dy);
	void VerticalBorderUpdate(int& line);
	void HorizontalBorderUpdate(int& line);

	int top, left, bottom, right;
	double cave[42][34];
	bool exitFound;
	Coordinates exitPos;
	FrameworkLibrary* lib;
	IBotAPI* bot;
};
