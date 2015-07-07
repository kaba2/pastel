// Description: Testing for depth-first traversal
// DocumentationOf: depth_first.h

#include "test/test_init.h"

#include "pastel/sys/graph/depth_first.h"

#include <vector>
#include <unordered_set>

TEST_CASE("traverseDepthFirst (traverseDepthFirst)")
{
	auto forEachSeedVertex = 
		[](const std::function<void(integer)>& visit)
	{
		visit(0);
	};

	auto forEachAdjacent =
		[](integer vertex, 
		const std::function<void(integer)>& visit)
	{
		if (vertex < 7)
		{
			visit(2 * vertex + 1);
			visit(2 * vertex + 2);
		}
	};

	std::unordered_set<integer> visitedSet;
	std::vector<integer> reportSet;

	auto mark = 
		[&](integer vertex)
	{
		visitedSet.insert(vertex);
		reportSet.push_back(vertex);
	};

	auto marked =
		[&](integer vertex) -> bool
	{
		return visitedSet.count(vertex);
	};

	traverseGraph<integer>(
		forEachSeedVertex,
		forEachAdjacent,
		mark,
		marked,
		DepthFirst_GraphTraversal());

	integer correctSet[] = {0, 1, 3, 7, 8, 4, 9, 10, 2, 5, 11, 12, 6, 13, 14};
	integer correctSize = std::end(correctSet) - std::begin(correctSet);

	REQUIRE(reportSet.size() == correctSize);

	if (reportSet.size() == correctSize)
	{
		REQUIRE(
			std::equal(std::begin(correctSet), std::end(correctSet),
			std::begin(reportSet)));
	}
}

