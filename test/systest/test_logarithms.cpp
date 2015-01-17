// Description: Testing for logarithms
// DocumentationOf: logarithm.h

#include "test_pastelsys.h"

#include "pastel/sys/math/logarithm.h"
#include "pastel/sys/math/error.h"

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
			real epsilon = 0.001;
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

			TEST_ENSURE_OP(integerLog2(0x00000001), == , 0);
			TEST_ENSURE_OP(integerLog2(0x00000002), == , 1);
			TEST_ENSURE_OP(integerLog2(0x00000004), == , 2);
			TEST_ENSURE_OP(integerLog2(0x00000008), == , 3);
			TEST_ENSURE_OP(integerLog2(0x00000019), == , 4);
			TEST_ENSURE_OP(integerLog2(0x00000029), == , 5);
			TEST_ENSURE_OP(integerLog2(0x00000049), == , 6);
			TEST_ENSURE_OP(integerLog2(0x00000089), == , 7);
			TEST_ENSURE_OP(integerLog2(0x00000198), == , 8);
			TEST_ENSURE_OP(integerLog2(0x00000298), == , 9);
			TEST_ENSURE_OP(integerLog2(0x00000498), == , 10);
			TEST_ENSURE_OP(integerLog2(0x00000898), == , 11);
			TEST_ENSURE_OP(integerLog2(0x00001987), == , 12);
			TEST_ENSURE_OP(integerLog2(0x00002987), == , 13);
			TEST_ENSURE_OP(integerLog2(0x00004987), == , 14);
			TEST_ENSURE_OP(integerLog2(0x00008987), == , 15);
			TEST_ENSURE_OP(integerLog2(0x00019876), == , 16);
			TEST_ENSURE_OP(integerLog2(0x00029876), == , 17);
			TEST_ENSURE_OP(integerLog2(0x00049876), == , 18);
			TEST_ENSURE_OP(integerLog2(0x00089876), == , 19);
			TEST_ENSURE_OP(integerLog2(0x00198765), == , 20);
			TEST_ENSURE_OP(integerLog2(0x00298765), == , 21);
			TEST_ENSURE_OP(integerLog2(0x00498765), == , 22);
			TEST_ENSURE_OP(integerLog2(0x00898765), == , 23);
			TEST_ENSURE_OP(integerLog2(0x01987654), == , 24);
			TEST_ENSURE_OP(integerLog2(0x02987654), == , 25);
			TEST_ENSURE_OP(integerLog2(0x04987654), == , 26);
			TEST_ENSURE_OP(integerLog2(0x08987654), == , 27);
			TEST_ENSURE_OP(integerLog2(0x19876543), == , 28);
			TEST_ENSURE_OP(integerLog2(0x29876543), == , 29);
			TEST_ENSURE_OP(integerLog2(0x49876543), == , 30);
			TEST_ENSURE_OP(integerLog2(0x89876543), == , 31);

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
