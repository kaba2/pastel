// Description: Testing for searching in a kd-tree.
// DocumentationOf: search_nearest_kdtree.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/search_nearest_kdtree.h>
#include <pastel/geometry/tdtree.h>
#include <pastel/sys/vector.h>
#include <pastel/sys/locator.h>

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
			using Point = Vector2;
			using Locator = Vector2_Locator;
			using Settings = TdTree_Settings<Locator>;
			using Tree = TdTree<Settings>;
			using Point_ConstIterator = Tree::Point_ConstIterator;
			
			Tree tree;

			searchNearest(
				tree, 
				Point(1, 2),
				PASTEL_TAG(acceptPoint), nullOutput(),
				PASTEL_TAG(indicator), allIndicator(),
				PASTEL_TAG(normBijection), Euclidean_NormBijection<real>(),
				PASTEL_TAG(searchAlgorithm), DepthFirst_SearchAlgorithm_PointKdTree(),
				PASTEL_TAG(intervalSequence), Vector2{-infinity<real>(), infinity<real>()},
				PASTEL_TAG(k), 10,
				PASTEL_TAG(maxDistance2), 10,
				PASTEL_TAG(maxRelativeError), 0,
				PASTEL_TAG(nBruteForce), 0,
				PASTEL_TAG(reportMissing));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("search_nearest_kdtree", test);
	}

	CallFunction run(addTest);

}
