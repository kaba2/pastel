// Description: Testing for temporal kd-tree
// DocumentationOf: tdtree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/tdtree.h"

#include "pastel/geometry/pointkdtree_search_nearest.h"

#include "pastel/sys/locators.h"
#include "pastel/sys/inputs.h"
#include "pastel/sys/outputs.h"
#include "pastel/sys/for_each_point.h"

using namespace Pastel;
using namespace std;

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

			auto output = Null_Output();
			auto accept = All_Indicator();
			auto norm = Euclidean_NormBijection<real>();
			auto algorithm = DepthFirst_SearchAlgorithm_PointKdTree();
			
			for (integer i = 0; i < n; ++i)
			{
				Vector2 timeInterval = { (real)i, (real)n };
				KeyValue<real, ConstIterator> nearestPair =
					searchNearest(tree, Point(0, 0), output, accept,
					norm, algorithm, timeInterval);
				TEST_ENSURE((integer)nearestPair.key() == square(i));
			}

			for (integer i = 0; i < n; ++i)
			{
				Vector2 timeInterval = { (real)0, (real)i + 1 };
				KeyValue<real, ConstIterator> nearestPair =
					searchNearest(tree, Point(0, 0), output, accept,
					norm, algorithm, timeInterval);
				TEST_ENSURE((integer)nearestPair.key() == 0);
			}

			for (integer i = 0; i < n; ++i)
			{
				Vector2 timeInterval = { (real)i, (real)i + 1 };
				KeyValue<real, ConstIterator> nearestPair =
					searchNearest(tree, Point(0, 0), output, accept,
					norm, algorithm, timeInterval);
				TEST_ENSURE((integer)nearestPair.key() == square(i));
			}

			for (integer i = 0; i < n; ++i)
			{
				Vector4 timeInterval = { (real)i, (real)i + 1, (real)i + 5, (real)i + 6 };
				KeyValue<real, ConstIterator> nearestPair =
					searchNearest(tree, Point(i + 2, 0), output, accept,
					norm, algorithm, timeInterval);
				TEST_ENSURE_OP((integer)nearestPair.key(), ==, square(2));
			}
		}

		void test()
		{
			{
				Tree tree;
				tree.clear();
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

				TEST_ENSURE_OP(tree.timeToIndex(-infinity<real>()), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(-2), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(-1), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(-0.5), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(0), ==, 0);
				TEST_ENSURE_OP(tree.timeToIndex(0.5), ==, 1);
				TEST_ENSURE_OP(tree.timeToIndex(1), ==, 1);

				std::vector<ConstIterator> neighborSet;

				auto report = [&](
					real distance,
					const ConstIterator& point)
				{
					neighborSet.emplace_back(point);
				};

				searchNearest(tree, Point(1, 3), report).kNearest(5);

				for (auto&& point : neighborSet)
				{
					std::cout << point->point() << std::endl;
				}
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
