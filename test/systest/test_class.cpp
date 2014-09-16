// Description: Testing for class wrapping
// DocumentationOf: class.h

#include "test_pastelsys.h"

#include <pastel/sys/class.h>

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
			Class<int> a(5);
			{
				TEST_ENSURE_OP(a, ==, 5);
			}

			a += 3;
			{
				TEST_ENSURE_OP(a, ==, 8);
			}

			Class<void> c;
			unused(c);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("class", test);
	}

	CallFunction run(addTest);

}
