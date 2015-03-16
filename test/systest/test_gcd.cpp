// Description: Testing for gcd
// DocumentationOf: gcd.h

#include "test_pastelsys.h"

#include "pastel/sys/integer/multi_integer.h"
#include "pastel/sys/integer/gcd.h"

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
			testSome<int16>();
			testSome<int32>();
			testSome<integer>();
			testSome<Signed_Integer<16>>();
			testSome<Signed_Integer<32>>();
		}

		template <typename Integer>
		void testSome()
		{
			TEST_ENSURE(gcd(Integer(1), Integer(5)) == Integer(1));
			TEST_ENSURE(gcd(Integer(1), Integer(3455)) == Integer(1));
			TEST_ENSURE(gcd(Integer(1), Integer(2235)) == Integer(1));
			TEST_ENSURE(gcd(Integer(1), Integer(2344)) == Integer(1));

			TEST_ENSURE(gcd(Integer(0), Integer(2344)) == Integer(2344));
			TEST_ENSURE(gcd(Integer(0), Integer(23455)) == Integer(23455));
			TEST_ENSURE(gcd(Integer(0), Integer(865)) == Integer(865));
			TEST_ENSURE(gcd(Integer(0), Integer(5487)) == Integer(5487));

			TEST_ENSURE(gcd(Integer(13), Integer(7)) == Integer(1));
			TEST_ENSURE(gcd(Integer(5), Integer(23)) == Integer(1));
			TEST_ENSURE(gcd(Integer(7), Integer(11)) == Integer(1));
			TEST_ENSURE(gcd(Integer(3), Integer(2)) == Integer(1));

			TEST_ENSURE(gcd(Integer(2), Integer(4)) == Integer(2));

			TEST_ENSURE(gcd(Integer(16), Integer(256)) == Integer(16));
			TEST_ENSURE(gcd(Integer(128), Integer(256)) == Integer(128));
			TEST_ENSURE(gcd(Integer(256), Integer(128)) == Integer(128));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Gcd", test);
	}

	CallFunction run(addTest);

}
