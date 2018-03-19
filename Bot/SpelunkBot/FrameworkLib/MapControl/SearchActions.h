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
	// GetNextNode fill actions into space state and its prices
	static bool ShouldWriteIntoState(const SearchCoords* const current, const SearchCoords* const target, int lifeLost, int ropeUsed);
	
	class BasicAction
	{
	public:
		// not useabble class - used for search testing
		static vector<SearchCoords*> GetNextNodes(MapControl &map, SearchCoords* coords, SearchCoords(&buffer)[42][34]);
	};

	class SideMove
	{
	public:
		static vector<SearchCoords*> GetNextNodes(MapControl &map, SearchCoords* coords, SearchCoords(&buffer)[42][34]);
	private:
		static vector<SearchCoords*> SideControl(int dx, MapControl &map, SearchCoords* coords, SearchCoords(&buffer)[42][34]);
		static vector<SearchCoords*> Falling(int dx, int& x, int& y, MapControl &map, SearchCoords* coords, SearchCoords(&buffer)[42][34]);
		static bool WriteToState(int x, int y, SearchCoords* startState, SearchCoords* prevCoord, SearchCoords(&buffer)[42][34]);
	};

	/*static class Jump
	{
	public:
		static vector<SearchCoords&> GetNextNodes(MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	};*/

	class ClimbLadder
	{
	public:
		static vector<SearchCoords*> GetNextNodes(MapControl &map, SearchCoords* coords, SearchCoords(&buffer)[42][34]);
	private:
		static vector<SearchCoords*> DirectionClimb(int dx, MapControl &map, SearchCoords* coords, SearchCoords(&buffer)[42][34]);
		static bool WriteToState(int x, int y, SearchCoords* startState, SearchCoords(&buffer)[42][34]);
	};

	/*static class ClimbRope
	{
	public:
		static vector<SearchCoords&> GetNextNodes(MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	};

	static class SetBomb
	{
	public:
		static vector<SearchCoords&> GetNextNodes(MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	};*/
}