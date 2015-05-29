// Description: Testing for distance between aligned box and point
// DocumentationOf: distance_alignedbox_alignedbox.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/distance/distance_alignedbox_point.h>
#include <pastel/sys/rational.h>
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
			test<real>();
			/*
			test<Rational<integer>>();
			test<integer>();
			*/
		}

		template <typename Real>
		void test()
		{
			using Box = AlignedBox<Real, 2>;
			using Point = Vector<Real, 2>;

			PASTEL_CONCEPT_CHECK(Point, Point_Concept);
			PASTEL_CONCEPT_CHECK(Euclidean_NormBijection<Real>, NormBijection_Concept);
			PASTEL_STATIC_ASSERT(EqualDimension<Integer<2>, Point_N<Point>>::value);

			{
				/*
				7
				6     BBB
				5     BBB
				4     BBB
				3   *
				2   
				1   
				0
				 01234567
				*/

				Point aPoint(3, 3);

				Box bBox(
					Point(5, 4), 
					Point(7, 7));

				TEST_ENSURE(distance2(bBox, aPoint) == square(2) + square(1));
				TEST_ENSURE(farthestDistance2(bBox, aPoint) == square(6) + square(6));
			}
			{
				/*
				7
				6 BBB
				5 BBB
				4 BBB
				3   *
				2   
				1   
				0
				 01234567
				*/

				Point aPoint(3, 3);

				Box bBox(
					Point(1, 4), 
					Point(4, 7));

				TEST_ENSURE(distance2(bBox, aPoint) == 1);
				TEST_ENSURE(farthestDistance2(bBox, aPoint) == square(3) + square(6));
			}

			{
				/*
				7
				6 BBB
				5 BBB
				4 BBB
				3     *
				2     
				1     
				0
				 01234567
				*/

				Point aPoint(5, 3);

				Box bBox(
					Point(1, 4), 
					Point(4, 7));

				TEST_ENSURE(distance2(bBox, aPoint) == 1);
				TEST_ENSURE(farthestDistance2(bBox, aPoint) == square(4) + square(6));
			}

			{
				/*
				7
				6 BBB    
				5 BBB    
				4 BBB  *    
				3
				2        
				1        
				0
				 012345678
				*/

				Point aPoint(6, 4);

				Box bBox(
					Point(1, 4), 
					Point(4, 7));

				TEST_ENSURE(distance2(bBox, aPoint) == 4);
				TEST_ENSURE(farthestDistance2(bBox, aPoint) == square(7) + square(3));
			}

			{
				/*
				7
				6 BBB
				5 B*B    
				4 BBB    
				3       
				2        
				1        
				0
				 01234567
				*/

				Point aPoint(2, 5);

				Box bBox(
					Point(1, 4), 
					Point(4, 7));

				TEST_ENSURE(distance2(bBox, aPoint) == 0);
				TEST_ENSURE(farthestDistance2(bBox, aPoint) == square(2) + square(2));
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
		testRunner().add("distance_alignedbox_point", test);
	}

	CallFunction run(addTest);

}