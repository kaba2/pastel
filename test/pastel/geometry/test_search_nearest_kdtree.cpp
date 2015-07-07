// Description: Testing for searching in a kd-tree.
// DocumentationOf: search_nearest_kdtree.h

#include "test/test_init.h"

#include <pastel/geometry/search_nearest_kdtree.h>
#include <pastel/geometry/tdtree.h>
#include <pastel/sys/vector.h>
#include <pastel/sys/locator.h>

namespace
{

	class Test
	{
	public:
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
				PASTEL_TAG(accept), nullOutput(),
				PASTEL_TAG(indicator), allIndicator(),
				Euclidean_NormBijection<real>(),
				PASTEL_TAG(searchAlgorithm), DepthFirst_SearchAlgorithm_PointKdTree(),
				PASTEL_TAG(intervalSequence), Vector2{-infinity<real>(), infinity<real>()},
				PASTEL_TAG(kNearest), 10,
				PASTEL_TAG(maxDistance2), 10,
				PASTEL_TAG(maxRelativeError), 0,
				PASTEL_TAG(nBruteForce), 0,
				PASTEL_TAG(reportMissing));
		}
	};

	TEST_CASE("search_nearest_kdtree", "[search_nearest_kdtree]")
	{
	}

}