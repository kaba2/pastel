// Description: Testing for temporal kd-tree
// DocumentationOf: tdtree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/tdtree.h"
#include "pastel/sys/locators.h"
#include "pastel/sys/inputs.h"
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
			using Point = Vector2i;
			using Locator = Vector_Locator<integer, 2>;
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

				Tree tree(rangeInput(pointSet), Locator());
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
