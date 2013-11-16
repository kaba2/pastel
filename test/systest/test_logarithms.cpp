// Description: Testing for logarithms
// DocumentationOf: logarithm.h

#include "test_pastelsys.h"

#include "pastel/sys/logarithm.h"
#include "pastel/sys/error.h"

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

		bool check(real measured, real correct)
		{
			const real epsilon = 0.001;
			return REPORT3((absoluteError<real>(measured, correct) > epsilon),
				measured, correct, epsilon);
		}

		void test()
		{
			check(floorLog2<real>(1), 0);
			check(floorLog2<real>(1.1), 0);
			check(floorLog2<real>(1.5), 0);
			check(floorLog2<real>(1.9), 0);
			
			check(floorLog2<real>(2), 1);
			check(floorLog2<real>(2.1), 1);
			check(floorLog2<real>(3), 1);
			check(floorLog2<real>(3.9), 1);
			
			check(floorLog2<real>(4), 2);
			check(floorLog2<real>(4.1), 2);
			check(floorLog2<real>(6), 2);
			check(floorLog2<real>(7.9), 2);

			check(floorLog2<real>(8), 3);
			check(floorLog2<real>(8.1), 3);
			check(floorLog2<real>(12), 3);
			check(floorLog2<real>(15.9), 3);

			check(floorLog2<real>(16), 4);
			check(floorLog2<real>(16.1), 4);
			check(floorLog2<real>(24), 4);
			check(floorLog2<real>(31.9), 4);

			check(floorLog2<real>(32), 5);
			check(floorLog2<real>(32.1), 5);
			check(floorLog2<real>(48), 5);
			check(floorLog2<real>(63.9), 5);

			check(log2<real>(1), 0);
			check(log2<real>(2), 1);
			check(log2<real>(4), 2);
			check(log2<real>(8), 3);
			check(log2<real>(16), 4);
			check(log2<real>(32), 5);

			TEST_ENSURE_OP(integerLog2(1), ==, 0);
			TEST_ENSURE_OP(integerLog2(2), ==, 1);
			TEST_ENSURE_OP(integerLog2(3), ==, 1);
			TEST_ENSURE_OP(integerLog2(4), ==, 2);
			TEST_ENSURE_OP(integerLog2(5), ==, 2);
			TEST_ENSURE_OP(integerLog2(6), ==, 2);
			TEST_ENSURE_OP(integerLog2(7), ==, 2);
			TEST_ENSURE_OP(integerLog2(8), ==, 3);
			TEST_ENSURE_OP(integerLog2(9), ==, 3);
			TEST_ENSURE_OP(integerLog2(10), ==, 3);
			TEST_ENSURE_OP(integerLog2(11), ==, 3);
			TEST_ENSURE_OP(integerLog2(12), ==, 3);
			TEST_ENSURE_OP(integerLog2(13), ==, 3);
			TEST_ENSURE_OP(integerLog2(14), ==, 3);
			TEST_ENSURE_OP(integerLog2(15), ==, 3);
			TEST_ENSURE_OP(integerLog2(16), ==, 4);

			TEST_ENSURE_OP(integerCeilLog2(1), ==, 0);
			TEST_ENSURE_OP(integerCeilLog2(2), ==, 1);
			TEST_ENSURE_OP(integerCeilLog2(3), ==, 2);
			TEST_ENSURE_OP(integerCeilLog2(4), ==, 2);
			TEST_ENSURE_OP(integerCeilLog2(5), ==, 3);
			TEST_ENSURE_OP(integerCeilLog2(6), ==, 3);
			TEST_ENSURE_OP(integerCeilLog2(7), ==, 3);
			TEST_ENSURE_OP(integerCeilLog2(8), ==, 3);
			TEST_ENSURE_OP(integerCeilLog2(9), ==, 4);
			TEST_ENSURE_OP(integerCeilLog2(10), ==, 4);
			TEST_ENSURE_OP(integerCeilLog2(11), ==, 4);
			TEST_ENSURE_OP(integerCeilLog2(12), ==, 4);
			TEST_ENSURE_OP(integerCeilLog2(13), ==, 4);
			TEST_ENSURE_OP(integerCeilLog2(14), ==, 4);
			TEST_ENSURE_OP(integerCeilLog2(15), ==, 4);
			TEST_ENSURE_OP(integerCeilLog2(16), ==, 4);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTests()
	{
		testRunner().add("logarithms", test);
	}

	CallFunction run(addTests);

}
