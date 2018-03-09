#pragma once

#include <vector>

class ISearchAction;

#include "SearchAlgorithmInterfaces.h"

using namespace std;

namespace SearchActions
{
	// GetNextNode fill actions into space state and its prices
	static bool ShouldWriteIntoState(const SearchCoords& current, const SearchCoords& target, int lifeLost, int ropeUsed);
	static class BasicAction
	{
	public:
		// not useabble class - used for search testing
		static vector<SearchCoords&> GetNextNodes(MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	};

	static class SideMove
	{
	public:
		static vector<SearchCoords&> GetNextNodes(MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	private:
		static vector<SearchCoords&> SideControl(int dx, MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34]);
		static vector<SearchCoords&> Falling(int dx, int& x, int& y, MapControl &map, SearchCoords& coords, SearchCoords(&buffer)[42][34]);
		static bool WriteToState(int x, int y, SearchCoords& startState, SearchCoords& prevState, SearchCoords(&buffer)[42][34]);
	};

	static class Jump
	{
	public:
		static vector<SearchCoords&> GetNextNodes(MapControl &map, const SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	};
	static class ClimbLadder
	{
	public:
		static vector<SearchCoords&> GetNextNodes(MapControl &map, const SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	};

	static class ClimbRope
	{
	public:
		static vector<SearchCoords&> GetNextNodes(MapControl &map, const SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	};

	static class SetBomb
	{
	public:
		static vector<SearchCoords&> GetNextNodes(MapControl &map, const SearchCoords& coords, SearchCoords(&buffer)[42][34]);
	};
}