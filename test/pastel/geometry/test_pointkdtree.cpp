// Description: Testing for PointKdTree
// Documentation: pointkdtree.txt

#include "test/test_init.h"

#include "pastel/geometry/count_nearest.h"
#include "pastel/geometry/search_nearest.h"
#include "pastel/geometry/nearestset/kdtree_nearestset.h"
#include "pastel/geometry/splitrule/slidingmidpoint_splitrule.h"
#include "pastel/geometry/bestfirst_pointkdtree_searchalgorithm.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/math/sampling/uniform_sampling.h"

#include "pastel/sys/iterator.h"
#include "pastel/sys/output.h"
#include "pastel/sys/locator.h"
#include "pastel/sys/indicator.h"

namespace
{

	template <integer N_>
	class Settings
	{
	public:
		using Real = real;
		static constexpr integer N = N_;
		using Locator = Vector_Locator<Real, N_>;
	};

	PASTEL_CONCEPT_CHECK(Settings<2>, PointKdTree_Settings_Concept);

	using Tree = PointKdTree<Settings<2>>;
	using Point_ConstIterator = Tree::Point_ConstIterator;

	PASTEL_CONCEPT_CHECK(Tree::Point, Point_Concept);

	template <typename SearchAlgorithm_PointKdTree>
	void testSearch(
		SearchAlgorithm_PointKdTree searchAlgorithm)
	{
		Euclidean_NormBijection<real> normBijection;

		/*
			0   |
				|2  3
			1  | 4
				|5
		--6--+--7---
				|  8
		9ABC D
				|    E
				|
		*/

		std::vector<Vector2> pointSet;
		// 0
		pointSet.push_back(
			Vector2(-4, 4));
		// 1
		pointSet.push_back(
			Vector2(-3, 2));
		// 2
		pointSet.push_back(
			Vector2(1, 3));
		// 3
		pointSet.push_back(
			Vector2(4, 3));
		// 4
		pointSet.push_back(
			Vector2(2, 2));
		// 5
		pointSet.push_back(
			Vector2(1, 1));
		// 6
		pointSet.push_back(
			Vector2(-3, 0));
		// 7
		pointSet.push_back(
			Vector2(3, 0));
		// 8
		pointSet.push_back(
			Vector2(3, -1));
		// 9
		pointSet.push_back(
			Vector2(-5, -2));
		// A
		pointSet.push_back(
			Vector2(-4, -2));
		// B
		pointSet.push_back(
			Vector2(-3, -2));
		// C
		pointSet.push_back(
			Vector2(-2, -2));
		// D
		pointSet.push_back(
			Vector2(0, -2));
		// E
		pointSet.push_back(
			Vector2(5, -3));

		std::vector<Point_ConstIterator> iteratorSet;

		Tree tree;
		tree.insertSet(
			pointSet,
			PASTEL_TAG(report), pushBackOutput(iteratorSet)
		);
		REQUIRE(testInvariants(tree));

		tree.refine(SlidingMidpoint_SplitRule(), 1);
		REQUIRE(testInvariants(tree));

		/*
			0   |
				|2  3
			1  | 4
				|5
		--6--+--7---
				|  8
		9ABC D
				|    E
				|
		*/

		/*
		Nearest neighbors:
		0 -> 1 (1^2 + 2^2 = 5)
		1 -> 6 (0^2 + 2^2 = 4)
		2 -> 4 (1^2 + 1^2 = 2)
		3 -> 4 (2^2 + 1^2 = 5)
		4 -> 2 or 5 (1^2 + 1^2 = 2)
		5 -> 4 (1^2 + 1^2 = 2)
		6 -> 1 or B (0^2 + 2^2 = 4)
		7 -> 8 (0^2 + 1^2 = 1)
		8 -> 7 (0^2 + 1^2 = 1)
		9 -> A (1^2 + 0^2 = 1)
		A -> 9 or B (1^2 + 0^2 = 1)
		B -> A or C (1^2 + 0^2 = 1)
		C -> B (1^2 + 0^2 = 1)
		D -> C (2^2 + 0^2 = 4)
		E -> 8 (2^2 + 2^2 = 8)
		*/

		std::vector<Point_ConstIterator> correctSet;
		correctSet.push_back(iteratorSet[1]);
		correctSet.push_back(iteratorSet[6]);
		correctSet.push_back(iteratorSet[4]);
		correctSet.push_back(iteratorSet[4]);
		correctSet.push_back(iteratorSet[2]);
		correctSet.push_back(iteratorSet[4]);
		correctSet.push_back(iteratorSet[1]);
		correctSet.push_back(iteratorSet[8]);
		correctSet.push_back(iteratorSet[7]);
		correctSet.push_back(iteratorSet[10]);
		correctSet.push_back(iteratorSet[9]);
		correctSet.push_back(iteratorSet[10]);
		correctSet.push_back(iteratorSet[11]);
		correctSet.push_back(iteratorSet[12]);
		correctSet.push_back(iteratorSet[8]);

		std::vector<real> distanceSet;
		distanceSet.push_back(5);
		distanceSet.push_back(4);
		distanceSet.push_back(2);
		distanceSet.push_back(5);
		distanceSet.push_back(2);
		distanceSet.push_back(2);
		distanceSet.push_back(4);
		distanceSet.push_back(1);
		distanceSet.push_back(1);
		distanceSet.push_back(1);
		distanceSet.push_back(1);
		distanceSet.push_back(1);
		distanceSet.push_back(1);
		distanceSet.push_back(4);
		distanceSet.push_back(8);

		for (integer i = 0;i < iteratorSet.size();++i)
		{
			{
				std::pair<real, Point_ConstIterator> result = 
					searchNearest(
						kdTreeNearestSet(tree), 
						iteratorSet[i]->point(), 
						normBijection, 
						PASTEL_TAG(searchAlgorithm), searchAlgorithm,
						PASTEL_TAG(nBruteForce), 1);

				real distance2 = result.first;
				Point_ConstIterator iter = result.second;
				unused(iter);

				//REQUIRE(iter == iteratorSet[i]);
				REQUIRE(distance2 == 0);
			}

			{
				std::pair<real, Point_ConstIterator> result = 
					searchNearest(
						kdTreeNearestSet(tree), 
						iteratorSet[i]->point(),
						PASTEL_TAG(accept), predicateIndicator(iteratorSet[i], NotEqualTo()),
						normBijection, 
						PASTEL_TAG(searchAlgorithm), searchAlgorithm,
						PASTEL_TAG(nBruteForce), 1);

				real distance2 = result.first;
				Point_ConstIterator iter = result.second;
				unused(iter);

				//REQUIRE(iter == correctSet[i]);
				REQUIRE(distance2 == distanceSet[i]);
			}
		}
	}

}

