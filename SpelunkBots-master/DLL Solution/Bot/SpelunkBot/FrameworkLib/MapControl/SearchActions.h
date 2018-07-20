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

	// For specific movement return specific action
	typedef unique_ptr<ActionHandlerFactory> GetActionFunction(int x, int y, int prevX, FallType type);

	static bool ShouldWriteIntoState(const SearchCoords* const current, const SearchCoords* const target, int cost, int lifeLost, int ropeUsed);
	static int GetCost(int x, int y, const SearchCoords* const prevCoord, MoveType type);
	
	// GetNextNode method of every class returns new states reachable from actual state
	
	class Fall
	{
	public:
		Fall(MapControl &map, SearchCoords(&buffer)[42][34], GetActionFunction* GetAction, MoveType type) :map(map), buffer(buffer), ActionGetter(GetAction) 
		{
			// Initialize data vectors for current fall type
			moveType = type;
			GetFallingVectors(type);
		}
		/// Falling with default falling vectors
		vector<SearchCoords*> Falling(int dx, int& x, int& y, SearchCoords* startState, SearchCoords* prevCoords);
		
		/// Specifies falling vectors with type param
		vector<SearchCoords*> Falling(int dx, int& x, int& y, SearchCoords* startState, SearchCoords* prevCoords, MoveType type);
	private:
		void GetFallingVectors(MoveType type);

		/// Write info about reaching state (x,y)
		bool WriteToState(int x, int y, FallType type, SearchCoords* prevCoord);
		SearchCoords* HangingCheck(int x, int y, SearchCoords* prevCoord);

		/// Return true if vector contains value x
		bool ContainsValue(vector<int> values, int x);

		int firstY; // initial value of y

		MoveType moveType;

		/// Gets proper moving action in actual falling search
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
		/// Method using falling object for side move
		vector<SearchCoords*> Falling(int dx, int& x, int& y, SearchCoords* prevCoords);
	private:
		/// Control movement to one side - right/left
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
		/// Control movement to one side - right/left
		vector<SearchCoords*> SideControl(int dx);

		/// Control situation while agent is jumping
		vector<SearchCoords*> JumpControl(int dx, int& x, int& y);

		/// SPecific control for jumping from ladder
		vector<SearchCoords*> LadderJumpControl(int dx, int& x, int& y);
		vector<SearchCoords*> Falling(int dx, int& x, int& y);
		vector<SearchCoords*> Falling(int dx, int& x, int& y, MoveType newType);
		bool WriteToState(int x, int y, SearchCoords* prevCoord, unique_ptr<ActionHandlerFactory> action);

		MoveType type;

		/// Method for getting action for this movement - passed into falling
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

		/// say if rope is worth in actual situation
		bool ColumnWorthToUse(int x, int y, int dy); 

		/// initial state for climbing - place rope and climb on it
		bool WriteToInitialPossition(int x, int y, LeaveDirection dir); 
		bool WriteToState(int x, int y, SearchCoords* prevCoords);

		/// Return ation for initial rope possition
		unique_ptr<ActionHandlerFactory> GetInitialAction(int x, int y, LeaveDirection dir);
		unique_ptr<ActionHandlerFactory> GetAction(int x, int y, bool last);

		SearchCoords* startState;
		MapControl & map;
		SearchCoords(&buffer)[42][34];
	}; 
}