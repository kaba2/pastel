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
				nullOutput(),
				allIndicator(),
				Euclidean_NormBijection<real>(),
				DepthFirst_SearchAlgorithm_PointKdTree(),
				Vector2{-infinity<real>(), infinity<real>()},
				[](auto& o)
			{
				o.k = 10;
				o.maxDistance2 = 10;
				o.maxRelativeError = 0;
				o.nBruteForce = 0;
				o.reportMissing = true;				
			});
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
