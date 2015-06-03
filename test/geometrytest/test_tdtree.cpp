// Description: Testing for temporal kd-tree
// DocumentationOf: tdtree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/tdtree/tdtree.h"

#include "pastel/geometry/search_nearest_kdtree.h"
#include "pastel/geometry/distance/distance_point_point.h"
#include "pastel/geometry/search_nearest_bruteforce.h"

#include "pastel/sys/locator.h"
#include "pastel/sys/input.h"
#include "pastel/sys/output.h"
#include "pastel/sys/for_each_point.h"
#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/iterator/counting_iterator.h"
#include "pastel/sys/iterator/constant_iterator.h"

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

			integer k = 7;

			for (integer i = 0; i < n; ++i)
			{
				std::vector<std::pair<real, Point>> bruteSet;
				bruteSet.reserve(k);

				real kDistanceBrute = searchNearest(
					bruteForceNearestSet(rangeInput(pointSet)),
					pointSet[i],
					PASTEL_TAG(nearestOutput), emplaceBackOutput(bruteSet),
					PASTEL_TAG(kNearest), k
					).first;

				std::vector<std::pair<real, ConstIterator>> treeSet;
				treeSet.reserve(k);

				real kDistanceTree = searchNearest(
						tree,
						pointSet[i],
						PASTEL_TAG(nearestOutput), emplaceBackOutput(treeSet),
						PASTEL_TAG(kNearest), k
					).first;

				TEST_ENSURE_OP(kDistanceBrute, ==, kDistanceTree);

				for (integer j = 0; j < k; ++j)
				{
					TEST_ENSURE_OP(bruteSet[j].first, ==, treeSet[j].first);
					if (bruteSet[j].first != treeSet[j].first)
					{
						std::cout << bruteSet[j].first << " != " << treeSet[j].first << std::endl;
					}
				}
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

			for (integer i = 0; i < n; ++i)
			{
				{
					Vector2 timeInterval = { (real)i, (real)n };
					integer distance =
						searchNearest(
							tree, 
							Point(0, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;
					TEST_ENSURE_OP(distance, ==, square(i));
				}
				{
					Vector2 timeInterval = { (real)0, (real)i + 1 };
					integer distance =
						searchNearest(
							tree, 
							Point(0, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;
					TEST_ENSURE_OP(distance, ==, 0);
				}
				{
					Vector2 timeInterval = { (real)i, (real)i + 1 };
					integer distance =
						searchNearest(
							tree, 
							Point(0, 0),
							PASTEL_TAG(intervalSequence), timeInterval).first;
					TEST_ENSURE_OP(distance, ==, square(i));
				}
			}

			for (integer i = 0; i < n; ++i)
			{
				Vector4 timeInterval = { (real)i, (real)i + 1, (real)i + 5, (real)i + 6 };
				{
					real distance =
						searchNearest(
							tree, 
							Point(i + 2, 0),
							PASTEL_TAG(intervalSequence), timeInterval).first;

					TEST_ENSURE_OP((integer)distance, ==, square(2));
				}

				{
					real distance =
						searchNearest(
							tree, 
							Point(i - 3, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;

					TEST_ENSURE_OP((integer)distance, ==, square(3));
				}

				{
					real distance =
						searchNearest(
							tree, 
							Point(i + 4, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;
					integer correct = i < (n - 5) ? square(1) : square(4);

					TEST_ENSURE_OP((integer)distance, ==, correct);
				}

				{
					real distance =
						searchNearest(
							tree, 
							Point(i + 7, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;
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

				searchNearest(
					tree, 
					Point(1, 3), 
					PASTEL_TAG(nearestOutput), report,
					PASTEL_TAG(kNearest), 5);
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
