// Description: Testing for BigInteger
// DocumentationOf: biginteger.h

#include "test_pastelsys.h"

#include <pastel/sys/biginteger.h>

namespace
{

	using namespace Pastel;

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
			testSimple();
		}

		void testSimple()
		{
			TEST_ENSURE(
				BigInteger(1234) + BigInteger(5634) ==
				BigInteger(6868));
			TEST_ENSURE(
				BigInteger(5634) + BigInteger(1234) ==
				BigInteger(6868));
			TEST_ENSURE(
				BigInteger(1234) - BigInteger(5634) ==
				BigInteger(-4400));
			TEST_ENSURE(
				BigInteger(5634) - BigInteger(1234) ==
				BigInteger(4400));
			TEST_ENSURE(
				BigInteger(1234) * BigInteger(5634) ==
				BigInteger(6952356));
			TEST_ENSURE(
				BigInteger(5634) * BigInteger(1234) ==
				BigInteger(6952356));
			TEST_ENSURE(
				BigInteger(1234) / BigInteger(5634) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger(5634) / BigInteger(1234) ==
				BigInteger(4));
			TEST_ENSURE(
				BigInteger(0) + BigInteger(0) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger(1234) - BigInteger(1234) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger(1234) + BigInteger(-1234) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger(-1234) + BigInteger(1234) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger(-1234) + BigInteger(-1234) ==
				BigInteger(-2468));
			TEST_ENSURE(
				BigInteger(1234) * BigInteger(0) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger(0) * BigInteger(1234) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger(-1234) * BigInteger(0) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger(0) * BigInteger(-1234) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger("-0") ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger("23452345") /
				BigInteger(10) ==
				BigInteger("2345234"));
			TEST_ENSURE(
				BigInteger("2345234") /
				BigInteger(10) ==
				BigInteger("234523"));
			TEST_ENSURE(
				BigInteger("234523") /
				BigInteger(10) ==
				BigInteger("23452"));
			TEST_ENSURE(
				BigInteger("23452") /
				BigInteger(10) ==
				BigInteger("2345"));
			TEST_ENSURE(
				BigInteger("2345") /
				BigInteger(10) ==
				BigInteger("234"));
			TEST_ENSURE(
				BigInteger("234") /
				BigInteger(10) ==
				BigInteger("23"));
			TEST_ENSURE(
				BigInteger("23") /
				BigInteger(10) ==
				BigInteger("2"));
			TEST_ENSURE(
				BigInteger("2") /
				BigInteger(10) ==
				BigInteger(0));
			TEST_ENSURE(
				BigInteger("21017020") /
				BigInteger(10) ==
				BigInteger("2101702"));
			TEST_ENSURE(-BigInteger(0) ==
				BigInteger(0));

			BigInteger a("2435325");
			BigInteger b("23452345");
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("BigInteger", test);
	}

	CallFunction run(addTest);

}