TEST_CASE("Types (Types)")
{
	PASTEL_STATIC_ASSERT(IsPointKdTree<Tree>::value);
	PASTEL_STATIC_ASSERT(IsPointKdTree<const Tree>::value);
	PASTEL_STATIC_ASSERT(IsPointKdTree<const Tree&>::value);
	PASTEL_STATIC_ASSERT(IsPointKdTree<const Tree&&>::value);
	PASTEL_STATIC_ASSERT(!IsPointKdTree<int>::value);
}

TEST_CASE("various (PointKdTree)")
{
	integer m = 10000;

	std::vector<Vector2> pointSet;
	pointSet.reserve(m);
	for (integer i = 0;i < m;++i)
	{
		pointSet.push_back(2 * randomVectorBall<real, 2>());
	}

	std::vector<Point_ConstIterator> iteratorSet;

	Tree tree;
	tree.insertSet(
		pointSet,
		PASTEL_TAG(report), pushBackOutput(iteratorSet)
	);
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.points() == m);
	REQUIRE(tree.leaves() == 1);
	REQUIRE(tree.nodes() == 1);

	tree.refine(SlidingMidpoint_SplitRule());
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.points() == m);

	{
		Tree bTree(tree);
		REQUIRE(testInvariants(bTree));
		REQUIRE(equivalent(tree, bTree));

		tree.swap(bTree);
		REQUIRE(testInvariants(bTree));
		REQUIRE(equivalent(tree, bTree));
	}

	tree.merge();
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.leaves() == 1);
	REQUIRE(tree.nodes() == 1);
	REQUIRE(tree.points() == m);

	tree.refine(SlidingMidpoint_SplitRule());
	REQUIRE(testInvariants(tree));

	tree.merge(tree.root());
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.leaves() == 1);
	REQUIRE(tree.nodes() == 1);
	REQUIRE(tree.points() == m);

	tree.refine(SlidingMidpoint_SplitRule());
	REQUIRE(testInvariants(tree));

	tree.hide();
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.points() == 0);

	tree.show();
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.points() == m);

	tree.erase();
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.points() == 0);

	tree.insertSet(
		pointSet,
		PASTEL_TAG(report), pushBackOutput(iteratorSet)
	);
	REQUIRE(testInvariants(tree));

	tree.clear();
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.points() == 0);
	REQUIRE(tree.leaves() == 1);
	REQUIRE(tree.nodes() == 1);
	{
		Tree emptyTree;
		REQUIRE(equivalent(tree, emptyTree));
	}
}

