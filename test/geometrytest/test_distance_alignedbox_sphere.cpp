// Description: Testing for distance between aligned box and a sphere
// DocumentationOf: distance_alignedbox_sphere.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/distance_alignedbox_sphere.h>

using namespace Pastel;
using namespace std;

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

		void test()
		{
			using Box = AlignedBox<real, 2>;
			using Sphere = Sphere<real, 2>;
			using Point = Vector<real, 2>;

			{
				// 5
				// 4 BBBB
				// 3 BBBB
				// 2
				// 1 
				// 0 S
				//  01234567

				Box box(Point(1, 3), Point(5, 5));
				Sphere sphere(Point(1, 0), 1);
				TEST_ENSURE_OP(distance(box, sphere), ==, std::sqrt(square(0) + square(3)) - 1);
			}

			{
				// 5
				// 4 BBBB
				// 3 BBBB
				// 2
				// 1 
				// 0S
				//  01234567

				Box box(Point(1, 3), Point(5, 5));
				Sphere sphere(Point(0, 0), 1);
				TEST_ENSURE(distance(box, sphere) == std::sqrt(square(1) + square(3)) - 1);
			}

			{
				// 5
				// 4 BBBB
				// 3 BBBB
				// 2       S
				// 1 
				// 0       
				//  01234567

				Box box(Point(1, 3), Point(5, 5));
				Sphere sphere(Point(7, 2), 1);
				TEST_ENSURE(distance(box, sphere) == std::sqrt(square(2) + square(1)) - 1);
			}

			{
				// 5
				// 4 BBBB
				// 3 BBBS
				// 2        
				// 1 
				// 0       
				//  01234567

				Box box(Point(1, 3), Point(5, 5));
				Sphere sphere(Point(5, 3), 1);
				TEST_ENSURE(distance(box, sphere) == 0);
			}

			{
				// 5
				// 4 BBBB
				// 3 BBBB  S
				// 2        
				// 1 
				// 0       
				//  01234567

				Box box(Point(1, 3), Point(5, 5));
				Sphere sphere(Point(7, 3), 1);
				TEST_ENSURE(distance(box, sphere) == std::sqrt(square(2) + square(0)) - 1);
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
		testRunner().add("distance_alignedbox_sphere", test);
	}

	CallFunction run(addTest);

}
