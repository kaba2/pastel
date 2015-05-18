// Description: Testing for nearest-set.
// DocumentationOf: nearestset_concept.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/search_nearest_bruteforce.h>

#include "pastel/geometry/pointkdtree/pointkdtree_count_nearest.h"
#include "pastel/geometry/pointkdtree/pointkdtree_search_nearest.h"
#include "pastel/geometry/splitrule/slidingmidpoint_splitrule.h"
#include "pastel/geometry/bestfirst_pointkdtree_searchalgorithm.h"

#include <pastel/math/normbijection/normbijections.h>

#include <pastel/sys/vector.h>
#include <pastel/sys/input.h>
#include <pastel/sys/output.h>
#include <pastel/sys/indicator.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/iterator/counting_iterator.h>

#include <vector>
#include <list>

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

		template <integer N_>
		class Settings
		{
		public:
			using Real = real;
			static PASTEL_CONSTEXPR integer N = N_;
			using Locator = Vector_Locator<Real, N_>;
		};

		using Tree = PointKdTree<Settings<2>>;
		using Point_ConstIterator = Tree::Point_ConstIterator;
			
		PASTEL_CONCEPT_CHECK(Tree::Point, Point_Concept);

		virtual void run()
		{
			testBruteForce();
			testPointKdTree(
				DepthFirst_SearchAlgorithm_PointKdTree());
			testPointKdTree(
				BestFirst_SearchAlgorithm_PointKdTree());

			{
				auto createNearestSet = [](auto pointSet)
				{
					return bruteForceNearestSet(rangeInput(pointSet));
				};

				test(createNearestSet);
			}

			/*
			{
				auto createNearestSet = [&](auto pointSet)
				{
					Tree tree;
					tree.insertRange(
						range(pointSet.begin(), pointSet.end()),
						nullOutput());
					TEST_ENSURE(testInvariants(tree));

					tree.refine(SlidingMidpoint_SplitRule(), 1);
					TEST_ENSURE(testInvariants(tree));

					return tree;
				};

				test(createNearestSet);
			}
			*/
		}

		template <typename Create_NearestSet>
		void test(
			Create_NearestSet createNearestSet)
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

			auto nearestSet = createNearestSet(pointSet);

			using NearestSet = decltype(nearestSet);
			PASTEL_CONCEPT_CHECK(NearestSet, NearestSet_Concept);

			Euclidean_NormBijection<real> normBijection;

			integer j = 0;
			for (auto i = pointSet.begin(); i != pointSet.end();++i)
			{
				{
					std::pair<real, NearestSet_Point<NearestSet>> result =
						searchNearest(addConst(nearestSet), *i);

					real distance2 = result.first;

					TEST_ENSURE(distance2 == 0);
				}

				/*
				{

					auto result =
						searchNearest(
							addConst(nearestSet),
							*i,
							Null_Output(),
							predicateIndicator(i, NotEqualTo()),
							normBijection
						);

					real distance2 = result.first;

					TEST_ENSURE(distance2 == distanceSet[j]);
				}
				*/
				++j;
			}
		
		}

		template <typename SearchAlgorithm_PointKdTree>
		void testPointKdTree(
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
			tree.insertRange(
				range(pointSet.begin(), pointSet.end()),
				pushBackOutput(iteratorSet));
			TEST_ENSURE(testInvariants(tree));

			tree.refine(SlidingMidpoint_SplitRule(), 1);
			TEST_ENSURE(testInvariants(tree));
			
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
						searchNearest(tree, iteratorSet[i], Null_Output(), 
						allIndicator(), normBijection, searchAlgorithm)
						.bucketSize(1);

					real distance2 = result.first;
					Point_ConstIterator iter = result.second;

					//TEST_ENSURE(iter == iteratorSet[i]);
					TEST_ENSURE(distance2 == 0);
				}
				
				{
					std::pair<real, Point_ConstIterator> result = 
						searchNearest(tree, iteratorSet[i], Null_Output(),
						predicateIndicator(iteratorSet[i], NotEqualTo()),
						normBijection, searchAlgorithm)
						.bucketSize(1);
					
					real distance2 = result.first;
					Point_ConstIterator iter = result.second;

					//TEST_ENSURE(iter == correctSet[i]);
					TEST_ENSURE(distance2 == distanceSet[i]);
				}
			}
		}

		void testBruteForce()
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

			auto aNearestSet = bruteForceNearestSet(
				rangeInput(pointSet));

			PASTEL_CONCEPT_CHECK(decltype(aNearestSet), NearestSet_Concept);

			auto bNearestSet = bruteForceNearestSet(
					locationSet(rangeInput(countingRange(pointSet)), indirectLocator<Point_Iterator>(Locator()))
			);

			PASTEL_CONCEPT_CHECK(decltype(bNearestSet), NearestSet_Concept);
			
			integer j = 0;
			for (auto i = pointSet.begin(); i != pointSet.end(); ++i)
			{
				{
					std::pair<real, Vector2> result =
						searchNearest(addConst(aNearestSet), *i);

					real distance2 = result.first;

					TEST_ENSURE(distance2 == 0);
				}

				{

					std::pair<real, Point_Iterator> result =
						searchNearest(
							addConst(bNearestSet),
							*i,
							Null_Output(),
							predicateIndicator(i, NotEqualTo()),
							normBijection
						);

					real distance2 = result.first;

					TEST_ENSURE(distance2 == distanceSet[j]);
				}
				++j;
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("search_nearest_bruteforce", test);
	}

	CallFunction run(addTest);

}
