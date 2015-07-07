// Description: Testing for breadth-first traversal
// DocumentationOf: breadth_first.h

#include "test/test_init.h"

#include "pastel/sys/graph/breadth_first.h"

#include <vector>
#include <unordered_set>

TEST_CASE("traverseBreadthFirst (traverseBreadthFirst)")
{
	std::vector<integer> reportSet;
	std::unordered_set<integer> visitedSet;

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
		BreadthFirst_GraphTraversal());

	integer correctSet[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	integer correctSize = std::end(correctSet) - std::begin(correctSet);

	REQUIRE(reportSet.size() == correctSize);

	if (reportSet.size() == correctSize)
	{
		REQUIRE(
			std::equal(std::begin(correctSet), std::end(correctSet),
			std::begin(reportSet)));
	}
}

