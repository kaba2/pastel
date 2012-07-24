// Description: Testing for refinable partition
// DocumentationOf: refinable_partition.h

#include "pastelsystest.h"

#include <pastel/sys/refinable_partition.h>

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
			RefinablePartition partition(5);
			
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("RefinablePartition", test);
	}

	CallFunction run(addTest);

}
