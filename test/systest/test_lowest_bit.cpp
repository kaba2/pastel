// Description: Testing for lowest bit.
// DocumentationOf: lowest_bit.h

#include "test_pastelsys.h"

#include <pastel/sys/integer/multi_integer.h>
#include <pastel/sys/bit/lowest_bit.h>
#include <pastel/sys/bit/highest_bit.h>
#include <pastel/sys/concept.h>

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
			test<uint8>();
			test<uint16>();
			test<uint32>();
			test<uint64>();
			test<Signed_Integer<113, uint8>>();
			test<Unsigned_Integer<113, uint8>>();
		}

		template <typename Finite_Integer>
		void test()
		{
			TEST_ENSURE_OP(lowestBit(Finite_Integer(0)), ==, -1);
			TEST_ENSURE_OP(highestBit(Finite_Integer(0)), ==, -1);

			Finite_Integer a = 1;
			for (integer i = 0;i < bits(a);++i)
			{
				TEST_ENSURE_OP(lowestBit(a), ==, i);
				TEST_ENSURE_OP(highestBit(a), ==, i);
				a <<= 1;
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
		testRunner().add("lowest_bit", test);
	}

	CallFunction run(addTest);

}
