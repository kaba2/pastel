// Description: Testing for logarithms
// DocumentationOf: logarithm.h

#include "test/test_init.h"

#include "pastel/sys/math/logarithm.h"
#include "pastel/sys/math/error.h"

namespace
{

	class Test
	{
	public:
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

			REQUIRE(integerLog2(1) == 0);
			REQUIRE(integerLog2(2) == 1);
			REQUIRE(integerLog2(3) == 1);
			REQUIRE(integerLog2(4) == 2);
			REQUIRE(integerLog2(5) == 2);
			REQUIRE(integerLog2(6) == 2);
			REQUIRE(integerLog2(7) == 2);
			REQUIRE(integerLog2(8) == 3);
			REQUIRE(integerLog2(9) == 3);
			REQUIRE(integerLog2(10) == 3);
			REQUIRE(integerLog2(11) == 3);
			REQUIRE(integerLog2(12) == 3);
			REQUIRE(integerLog2(13) == 3);
			REQUIRE(integerLog2(14) == 3);
			REQUIRE(integerLog2(15) == 3);
			REQUIRE(integerLog2(16) == 4);

			REQUIRE(integerLog2(0x00000001) == 0);
			REQUIRE(integerLog2(0x00000002) == 1);
			REQUIRE(integerLog2(0x00000004) == 2);
			REQUIRE(integerLog2(0x00000008) == 3);
			REQUIRE(integerLog2(0x00000019) == 4);
			REQUIRE(integerLog2(0x00000029) == 5);
			REQUIRE(integerLog2(0x00000049) == 6);
			REQUIRE(integerLog2(0x00000089) == 7);
			REQUIRE(integerLog2(0x00000198) == 8);
			REQUIRE(integerLog2(0x00000298) == 9);
			REQUIRE(integerLog2(0x00000498) == 10);
			REQUIRE(integerLog2(0x00000898) == 11);
			REQUIRE(integerLog2(0x00001987) == 12);
			REQUIRE(integerLog2(0x00002987) == 13);
			REQUIRE(integerLog2(0x00004987) == 14);
			REQUIRE(integerLog2(0x00008987) == 15);
			REQUIRE(integerLog2(0x00019876) == 16);
			REQUIRE(integerLog2(0x00029876) == 17);
			REQUIRE(integerLog2(0x00049876) == 18);
			REQUIRE(integerLog2(0x00089876) == 19);
			REQUIRE(integerLog2(0x00198765) == 20);
			REQUIRE(integerLog2(0x00298765) == 21);
			REQUIRE(integerLog2(0x00498765) == 22);
			REQUIRE(integerLog2(0x00898765) == 23);
			REQUIRE(integerLog2(0x01987654) == 24);
			REQUIRE(integerLog2(0x02987654) == 25);
			REQUIRE(integerLog2(0x04987654) == 26);
			REQUIRE(integerLog2(0x08987654) == 27);
			REQUIRE(integerLog2(0x19876543) == 28);
			REQUIRE(integerLog2(0x29876543) == 29);
			REQUIRE(integerLog2(0x49876543) == 30);
			REQUIRE(integerLog2(0x89876543) == 31);

			REQUIRE(integerCeilLog2(1) == 0);
			REQUIRE(integerCeilLog2(2) == 1);
			REQUIRE(integerCeilLog2(3) == 2);
			REQUIRE(integerCeilLog2(4) == 2);
			REQUIRE(integerCeilLog2(5) == 3);
			REQUIRE(integerCeilLog2(6) == 3);
			REQUIRE(integerCeilLog2(7) == 3);
			REQUIRE(integerCeilLog2(8) == 3);
			REQUIRE(integerCeilLog2(9) == 4);
			REQUIRE(integerCeilLog2(10) == 4);
			REQUIRE(integerCeilLog2(11) == 4);
			REQUIRE(integerCeilLog2(12) == 4);
			REQUIRE(integerCeilLog2(13) == 4);
			REQUIRE(integerCeilLog2(14) == 4);
			REQUIRE(integerCeilLog2(15) == 4);
			REQUIRE(integerCeilLog2(16) == 4);
		}
	};

}
