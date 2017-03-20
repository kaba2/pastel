// Description: Testing for nearest-set.
// DocumentationOf: nearestset_concept.h

#include "test/test_init.h"

#include "pastel/geometry/search_nearest.h"
#include "pastel/geometry/kdtree_nearestset.h"
#include "pastel/geometry/nearestset/bruteforce_nearestset.h"

#include "pastel/geometry/splitrule/slidingmidpoint_splitrule.h"
#include "pastel/geometry/bestfirst_pointkdtree_searchalgorithm.h"
#include "pastel/geometry/pointkdtree.h"

#include <pastel/math/normbijection/normbijections.h>

#include <pastel/sys/vector.h>
#include <pastel/sys/set.h>
#include <pastel/sys/output.h>
#include <pastel/sys/indicator.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/iterator/counting_iterator.h>

#include <vector>
#include <list>

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

	using Tree = PointKdTree<Settings<2>>;
	using Point_ConstIterator = Tree::Point_ConstIterator;

	PASTEL_CONCEPT_CHECK(Tree::Point, Point_Concept);

	template <typename NearestSet>
	void test(
		const NearestSet& nearestSet,
		const std::vector<real>& distanceSet)
	{
		PASTEL_CONCEPT_CHECK(NearestSet, NearestSet_Concept);

		Euclidean_NormBijection<real> normBijection;

		for(auto&& i : nearestSet.pointSet())
		{
			auto result =
				searchNearest(nearestSet, nearestSet.asPoint(i));

			real distance2 = result.first;

			REQUIRE(distance2 == 0);
		}

		integer j = 0;
		for (auto&& i : nearestSet.pointSet())
		{
			auto indicator = predicateIndicator(i, NotEqualTo());
			PASTEL_CONCEPT_CHECK(decltype(indicator), Indicator_Concept(decltype(i)));

			auto result =
				searchNearest(
					nearestSet,
					nearestSet.asPoint(i),
					PASTEL_TAG(accept), indicator,
					normBijection
				);

			real distance2 = result.first;

			REQUIRE(distance2 == distanceSet[j]);
			++j;
		}
	}

}

TEST_CASE("Small (search_nearest_bruteforce)")
{
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

	using PointSet = std::list<Vector2>;
	using Point_Iterator = PointSet::iterator;
	using Locator = Vector_Locator<real, 2>;

	PointSet pointSet =
	{
		// 0
		{-4, 4},
		// 1
		{ -3, 2 },
		// 2
		{ 1, 3 },
		// 3
		{ 4, 3 },
		// 4
		{ 2, 2 },
		// 5
		{ 1, 1 },
		// 6
		{ -3, 0 },
		// 7
		{ 3, 0 },
		// 8
		{ 3, -1 },
		// 9
		{ -5, -2 },
		// A
		{ -4, -2 },
		// B
		{ -3, -2 },
		// C
		{ -2, -2 },
		// D
		{ 0, -2 },
		// E
		{ 5, -3 }
	};

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

	std::vector<real> distanceSet =
	{
		5, 4, 2, 5, 2, 2, 4, 1, 1, 1, 1, 1, 1, 4, 8
	};

	{
		auto nearestSet = bruteForceNearestSet(pointSet);
		test(nearestSet, distanceSet);
	}
	{
		Tree tree;
		tree.insertSet(pointSet);
		REQUIRE(testInvariants(tree));

		tree.refine(SlidingMidpoint_SplitRule(), 1);
		REQUIRE(testInvariants(tree));

		test(kdTreeNearestSet(tree), distanceSet);
	}
}

namespace
{

	template <typename SearchAlgorithm_PointKdTree>
	void testPointKdTree(
		SearchAlgorithm_PointKdTree searchAlgorithm)
	{
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

		auto nearestSet = kdTreeNearestSet(tree);

		for (integer i = 0;i < iteratorSet.size();++i)
		{
			{
				std::pair<real, Point_ConstIterator> result = 
					searchNearest(
						nearestSet, 
						pointSet[i], 
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
						nearestSet, 
						pointSet[i], 
						PASTEL_TAG(accept), predicateIndicator(iteratorSet[i], NotEqualTo()),
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

TEST_CASE("BruteForce (search_nearest_bruteforce)")
{
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

	using PointSet = std::list<Vector2>;
	using Point_Iterator = PointSet::iterator;
	using Point_ConstIterator = PointSet::const_iterator;
	using Locator = Vector_Locator<real, 2>;

	PointSet pointSet =
	{
		// 0
		{-4, 4},
		// 1
		{ -3, 2 },
		// 2
		{ 1, 3 },
		// 3
		{ 4, 3 },
		// 4
		{ 2, 2 },
		// 5
		{ 1, 1 },
		// 6
		{ -3, 0 },
		// 7
		{ 3, 0 },
		// 8
		{ 3, -1 },
		// 9
		{ -5, -2 },
		// A
		{ -4, -2 },
		// B
		{ -3, -2 },
		// C
		{ -2, -2 },
		// D
		{ 0, -2 },
		// E
		{ 5, -3 }
	};

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

	std::vector<real> distanceSet =
	{
		5, 4, 2, 5, 2, 2, 4, 1, 1, 1, 1, 1, 1, 4, 8
	};

	auto aNearestSet = bruteForceNearestSet(
		pointSet);

	PASTEL_CONCEPT_CHECK(decltype(aNearestSet), NearestSet_Concept);

	auto bNearestSet = 
		bruteForceNearestSet(
			locationSet(
				intervalSet(pointSet.begin(), pointSet.end()), 
				indirectLocator<Point_Iterator>(Locator())
			)
		);

	PASTEL_CONCEPT_CHECK(decltype(bNearestSet), NearestSet_Concept);

	integer j = 0;
	for (auto i = pointSet.begin(); i != pointSet.end(); ++i)
	{
		{
			std::pair<real, Point_ConstIterator> result =
				searchNearest(addConst(aNearestSet), *i);

			real distance2 = result.first;

			REQUIRE(distance2 == 0);
		}

		{
			auto result =
				searchNearest(addConst(bNearestSet), *i);

			real distance2 = result.first;

			REQUIRE(distance2 == distanceSet[j]);
		}
		++j;
	}
}

TEST_CASE("search_nearest_bruteforce (search_nearest_bruteforce)")
{
	testPointKdTree(
		DepthFirst_SearchAlgorithm_PointKdTree());
	testPointKdTree(
		BestFirst_SearchAlgorithm_PointKdTree());
}
