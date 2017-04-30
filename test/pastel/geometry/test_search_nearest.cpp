// Description: Testing for nearest-set.
// DocumentationOf: nearestset_concept.h

#include "test/test_init.h"


#include "pastel/geometry/search_nearest.h"
#include "pastel/geometry/nearestset/kdtree_nearestset.h"
#include "pastel/geometry/nearestset/bruteforce_nearestset.h"

#include "pastel/geometry/splitrule/slidingmidpoint_splitrule.h"
#include "pastel/geometry/bestfirst_pointkdtree_searchalgorithm.h"
#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/tdtree/tdtree.h"
#include "pastel/geometry/pointkdtree/pointkdtree_equivalent.h"

#include <pastel/math/normbijection/normbijections.h>
#include <pastel/sys/random.h>

#include "pastel/sys/vector.h"
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

	template <typename Cursor>
	void printTree(const Cursor& cursor, integer depth = 0)
	{
		if (!cursor || cursor.left() == cursor)
		{
			return;
		}

		for (integer i = 0;i < depth; ++i)	
		{
			std::cout << "  ";
		}
		std::cout << cursor.min() << " : " << cursor.max() << std::endl;
		printTree(cursor.left(), depth + 1);
		printTree(cursor.right(), depth + 1);
	}

	template <
		typename Create,
		typename PointSet>
	void test(
		const Create& create,
		const PointSet& pointSet,
		const std::vector<real>& distanceSet)
	{
		auto result = create.createDataSet(pointSet);
		const auto& dataSet = result.first;
		const std::vector<integer>& permutationSet = result.second;
		
		auto nearestSet = create.createNearestSet(dataSet);
		REQUIRE(setSize(nearestSet) == distanceSet.size());

		std::vector<real> reorderedDistanceSet;
		for (integer i = 0;i < permutationSet.size();++i)
		{
			reorderedDistanceSet.push_back(
				distanceSet[permutationSet[i]]);
		}

		Euclidean_NormBijection<real> normBijection;

		integer j = 0;
		RANGES_FOR(auto&& i, nearestSet)
		{
			auto result =
				searchNearest(
					nearestSet,
					nearestSet.asPoint(i),
					PASTEL_TAG(accept), predicateIndicator(i, NotEqualTo()),
					normBijection
				);

			real distance2 = result.first;
			REQUIRE(distance2 == reorderedDistanceSet[j]);

			++j;
		}
	}

	template <
		typename Create,
		typename PointSet>
	void testCount(
		const Create& create,
		const PointSet& pointSet,
		const real maxDistance2,
		const std::vector<integer>& countSet)
	{
		auto result = create.createDataSet(pointSet);
		const auto& dataSet = result.first;

		const std::vector<integer>& permutationSet = result.second;
		
		auto nearestSet = create.createNearestSet(dataSet);
		REQUIRE(setSize(nearestSet) == countSet.size());

		std::vector<integer> reorderedCountSet;
		for (integer i = 0;i < permutationSet.size();++i)
		{
			reorderedCountSet.push_back(
				countSet[permutationSet[i]]);
		}

		Euclidean_NormBijection<real> normBijection;

		integer j = 0;
		RANGES_FOR(auto&& i, nearestSet)
		{
			integer count = 0;
			auto report = [&](auto&&, auto&&){++count;};

			auto result =
				searchNearest(
					nearestSet,
					nearestSet.asPoint(i),
					PASTEL_TAG(report), report, 
					PASTEL_TAG(counting), true,
					PASTEL_TAG(maxDistance2), maxDistance2,
					PASTEL_TAG(accept), predicateIndicator(i, NotEqualTo()),
					normBijection
				);

			REQUIRE(count == reorderedCountSet[j]);
			++j;
		}
	}

}

template <typename SearchAlgorithm>
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
		using Locator = PointSet_Locator<PointSet>;
		using Settings = PointKdTree_Settings<Locator>;
		using Tree = PointKdTree<Settings>;
		using Point_ConstIterator = typename Tree::Point_ConstIterator;

		PASTEL_CONCEPT_CHECK(typename Tree::Point, Point_Concept);

		std::map<typename Tree::Point_ConstIterator, integer> iteratorSet;
		integer n = 0;

		Tree tree(Locator(), true);
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

		//printTree(tree.root());

		// Tree copyTree(tree);
		// REQUIRE(testInvariants(copyTree));

		//printTree(copyTree.root());

		std::vector<integer> permutationSet;
		RANGES_FOR(auto&& point, intervalSet(tree))
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
		return kdTreeNearestSet(
			dataSet,
			PASTEL_TAG(nBruteForce), 1,
			PASTEL_TAG(searchAlgorithm),
			SearchAlgorithm());
	}
};

