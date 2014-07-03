// Description: Testing for point kd-tree range search
// DocumentationOf: pointkdtree_range_search.h

#include "test_pastelsys.h"

#include <pastel/sys/pointkdtree.h>
#include <pastel/sys/pointkdtree_range_search.h>

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
			typedef PointKdTree<real, 2> Tree;
			typedef Tree::Point_ConstIterator Point_ConstIterator;

			Tree tree;
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("pointkdtree_range_search", test);
	}

	CallFunction run(addTest);

}
