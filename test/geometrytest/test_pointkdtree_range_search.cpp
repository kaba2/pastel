// Description: Testing for point kd-tree range search
// DocumentationOf: pointkdtree_search_range.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/pointkdtree/pointkdtree.h>
#include <pastel/geometry/pointkdtree/pointkdtree_search_range.h>

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

		virtual void run()
		{
			test();
		}

		void test()
		{
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("pointkdtree_search_range", test);
	}

	CallFunction run(addTest);

}
