// Description: Testing for matchPointsKr
// DocumentationOf: match_points_kr.h

#include "test/test_init.h"

#include <pastel/geometry/pattern_matching/match_points_kr.h>
#include <pastel/geometry/distance/distance_point_point.h>

#include <pastel/sys/random.h>
#include <pastel/sys/range.h>
#include <pastel/sys/rational.h>

#include <pastel/geometry/pointkdtree.h>

TEST_CASE("matchPointsKr (matchPointsKr)")
{
	//using Real = Rational<integer>;
	using Real = real;
	using Point = Vector<Real, 2>;
	using Tree_Settings = PointKdTree_Settings<Vector_Locator<Real, 2>>;
	using Tree = PointKdTree<Tree_Settings>;

	Point translation(1, 5);

	// Generate a point-set.

	integer n = 100;
	std::vector<Point> modelSet;
	modelSet.reserve(n);

	for (integer i = 0;i < n;++i)
	{
		Point p = 2 * randomVector<Real, 2>() - 1;
		modelSet.push_back(p);
	}

	Tree modelTree;
	modelTree.insertSet(rangeSet(modelSet));
	modelTree.refine();

	// Generate a translated point-set.

	int m = n;
	std::vector<Point> sceneSet;
	sceneSet.reserve(m);

	for (integer i = 0; i < m; ++i)
	{
		sceneSet.push_back(modelSet[i] + translation);
	}

	// Randomize the order of the translated point-set.
	std::random_shuffle(sceneSet.begin(), sceneSet.end());

	Tree sceneTree;
	sceneTree.insertSet(rangeSet(sceneSet));
	sceneTree.refine();

	using Tree_ConstIterator = Tree::Point_ConstIterator;

	std::vector<std::pair<Tree_ConstIterator, Tree_ConstIterator>> pairSet;

	Real matchingDistance2 = 0.01;

	auto result = matchPointsKr(
		modelTree, sceneTree, 
		PASTEL_TAG(kNearest), 16,
		PASTEL_TAG(minMatchRatio), 0.7,
		PASTEL_TAG(matchingDistance2), matchingDistance2,
		PASTEL_TAG(maxBias), 0.1,
		PASTEL_TAG(report), pushBackOutput(pairSet));

	REQUIRE(result.success);
	REQUIRE(allEqual(translation, result.translation));

	for (auto&& pairing : pairSet)
	{
		Real actualDistance = distance2(pairing.first->point(), evaluate(pairing.second->point() + result.translation));
		REQUIRE(actualDistance <= matchingDistance2);
	}
}

