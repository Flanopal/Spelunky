#pragma once

namespace SearchActions
{
	class BasicAction;
	class SideMove;
	class ClimbLadder;
}

#include <vector>

#include "SearchAlgorithmInterfaces.h"
#include "MapControl.h"
#include "PathSearch.h"

using namespace std;

namespace SearchActions
{
	enum class FallType { withWaiting = 0, normal = 1, blocked = 2, toLand = 3 };
	typedef unique_ptr<ActionHandlerFactory> GetActionFunction(int x, int y, int prevX, FallType type);
	static bool ShouldWriteIntoState(const SearchCoords* const current, const SearchCoords* const target, int cost, int lifeLost, int ropeUsed);
	static int GetCost(int x, int y, const SearchCoords* const prevCoord);
	// GetNextNode in every class fills actions into space state and their price
	class SideMove
	{
	public:
		SideMove(MapControl &map, SearchCoords(&buffer)[42][34]) :map(map), buffer(buffer) {}
		vector<SearchCoords*> GetNextNodes(SearchCoords* startState);
		vector<SearchCoords*> Falling(int dx, int& x, int& y, SearchCoords* prevCoords);
		vector<SearchCoords*> Falling(int dx, int& x, int& y, SearchCoords* startState, GetActionFunction GetAction);
	private:
		vector<SearchCoords*> SideControl(int dx);
		SearchCoords* HangingCheck(int x, int y, SearchCoords* prevCoord);
		bool WriteToState(int x, int y, FallType type, SearchCoords* prevCoord);
		
		GetActionFunction* CurrentActionGetter;

		static GetActionFunction GetAction;
		static const int waitTime = 7;

		SearchCoords* startState;
		MapControl &map;
		SearchCoords(&buffer)[42][34];
	};

	class Jump
	{
	public:
		Jump(MapControl &map, SearchCoords(&buffer)[42][34]) :map(map), buffer(buffer) {}
		vector<SearchCoords*> GetNextNodes(SearchCoords* startState);
	private:
		vector<SearchCoords*> SideControl(int dx);
		vector<SearchCoords*> JumpControl(int dx, int& x, int& y);
		vector<SearchCoords*> Falling(int dx, int& x, int& y);
		bool WriteToState(int x, int y, SearchCoords* prevCoord, unique_ptr<ActionHandlerFactory> action);

		static GetActionFunction GetAction;
		static const int waitTime = 7;

		SearchCoords* startState;
		MapControl &map;
		SearchCoords(&buffer)[42][34];
	};

	class ClimbLadder
	{
	public:
		ClimbLadder(MapControl &map, SearchCoords(&buffer)[42][34]) :map(map), buffer(buffer) {}
		vector<SearchCoords*> GetNextNodes(SearchCoords* startState);
	private:
		bool WriteToState(int x, int y);
		unique_ptr<ActionHandlerFactory> GetAction(int x, int y,bool last);
		SearchCoords* startState;
		MapControl &map;
		SearchCoords(&buffer)[42][34];
	};

	class ClimbRope
	{
	public:
		vector<SearchCoords&> GetNextNodes(MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	}; 
}