// Description: Testing for depth-first traversal
// DocumentationOf: depth_first.h

#include "test_pastelsys.h"

#include "pastel/sys/graph/depth_first/depth_first.h"

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			test();
		}

		void test()
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
			
			TEST_ENSURE_OP(reportSet.size(), ==, correctSize);

			if (reportSet.size() == correctSize)
			{
				TEST_ENSURE(
					std::equal(std::begin(correctSet), std::end(correctSet),
					std::begin(reportSet)));
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("traverseDepthFirst", test);
	}

	CallFunction run(addTest);

}
