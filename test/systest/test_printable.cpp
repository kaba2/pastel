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

		template <typename Type>
		void operator<<(const Type& that)
		{
			asString(that);
		}

		void test()
		{
			char testArray[] = { 't', 'e', 's', 't'};

			enum class A { a = 113 };

			*this << "test";

			TEST_ENSURE(asString(A::a) == "113");
			TEST_ENSURE(asString(113) == "113");
			TEST_ENSURE(asString((double)113) == "113");
			TEST_ENSURE(asString((float)113) == "113");
			TEST_ENSURE(asString('a') == "a");
			TEST_ENSURE(asString("test") == "test");
			TEST_ENSURE(asString(std::string("test")) == "test");
			TEST_ENSURE(asString(testArray) == "test");
			TEST_ENSURE(asString((short)113) == "113");
			TEST_ENSURE(asString((ushort)113) == "113");
			TEST_ENSURE(asString((int)113) == "113");
			TEST_ENSURE(asString((uint)113) == "113");
			TEST_ENSURE(asString((long)113) == "113");
			TEST_ENSURE(asString((ulong)113) == "113");
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
