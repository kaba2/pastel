// Description: Testing for searching in a kd-tree.
// DocumentationOf: search_nearest.h

#include "test/test_init.h"

#include <pastel/geometry/search_nearest.h>
#include <pastel/geometry/kdtree_nearestset.h>
#include <pastel/geometry/tdtree.h>
#include <pastel/sys/vector.h>
#include <pastel/sys/locator.h>

TEST_CASE("search_nearest_kdtree (search_nearest_kdtree)")
{
	using Point = Vector2;
	using Locator = Vector2_Locator;
	using Settings = TdTree_Settings<Locator>;
	using Tree = TdTree<Settings>;
	using Point_ConstIterator = Tree::Point_ConstIterator;

	Tree tree;

	auto nearestSet = kdTreeNearestSet(tree);

	PASTEL_CONCEPT_CHECK(decltype(nearestSet), NearestSet_Concept);

	searchNearest(
		nearestSet,		 
		Point(1, 2),
		PASTEL_TAG(accept), nullOutput(),
		PASTEL_TAG(indicator), allIndicator(),
		Euclidean_NormBijection<real>(),
		PASTEL_TAG(searchAlgorithm), DepthFirst_SearchAlgorithm_PointKdTree(),
		PASTEL_TAG(intervalSequence), Vector2{-(real)Infinity(), (real)Infinity()},
		PASTEL_TAG(kNearest), 10,
		PASTEL_TAG(maxDistance2), 10,
		PASTEL_TAG(maxRelativeError), 0,
		PASTEL_TAG(nBruteForce), 0,
		PASTEL_TAG(reportMissing)
	);
}