TEST_CASE("Empty (PointKdTree)")
{
	Tree tree;
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.empty());
	REQUIRE(tree.points() == 0);
	REQUIRE(tree.bound().empty());
	REQUIRE(tree.leaves() == 1);
	REQUIRE(tree.nodes() == 1);

	tree.clear();
	REQUIRE(testInvariants(tree));
	REQUIRE(tree.empty());
	REQUIRE(tree.points() == 0);
	REQUIRE(tree.bound().empty());
	REQUIRE(tree.leaves() == 1);
	REQUIRE(tree.nodes() == 1);

	{
		Tree bTree(tree);
		REQUIRE(testInvariants(tree));
		REQUIRE(equivalent(tree, bTree));

		tree.swap(bTree);
		REQUIRE(testInvariants(tree));
		REQUIRE(equivalent(tree, bTree));
	}
}

namespace
{

	template <integer N>
	void testCircular()
	{
		using Tree = PointKdTree<Settings<N>>;
		using Point_ConstIterator = typename Tree::Point_ConstIterator;

		integer m = 10000;

		std::vector<Vector<real, N> > pointSet;
		pointSet.reserve(m);
		for (integer i = 0;i < m;++i)
		{

			pointSet.push_back(2 * randomVectorSphere<real, N>());
		}

		Tree tree;

		tree.insertSet(pointSet);
		tree.refine(SlidingMidpoint_SplitRule());

		Euclidean_NormBijection<real> normBijection;

		{
			std::vector<Point_ConstIterator> neighborSet;
			std::vector<real> distanceSet;

			auto report = [&](
				real distance,
				Point_ConstIterator point)
			{
				distanceSet.push_back(distance);
				neighborSet.push_back(point);
			};

			searchNearest(
				kdTreeNearestSet(addConst(tree)), 
				Vector<real, N>(0), 
				PASTEL_TAG(report), report,
				PASTEL_TAG(kNearest), m);

			integer count = distanceSet.size();

			REQUIRE(count == m);
			REQUIRE(neighborSet.size() == m);
			REQUIRE(distanceSet.size() == m);

			for (integer i = 0;i < count;++i)
			{
				REQUIRE(relativeError<real>(distanceSet[i], 
					normBijection.toBijection(2)) < 0.001);
			}
		}
		{
			integer outerCount = countNearest(
				kdTreeNearestSet(tree), 
				Vector<real, N>(0),
				PASTEL_TAG(maxDistance2), normBijection.toBijection(2.001));
			REQUIRE(outerCount == m);

			integer innerCount = countNearest(
				kdTreeNearestSet(tree), 
				Vector<real, N>(0),
				PASTEL_TAG(maxDistance2), normBijection.toBijection(1.999));
			REQUIRE(innerCount == 0);
		}
	}
}

TEST_CASE("PointKdTree (PointKdTree)")
{
	testCircular<2>();
	testCircular<3>();
	testSearch(
		DepthFirst_SearchAlgorithm_PointKdTree());
	testSearch(
		BestFirst_SearchAlgorithm_PointKdTree());
}
