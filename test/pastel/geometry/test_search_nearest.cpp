// Description: Testing for nearest-set.
// DocumentationOf: nearestset_concept.h

#include "test/test_init.h"

#include "pastel/geometry/search_nearest.h"
#include "pastel/geometry/kdtree_nearestset.h"
#include "pastel/geometry/nearestset/bruteforce_nearestset.h"

#include "pastel/geometry/splitrule/slidingmidpoint_splitrule.h"
#include "pastel/geometry/bestfirst_pointkdtree_searchalgorithm.h"
#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/tdtree/tdtree.h"

#include <pastel/math/normbijection/normbijections.h>

#include <pastel/sys/vector.h>
#include <pastel/sys/set.h>
#include <pastel/sys/output.h>
#include <pastel/sys/indicator.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/iterator/counting_iterator.h>

#include <vector>
#include <list>
#include <map>

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

	template <typename NearestSet>
	void test(
		const NearestSet& nearestSet,
		const std::vector<real>& distanceSet)
	{
		PASTEL_CONCEPT_CHECK(NearestSet, NearestSet_Concept);

		Euclidean_NormBijection<real> normBijection;

		for(auto&& i : nearestSet)
		{
			auto result =
				searchNearest(nearestSet, nearestSet.asPoint(i));

			real distance2 = result.first;

			REQUIRE(distance2 == 0);
		}

		PASTEL_CONCEPT_CHECK(decltype(nearestSet), Set_Concept);

		REQUIRE(setSize(nearestSet) == distanceSet.size());

		integer j = 0;
		for (auto&& i : nearestSet)
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

class CreatePointKdTree
{
public:
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	decltype(auto) createDataSet(
		const PointSet& pointSet) const
	{
		using Tree = PointKdTree<Settings<2>>;
		using Point_ConstIterator = Tree::Point_ConstIterator;

		PASTEL_CONCEPT_CHECK(Tree::Point, Point_Concept);

		std::map<Tree::Point_ConstIterator, integer> iteratorSet;
		integer n = 0;

		Tree tree;
		tree.insertSet(
			pointSet, 
			PASTEL_TAG(report),
			[&](auto&& i)
			{
				iteratorSet[i] = n; 
				++n;
			}
		);

		REQUIRE(tree.points() == pointSet.size());
		REQUIRE(testInvariants(tree));

		tree.refine(SlidingMidpoint_SplitRule(), 1);
		REQUIRE(testInvariants(tree));

		std::vector<integer> permutationSet;
		for (auto&& point : intervalSet(tree))
		{
			permutationSet.push_back(iteratorSet[point]);
		}

		return std::make_pair(
			std::move(tree), 
			std::move(permutationSet));
	}

	template <typename DataSet>
	decltype(auto) createNearestSet(const DataSet& dataSet) const
	{
		return kdTreeNearestSet(dataSet);
	}
};

class CreateTdTree
{
public:
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	decltype(auto) createDataSet(
		const PointSet& pointSet) const
	{
		using Settings = TdTree_Settings<Default_Locator<Vector2, real, 2>>;
		using Tree = TdTree<Settings>;
		using Point_ConstIterator = Tree::Point_ConstIterator;

		PASTEL_CONCEPT_CHECK(Tree::Point, Point_Concept);
		
		Tree tree(pointSet);

		//Tree tree(
		//	pointSet,
		//	PASTEL_TAG(splitRule),
		//	SlidingMidpoint_SplitRule());

		REQUIRE(tree.points() == pointSet.size());
		//REQUIRE(testInvariants(tree));

		integer n = 0;
		std::vector<integer> permutationSet;
		for (auto&& point : pointSet)
		{
			permutationSet.push_back(n);
			++n;
		}

		return std::make_pair(
			std::move(tree), 
			std::move(permutationSet));
	}

	template <typename DataSet>
	decltype(auto) createNearestSet(const DataSet& dataSet) const
	{
		return kdTreeNearestSet(dataSet);
	}
};

class CreateBruteForce
{
public:
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	decltype(auto) createDataSet(
		PointSet&& pointSet) const
	{
		std::vector<integer> permutationSet;
		integer i = 0;
		for (auto&& point : pointSet)
		{
			permutationSet.push_back(i);
			++i;
		}

		return std::make_pair(
			std::move(pointSet), 
			std::move(permutationSet));
	}

	template <typename DataSet>
	decltype(auto) createNearestSet(const DataSet& dataSet) const
	{
		return bruteForceNearestSet(dataSet);
	}
};

template <typename Create>
void testCase(const Create& create)
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

	auto result = create.createDataSet(pointSet);
	const auto& dataSet = result.first;
	const std::vector<integer>& permutationSet = result.second;
	
	auto nearestSet = create.createNearestSet(dataSet);

	std::vector<real> reorderedDistanceSet;
	for (integer i = 0;i < permutationSet.size();++i)
	{
		reorderedDistanceSet.push_back(
			distanceSet[permutationSet[i]]);
	}
	
	test(nearestSet, reorderedDistanceSet);
}

TEST_CASE("search_nearest (PointKdTree)")
{
	testCase(CreatePointKdTree());
}

TEST_CASE("search_nearest (brute-force)")
{
	testCase(CreateBruteForce());
}

TEST_CASE("search_nearest (TdTree)")
{
	testCase(CreateTdTree());
}

//TEST_CASE("search_nearest_bruteforce (search_nearest_bruteforce)")
//{
//	testPointKdTree(
//		DepthFirst_SearchAlgorithm_PointKdTree());
//	testPointKdTree(
//		BestFirst_SearchAlgorithm_PointKdTree());
//}
