// Description: Testing for temporal kd-tree
// Documentation: tdtree.txt

#include "test/test_init.h"

#include "pastel/geometry/tdtree/tdtree.h"

#include "pastel/geometry/search_nearest.h"
#include "pastel/geometry/kdtree_nearestset.h"
#include "pastel/geometry/nearestset/bruteforce_nearestset.h"
#include "pastel/geometry/distance/distance_point_point.h"

#include "pastel/sys/locator.h"
#include "pastel/sys/set.h"
#include "pastel/sys/output.h"
#include "pastel/sys/for_each_point.h"
#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/iterator/counting_iterator.h"
#include "pastel/sys/iterator/constant_iterator.h"

namespace
{

	using Point = Vector2;
	using Locator = Vector_Locator<real, 2>;
	using Tree = TdTree<TdTree_Settings<Locator>>;
	using ConstIterator = Tree::ConstIterator;


}

TEST_CASE("Concepts (TdTree)")
{
	PASTEL_CONCEPT_CHECK(Tree, PointSet_Concept);
	PASTEL_CONCEPT_CHECK(PointSet_Real<Tree>, Real_Concept);
}

TEST_CASE("Construction (TdTree)")
{
	Tree tree;
	REQUIRE(tree.simple());

	tree.clear();
	REQUIRE(tree.simple());

	Tree().swap(tree);
}

TEST_CASE("Grid (TdTree)")
{
	Array<Point> pointSet(Vector2i(5, 5));

	forEachPoint(
		AlignedBox2i(Point(0), Point(5)),
		[&](const Point& point)
	{
		pointSet(point[0], point[1]) = point;
	});

	PASTEL_CONCEPT_CHECK(decltype(pointSet), Set_Concept);
	pointSetLocator(pointSet);

	using PointSet = decltype(pointSet);
	PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept(PointSet_PointId<PointSet>));

	Tree tree(pointSet);
	REQUIRE(tree.simple());

	REQUIRE(tree.bound() == 
		AlignedBox2(Vector2(0, 0), Vector2(4, 4)));

	REQUIRE(tree.timeToIndex(-(real)Infinity()) == 0);
	REQUIRE(tree.timeToIndex(-2) == 0);
	REQUIRE(tree.timeToIndex(-1) == 0);
	REQUIRE(tree.timeToIndex(-0.5) == 0);
	REQUIRE(tree.timeToIndex(0) == 0);
	REQUIRE(tree.timeToIndex(0.5) == 1);
	REQUIRE(tree.timeToIndex(1) == 1);
	REQUIRE(tree.timeToIndex(24) == 24);
	REQUIRE(tree.timeToIndex(24.5) == 25);
	REQUIRE(tree.timeToIndex(25.5) == 25);
	REQUIRE(tree.timeToIndex(26) == 25);
	REQUIRE(tree.timeToIndex((real)Infinity()) == 25);

	std::unordered_set<ConstIterator, IteratorAddress_Hash> neighborSet;

	auto report = [&](
		real distance,
		const ConstIterator& point)
	{
		neighborSet.insert(point);
	};

	searchNearest(
		kdTreeNearestSet(tree), 
		Point(1, 3), 
		PASTEL_TAG(report), report,
		PASTEL_TAG(kNearest), 5);

	auto pointSet_ = tree.pointSetSet();

	REQUIRE(neighborSet.size() == 5);
	//REQUIRE(neighborSet.count(pointSet(1, 3)) > 0);
}

TEST_CASE("Linear (TdTree)")
{
	std::vector<Point> pointSet;
	integer n = 1000;
	for (integer i = 0; i < n; ++i)
	{
		pointSet.emplace_back(i, 0);
	}

	Tree tree(pointSet);
	REQUIRE(tree.simple());

	for (integer i = 0; i < n; ++i)
	{
		{
			Vector2 timeInterval = { (real)i, (real)n };
			auto nearestSet = kdTreeNearestSet(tree, PASTEL_TAG(intervalSequence), timeInterval);
			integer distance =
				searchNearest(
					nearestSet, 
					Point(0, 0)).first;
			REQUIRE(distance == square(i));
		}
		{
			Vector2 timeInterval = { (real)0, (real)i + 1 };
			auto nearestSet = kdTreeNearestSet(tree, PASTEL_TAG(intervalSequence), timeInterval);
			integer distance =
				searchNearest(
					nearestSet, 
					Point(0, 0)).first;
			REQUIRE(distance == 0);
		}
		{
			Vector2 timeInterval = { (real)i, (real)i + 1 };
			auto nearestSet = kdTreeNearestSet(tree, PASTEL_TAG(intervalSequence), timeInterval);
			integer distance =
				searchNearest(
					nearestSet, 
					Point(0, 0)).first;
			REQUIRE(distance == square(i));
		}
	}

	for (integer i = 0; i < n; ++i)
	{
		Vector4 timeInterval = { (real)i, (real)i + 1, (real)i + 5, (real)i + 6 };
		{
			auto nearestSet = kdTreeNearestSet(tree, PASTEL_TAG(intervalSequence), timeInterval);
			real distance =
				searchNearest(
					nearestSet, 
					Point(i + 2, 0)).first;

			REQUIRE((integer)distance == square(2));
		}

		{
			auto nearestSet = kdTreeNearestSet(tree, PASTEL_TAG(intervalSequence), timeInterval);
			real distance =
				searchNearest(
					nearestSet, 
					Point(i - 3, 0)).first;

			REQUIRE((integer)distance == square(3));
		}

		{
			auto nearestSet = kdTreeNearestSet(tree, PASTEL_TAG(intervalSequence), timeInterval);
			real distance =
				searchNearest(
					nearestSet, 
					Point(i + 4, 0)).first;
			integer correct = i < (n - 5) ? square(1) : square(4);

			REQUIRE((integer)distance == correct);
		}

		{
			auto nearestSet = kdTreeNearestSet(tree, PASTEL_TAG(intervalSequence), timeInterval);
			real distance =
				searchNearest(
					nearestSet, 
					Point(i + 7, 0)).first;
			integer correct = i < (n - 5) ? square(2) : square(7);

			REQUIRE((integer)distance == correct);
		}
	}
}
