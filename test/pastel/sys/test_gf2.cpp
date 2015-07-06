// Description: Testing for Gf2
// DocumentationOf: gf2.h

#include "test_pastelsys.h"

#include "pastel/sys/gf2.h"

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
			{
				using F = Gf2<1>;

				TEST_ENSURE(F(0) + F(0) == F(0));
				TEST_ENSURE(F(0) + F(1) == F(1));
				TEST_ENSURE(F(1) + F(0) == F(1));
				TEST_ENSURE(F(1) + F(1) == F(0));

				TEST_ENSURE(F(0) * F(0) == F(0));
				TEST_ENSURE(F(0) * F(1) == F(0));
				TEST_ENSURE(F(1) * F(0) == F(0));
				TEST_ENSURE(F(1) * F(1) == F(1));

				TEST_ENSURE((F(0) << 1) == F(0));
				TEST_ENSURE((F(1) << 1) == F(0));

				TEST_ENSURE((F(0) << 2) == F(0));
				TEST_ENSURE((F(1) << 2) == F(0));
			}

			{
				using F = Gf2<2>;

				TEST_ENSURE(F(0) + F(0) == F(0));
				TEST_ENSURE(F(0) + F(1) == F(1));
				TEST_ENSURE(F(0) + F(2) == F(2));
				TEST_ENSURE(F(0) + F(3) == F(3));
				TEST_ENSURE(F(1) + F(0) == F(1));
				TEST_ENSURE(F(1) + F(1) == F(0));
				TEST_ENSURE(F(1) + F(2) == F(3));
				TEST_ENSURE(F(1) + F(3) == F(2));
				TEST_ENSURE(F(2) + F(0) == F(2));
				TEST_ENSURE(F(2) + F(1) == F(3));
				TEST_ENSURE(F(2) + F(2) == F(0));
				TEST_ENSURE(F(2) + F(3) == F(1));
				TEST_ENSURE(F(3) + F(0) == F(3));
				TEST_ENSURE(F(3) + F(1) == F(2));
				TEST_ENSURE(F(3) + F(2) == F(1));
				TEST_ENSURE(F(3) + F(3) == F(0));

				TEST_ENSURE(F(0) * F(0) == F(0));
				TEST_ENSURE(F(0) * F(1) == F(0));
				TEST_ENSURE(F(0) * F(2) == F(0));
				TEST_ENSURE(F(0) * F(3) == F(0));
				TEST_ENSURE(F(1) * F(0) == F(0));
				TEST_ENSURE(F(1) * F(1) == F(1));
				TEST_ENSURE(F(1) * F(2) == F(2));
				TEST_ENSURE(F(1) * F(3) == F(3));
				TEST_ENSURE(F(2) * F(0) == F(0));
				TEST_ENSURE(F(2) * F(1) == F(2));
				TEST_ENSURE(F(2) * F(2) == F(3));
				TEST_ENSURE(F(2) * F(3) == F(1));
				TEST_ENSURE(F(3) * F(0) == F(0));
				TEST_ENSURE(F(3) * F(1) == F(3));
				TEST_ENSURE(F(3) * F(2) == F(1));
				TEST_ENSURE(F(3) * F(3) == F(2));

				TEST_ENSURE((F(0) << 1) == F(0) * F(2));
				TEST_ENSURE((F(1) << 1) == F(1) * F(2));
				TEST_ENSURE((F(2) << 1) == F(2) * F(2));
				TEST_ENSURE((F(3) << 1) == F(3) * F(2));

				TEST_ENSURE((F(0) << 2) == F(0) * F(2) * F(2));
				TEST_ENSURE((F(1) << 2) == F(1) * F(2) * F(2));
				TEST_ENSURE((F(2) << 2) == F(2) * F(2) * F(2));
				TEST_ENSURE((F(3) << 2) == F(3) * F(2) * F(2));
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
		testRunner().add("Gf2", test);
	}

	CallFunction run(addTest);

}
