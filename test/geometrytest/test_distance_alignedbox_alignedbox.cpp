// Description: Testing for distance between aligned boxes
// DocumentationOf: distance_alignedbox_alignedbox.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/distance/distance_alignedbox_alignedbox.h>
#include <pastel/sys/rational.h>

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
			test<real>();
			test<Rational<integer>>();
			test<integer>();
		}

		template <typename Real>
		void test()
		{
			using Box = AlignedBox<Real, 2>;
			using Point = Vector<Real, 2>;

			{
				/*
				7
				6    BBB
				5    BBB
				4    BBB
				3
				2 AA
				1 AA
				0
				 01234567
				*/

				Box aBox(
					Point(1, 1), 
					Point(3, 3));

				Box bBox(
					Point(5, 4), 
					Point(7, 7));

				TEST_ENSURE(distance2(aBox, bBox) == square(2) + square(1));
				TEST_ENSURE(distance2(bBox, aBox) == square(2) + square(1));

				TEST_ENSURE(farthestDistance2(aBox, bBox) == square(6) + square(6));
				TEST_ENSURE(farthestDistance2(bBox, aBox) == square(6) + square(6));
			}

			{
				/*
				7
				6 BBB
				5 BBB
				4 BBB
				3
				2 AA
				1 AA
				0
				 01234567
				*/

				Box aBox(
					Point(1, 1), 
					Point(3, 3));

				Box bBox(
					Point(1, 4), 
					Point(4, 7));

				TEST_ENSURE(distance2(aBox, bBox) == 1);
				TEST_ENSURE(distance2(bBox, aBox) == 1);

				TEST_ENSURE(farthestDistance2(aBox, bBox) == square(3) + square(6));
				TEST_ENSURE(farthestDistance2(bBox, aBox) == square(3) + square(6));
			}

			{
				/*
				7
				6 BBB
				5 BBB
				4 BBB
				3
				2   AA
				1   AA
				0
				 01234567
				*/

				Box aBox(
					Point(3, 1), 
					Point(5, 3));

				Box bBox(
					Point(1, 4), 
					Point(4, 7));

				TEST_ENSURE(distance2(aBox, bBox) == 1);
				TEST_ENSURE(distance2(bBox, aBox) == 1);

				TEST_ENSURE(farthestDistance2(aBox, bBox) == square(4) + square(6));
				TEST_ENSURE(farthestDistance2(bBox, aBox) == square(4) + square(6));
			}

			{
				/*
				7
				6 BBB
				5 BBB  AA
				4 BBB  AA
				3
				2        
				1        
				0
				 01234567
				*/

				Box aBox(
					Point(6, 4), 
					Point(8, 6));

				Box bBox(
					Point(1, 4), 
					Point(4, 7));

				TEST_ENSURE(distance2(aBox, bBox) == 4);
				TEST_ENSURE(distance2(bBox, aBox) == 4);

				TEST_ENSURE(farthestDistance2(aBox, bBox) == square(7) + square(3));
				TEST_ENSURE(farthestDistance2(bBox, aBox) == square(7) + square(3));
			}

			{
				/*
				7
				6 BBB
				5 BAB    
				4 BBB    
				3
				2        
				1        
				0
				 01234567
				*/

				Box aBox(
					Point(2, 5),
					Point(3, 6));

				Box bBox(
					Point(1, 4), 
					Point(4, 7));

				TEST_ENSURE(distance2(aBox, bBox) == 0);
				TEST_ENSURE(distance2(bBox, aBox) == 0);

				TEST_ENSURE(farthestDistance2(aBox, bBox) == square(2) + square(2));
				TEST_ENSURE(farthestDistance2(bBox, aBox) == square(2) + square(2));
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
		testRunner().add("distance_alignedbox_alignedbox", test);
	}

	CallFunction run(addTest);

}
