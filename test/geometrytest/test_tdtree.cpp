// Description: Testing for temporal kd-tree
// DocumentationOf: tdtree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/tdtree.h"

#include "pastel/geometry/pointkdtree_search_nearest.h"
#include "pastel/geometry/search_all_neighbors_bruteforce.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/locators.h"
#include "pastel/sys/inputs.h"
#include "pastel/sys/outputs.h"
#include "pastel/sys/for_each_point.h"
#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/constant_iterator.h"

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

		using Point = Vector2;
		using Locator = Vector_Locator<real, 2>;
		using Tree = TdTree<TdTree_Settings<Locator>>;
		using ConstIterator = Tree::ConstIterator;

		virtual void run()
		{
			test();
			testLinear();
			testGaussian();
		}

		void testGaussian()
		{
			using Point = Vector3;
			using Locator = Vector_Locator<real, 3>;
			using Tree = TdTree<TdTree_Settings<Locator>>;
			using ConstIterator = Tree::ConstIterator;

			using PointSet = std::vector<Point>;
			
			integer n = 1000;
			
			PointSet pointSet;
			pointSet.reserve(n);

			using Point_Iterator = PointSet::iterator;

			for (integer i = 0; i < n; ++i)
			{
				pointSet.emplace_back(
					randomGaussianVector<real, 3>());
			}
			
			Tree tree(rangeInput(pointSet));

			Array<Point_Iterator, 2> nearestSet(
				Vector2i(1, n));

			searchAllNeighborsBruteForce(
				pointSet,
				Locator(),
				nearestSet,
				countingRange(pointSet),
				1,
				constantRange(infinity<real>(), boost::size(pointSet)));

			std::vector<real> distanceSet;
			distanceSet.reserve(n);
			for (integer i = 0; i < n; ++i)
			{
				distanceSet.emplace_back(distance2(*nearestSet(i), pointSet[i]));
			}
		}

		void testLinear()
		{
			std::vector<Point> pointSet;
			integer n = 1000;
			for (integer i = 0; i < n; ++i)
			{
				pointSet.emplace_back(i, 0);
			}

			Tree tree(rangeInput(pointSet));
			TEST_ENSURE(tree.simple());

			auto output = Null_Output();
			auto accept = All_Indicator();
			auto norm = Euclidean_NormBijection<real>();
			auto algorithm = DepthFirst_SearchAlgorithm_PointKdTree();
			
			for (integer i = 0; i < n; ++i)
			{
				{
					Vector2 timeInterval = { (real)i, (real)n };
					integer distance =
						searchNearest(tree, Point(0, 0), output, accept,
						norm, algorithm, timeInterval);
					TEST_ENSURE_OP(distance, ==, square(i));
				}
				{
					Vector2 timeInterval = { (real)0, (real)i + 1 };
					integer distance =
						searchNearest(tree, Point(0, 0), output, accept,
						norm, algorithm, timeInterval);
					TEST_ENSURE_OP(distance, ==, 0);
				}
				{
					Vector2 timeInterval = { (real)i, (real)i + 1 };
					integer distance =
						searchNearest(tree, Point(0, 0), output, accept,
						norm, algorithm, timeInterval);
					TEST_ENSURE_OP(distance, ==, square(i));
				}
			}

			for (integer i = 0; i < n; ++i)
			{
				Vector4 timeInterval = { (real)i, (real)i + 1, (real)i + 5, (real)i + 6 };
				{
					real distance =
						searchNearest(tree, Point(i + 2, 0), output, accept,
						norm, algorithm, timeInterval);

					TEST_ENSURE_OP((integer)distance, ==, square(2));
				}

				{
					real distance =
						searchNearest(tree, Point(i - 3, 0), output, accept,
						norm, algorithm, timeInterval);

					TEST_ENSURE_OP((integer)distance, ==, square(3));
				}

				{
					real distance =
						searchNearest(tree, Point(i + 4, 0), output, accept,
						norm, algorithm, timeInterval);
					integer correct = i < (n - 5) ? square(1) : square(4);

					TEST_ENSURE_OP((integer)distance, ==, correct);
				}

				{
					real distance =
						searchNearest(tree, Point(i + 7, 0), output, accept,
						norm, algorithm, timeInterval);
					integer correct = i < (n - 5) ? square(2) : square(7);

					TEST_ENSURE_OP((integer)distance, ==, correct);
				}
			}
		}

		void test()
		{
			{
				Tree tree;
				TEST_ENSURE(tree.simple());

				tree.clear();
				TEST_ENSURE(tree.simple());

				tree.swap(Tree());
			}

			{
				std::vector<Point> pointSet;
				
				forEachPoint(
					AlignedBox2i(Point(0), Point(5)),
					[&](const Point& point)
				{
					pointSet.emplace_back(point);
				});

				Tree tree(rangeInput(pointSet));
				TEST_ENSURE(tree.simple());
					
				TEST_ENSURE_OP(tree.timeToIndex(-infinity<real>()), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(-2), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(-1), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(-0.5), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(0), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(0.5), ==, 1);
				TEST_ENSURE_OP(tree.timeToIndex(1), ==, 1);
				TEST_ENSURE_OP(tree.timeToIndex(24), ==, 24);
				TEST_ENSURE_OP(tree.timeToIndex(24.5), ==, 25);
				TEST_ENSURE_OP(tree.timeToIndex(25.5), ==, 25);
				TEST_ENSURE_OP(tree.timeToIndex(26), ==, 25);
				TEST_ENSURE_OP(tree.timeToIndex(infinity<real>()), ==, 25);

				std::vector<ConstIterator> neighborSet;

				auto report = [&](
					real distance,
					const ConstIterator& point)
				{
					neighborSet.emplace_back(point);
				};

				searchNearest(tree, Point(1, 3), report).kNearest(5);
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
		testRunner().add("TdTree", test);
	}

	CallFunction run(addTest);

}
