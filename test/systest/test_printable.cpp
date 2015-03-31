// Description: Testing for the printable concept
// Documentation: unit_testing.txt

#include "test_pastelsys.h"

#include <pastel/sys/printable.h>

using namespace Pastel;

namespace
{

	PASTEL_CONCEPT_CHECK(integer, Printable_Concept);

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
			enum class A { a = 113 };

			TEST_ENSURE(abs(stringAsReal<float>("3.34897") - 3.34897) <= 0.000005);
			TEST_ENSURE(abs(stringAsReal<float>("5.09865") - 5.09865) <= 0.000005);
			TEST_ENSURE(abs(stringAsReal<double>("5.12345678901234567890") - 5.12345678901234567890) == 0);

			TEST_ENSURE(asString(A::a) == "113");
			TEST_ENSURE(asString(113) == "113");
			TEST_ENSURE(asString((double)113) == "113");
			TEST_ENSURE(asString((float)113) == "113");
			TEST_ENSURE(asString('a') == "a");
			TEST_ENSURE(asString("test") == "test");
			TEST_ENSURE(asString(std::string("test")) == "test");
			TEST_ENSURE(asString((short)113) == "113");
			TEST_ENSURE(asString((ushort)113) == "113");
			TEST_ENSURE(asString((int)113) == "113");
			TEST_ENSURE(asString((uint)113) == "113");
			TEST_ENSURE(asString((long)113) == "113");
			TEST_ENSURE(asString((ulong)113) == "113");

			char testArray[] = { 't', 'e', 's', 't'};
			// FIX: What should happen here?
			//TEST_ENSURE(asString(testArray) == "test");
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("printable", test);
	}

	CallFunction run(addTest);

}
