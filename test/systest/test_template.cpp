// Description: Testing template
// Documentation: unit_testing.txt

#include "test_pastelsys.h"

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
		testRunner().add("None", test);
	}

	CallFunction run(addTest);

}
