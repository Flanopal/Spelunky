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
	enum class MoveType { sideMove = 0, jump, jumpFromLadder, ladderWithTopBlock,withoutSpeed };

	typedef bool CanBeHanged(int firstY, int y);
	typedef int GetNextColumnYCoord(int column);
	typedef unique_ptr<ActionHandlerFactory> GetActionFunction(int x, int y, int prevX, FallType type);

	static bool ShouldWriteIntoState(const SearchCoords* const current, const SearchCoords* const target, int cost, int lifeLost, int ropeUsed);
	static int GetCost(int x, int y, const SearchCoords* const prevCoord, MoveType type);
	// GetNextNode in every class fills actions into space state and their price
	
	class Fall
	{
	public:
		Fall(MapControl &map, SearchCoords(&buffer)[42][34], GetActionFunction* GetAction, MoveType type) :map(map), buffer(buffer), ActionGetter(GetAction) 
		{
			// Initialize data vectors for current fall type
			moveType = type;
			GetFallingVectors(type);
		}
		vector<SearchCoords*> Falling(int dx, int& x, int& y, SearchCoords* startState, SearchCoords* prevCoords);
		vector<SearchCoords*> Falling(int dx, int& x, int& y, SearchCoords* startState, SearchCoords* prevCoords, MoveType type);
	private:
		void GetFallingVectors(MoveType type);
		bool WriteToState(int x, int y, FallType type, SearchCoords* prevCoord);
		SearchCoords* HangingCheck(int x, int y, SearchCoords* prevCoord);
		bool ContainsValue(vector<int> values, int x);

		MoveType moveType;
		GetActionFunction* ActionGetter;

		//Currently used data vectors
		vector<int> yCoordsDiffs;
		vector<int> hangColumns;

		// Data for fall computing for different fall types
		vector<int> sideMoveHanging = { 2 };
		vector<int> sideMoveColumnsYCoordDiff = {1,1,3,3,4,3,3,4};

		vector<int> jumpHanging = { 2,3,4,6,7 };
		vector<int> jumpColumnsYCoordDiff = {0,1,2,4,3,3,4};

		vector<int> ladderJumpHanging = { 2 };
		vector<int> ladderJumpColumnsYCoordDiff = { 0,1,2,3,4,3,3,4};

		vector<int> ladderWithTopBlockHang = { };
		vector<int> ladderWithTopBlockColumnsYCoordDiff = {0,1,3,3,4};

		vector<int> withoutInitiativeHanging { };
		vector<int> withoutInitiativeColumnsYCoordDiff = { 3,3,4,3,3,4 };

		SearchCoords* startState;
		MapControl &map;
		SearchCoords(&buffer)[42][34];
	};

	class SideMove
	{
	public:
		SideMove(MapControl &map, SearchCoords(&buffer)[42][34]) :map(map), buffer(buffer) {}
		vector<SearchCoords*> GetNextNodes(SearchCoords* startState);
		vector<SearchCoords*> Falling(int dx, int& x, int& y, SearchCoords* prevCoords);
	private:
		vector<SearchCoords*> SideControl(int dx);
		bool WriteToState(int x, int y, SearchCoords* prevCoord);

		static GetActionFunction GetAction;
		static const int waitTime = 7;
		unique_ptr<Fall> falling;
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
		vector<SearchCoords*> LadderJumpControl(int dx, int& x, int& y);
		vector<SearchCoords*> Falling(int dx, int& x, int& y);
		vector<SearchCoords*> Falling(int dx, int& x, int& y, MoveType newType);
		bool WriteToState(int x, int y, SearchCoords* prevCoord, unique_ptr<ActionHandlerFactory> action);

		MoveType type;
		static GetActionFunction GetAction;
		static const int waitTime = 7;
		unique_ptr<Fall> falling;
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
		bool WriteToState(int x, int y, SearchCoords* prevCoords);
		unique_ptr<ActionHandlerFactory> GetAction(int x, int y,bool last);
		SearchCoords* startState;
		MapControl &map;
		SearchCoords(&buffer)[42][34];
	};

	class ClimbRope
	{
	public:
		ClimbRope(MapControl &map, SearchCoords(&buffer)[42][34]) :map(map), buffer(buffer) {}
		vector<SearchCoords*> GetNextNodes(SearchCoords* startState);
	private:
		vector<SearchCoords*> GetColumn(int x, int y, int dy, LeaveDirection dir);
		bool ColumnWorthToUse(int x, int y, int dy);
		bool WriteToInitialPossition(int x, int y, LeaveDirection dir);
		bool WriteToState(int x, int y, SearchCoords* prevCoords);
		unique_ptr<ActionHandlerFactory> GetAction(int x, int y, bool last);

		SearchCoords* startState;
		MapControl & map;
		SearchCoords(&buffer)[42][34];
	}; 
}