template <typename SearchAlgorithm>
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
		using Settings = TdTree_Settings<PointSet_Locator<PointSet>>;
		using Tree = TdTree<Settings>;
		using Point_ConstIterator = typename Tree::Point_ConstIterator;

		PASTEL_CONCEPT_CHECK(typename Tree::Point, Point_Concept);
		
		Tree tree(
			pointSet,
			PASTEL_TAG(splitRule),
			SlidingMidpoint_SplitRule());

		//printTree(tree.root());

		REQUIRE(tree.points() == pointSet.size());
		REQUIRE(testInvariants(tree));

		integer n = 0;
		std::vector<integer> permutationSet;
		RANGES_FOR(auto&& point, pointSet)
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
		return kdTreeNearestSet(
			dataSet,
			PASTEL_TAG(nBruteForce), 1,
			PASTEL_TAG(searchAlgorithm),
			SearchAlgorithm());
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
		const PointSet& pointSet) const
	{
		std::vector<integer> permutationSet;
		integer i = 0;
		RANGES_FOR(auto&& point, pointSet)
		{
			permutationSet.push_back(i);
			++i;
		}

		return std::make_pair(
			pointSet, 
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
	test(create, pointSet, distanceSet);

	std::vector<integer> countSet =
	{
		0, 0, 1, 0, 2, 1, 0, 1, 1, 1, 2, 2, 1, 0, 0
	};
	testCount(create, pointSet, 2.25, countSet);
}

//TEST_CASE("search_nearest (PointKdTree)")
//{
//	testCase(CreatePointKdTree<DepthFirst_SearchAlgorithm_PointKdTree>());
//	testCase(CreatePointKdTree<BestFirst_SearchAlgorithm_PointKdTree>());
//}

TEST_CASE("search_nearest (brute-force)")
{
	testCase(CreateBruteForce());
}

TEST_CASE("search_nearest (TdTree)")
{
	testCase(CreateTdTree<DepthFirst_SearchAlgorithm_PointKdTree>());
	testCase(CreateTdTree<BestFirst_SearchAlgorithm_PointKdTree>());
}

template <
	typename Range,
	typename A_Create,
	typename B_Create>
void testGaussian(
	const Range& pointSet,
	const A_Create& aCreate,
	const B_Create& bCreate)
{
	const integer n = setSize(pointSet);

	auto aDataSet = aCreate.createDataSet(pointSet).first;
	auto aNearestSet = aCreate.createNearestSet(aDataSet);
	using A_NearestSet = decltype(aNearestSet);
	using A_Point = PointSet_Point<A_NearestSet>;
	PASTEL_CONCEPT_CHECK(RemoveCvRef<decltype(aNearestSet.asPoint(A_Point()))>, Point_Concept);
	
	auto bDataSet = bCreate.createDataSet(pointSet).first;
	auto bNearestSet = bCreate.createNearestSet(bDataSet);
	using B_NearestSet = decltype(bNearestSet);
	using B_Point = PointSet_Point<B_NearestSet>;

	integer k = 7;

	auto normBijection = Maximum_NormBijection<real>();

	REQUIRE(pointSet.size() == n);

	std::vector<std::pair<real, A_Point>> aSet;
	aSet.reserve(k);

	std::vector<std::pair<real, B_Point>> bSet;
	bSet.reserve(k);

	integer equalDistances = 0;
	for (integer i = 0; i < n; ++i)
	{
		aSet.clear();
		real kDistanceA = searchNearest(
			aNearestSet,
			pointSet[i],
			PASTEL_TAG(report), emplaceBackOutput(aSet),
			PASTEL_TAG(kNearest), k,
			PASTEL_TAG(normBijection), normBijection,
			PASTEL_TAG(reportMissing)
			).first;
		REQUIRE(aSet.size() == k);

		bSet.clear();
		real kDistanceB = searchNearest(
			bNearestSet,
			pointSet[i],
			PASTEL_TAG(report), emplaceBackOutput(bSet),
			PASTEL_TAG(kNearest), k,
			PASTEL_TAG(normBijection), normBijection,
			PASTEL_TAG(reportMissing)
		).first;
		REQUIRE(bSet.size() == k);

		bool correct = true;
		for (integer j = 0; j < k; ++j)
		{
			if (aSet[j].first != bSet[j].first)
			{
				correct = false;
				break;
			}
		}

		if (correct)
		{
			++equalDistances;
		}
	}
	
	REQUIRE(equalDistances == n);
}

TEST_CASE("search_nearest gaussian tdtree")
{
	auto aCreate = CreateBruteForce();

	static constexpr integer N = 3;
	using PointSet = std::vector<Vector<real, N>>;

	integer n = 1000;
	PointSet pointSet;
	pointSet.reserve(n);

	for (integer i = 0; i < n; ++i)
	{
		pointSet.emplace_back(
			randomGaussianVector<real, N>());
	}

	{
		auto bCreate = CreateTdTree<DepthFirst_SearchAlgorithm_PointKdTree>();
		testGaussian(pointSet, aCreate, bCreate);
	}
	{
		auto bCreate = CreateTdTree<BestFirst_SearchAlgorithm_PointKdTree>();
		testGaussian(pointSet, aCreate, bCreate);
	}
	{
		auto bCreate = CreatePointKdTree<DepthFirst_SearchAlgorithm_PointKdTree>();
		testGaussian(pointSet, aCreate, bCreate);
	}
	{
		auto bCreate = CreatePointKdTree<BestFirst_SearchAlgorithm_PointKdTree>();
		testGaussian(pointSet, aCreate, bCreate);
	}
}
