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

		virtual void run()
		{
			test();
		}

		void test()
		{
			using Point = Vector2;
			using Locator = Vector_Locator<real, 2>;
			using Tree = TdTree<TdTree_Settings<Locator>>;

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
				using ConstIterator = Tree::ConstIterator;

				std::vector<ConstIterator> neighborSet;

				auto report = [&](
					real distance,
					const ConstIterator& point)
				{
					neighborSet.emplace_back(point);
				};
				
				searchNearest(tree, Point(0, 0), report);
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
