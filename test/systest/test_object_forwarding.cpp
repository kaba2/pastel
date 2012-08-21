// Description: Testing for object forwarding
// DocumentationOf: object_forwarding.h

#include "test_pastelsys.h"

#include <pastel/sys/object_forwarding.h>

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
		testRunner().add("object_forwarding", test);
	}

	CallFunction run(addTest);

}
