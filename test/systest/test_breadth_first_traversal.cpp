// Description: Testing for breadth-first traversal
// DocumentationOf: breadth_first_traversal.h

#include "test_pastelsys.h"

#include "pastel/sys/breadth_first_traversal.h"

using namespace Pastel;
using namespace std;

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
		testRunner().add("traverseBreadthFirst", test);
	}

	CallFunction run(addTest);

}
