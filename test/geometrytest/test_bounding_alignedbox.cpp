// Description: Testing for bounding aligned box
// DocumentationOf: bounding_alignedbox.h

#include "test_pastelgeometry.h"

#include <pastel/sys/locator.h>
#include <pastel/sys/vector.h>
#include <pastel/sys/input.h>
#include <pastel/sys/rational.h>

#include <pastel/geometry/bounding/bounding_alignedbox.h>

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
			testTransformedSphere();
			testSphere();
			testPoints();
		}

		void testTransformedSphere()
		{
			using Real = real;
			using Point = Vector<Real, 2>;

			{
				Sphere<Real, 2> sphere(Point(0, 0), 4);

				AffineTransformation<Real> transform =
					affineScaling(Point(2, 1));								
				
				AlignedBox<Real, 2> bound =
					boundingAlignedBox(sphere, transform);

				TEST_ENSURE(bound.min() == Point(0 - 2 * 4, 0 - 1 * 4));
				TEST_ENSURE(bound.max() == Point(0 + 2 * 4, 0 + 1 * 4));
			}
			{
				Sphere<Real, 2> sphere(Point(3, 5), 4);

				AffineTransformation<Real> transform =
					affineScaling(Point(2, 1));								
				
				AlignedBox<Real, 2> bound =
					boundingAlignedBox(sphere, transform);

				TEST_ENSURE(bound.min() == Point(3 * 2 - 2 * 4, 5 - 1 * 4));
				TEST_ENSURE(bound.max() == Point(3 * 2 + 2 * 4, 5 + 1 * 4));
			}
			{
				Sphere<Real, 2> sphere(Point(3, 5), 4);

				AffineTransformation<Real> transform =
					affineTranslation(Point(1, -4)) * affineScaling(Point(2, 1));
				
				AlignedBox<Real, 2> bound =
					boundingAlignedBox(sphere, transform);

				TEST_ENSURE(bound.min() == Point((3 * 2 + 1) - 2 * 4, (5 - 4) - 1 * 4));
				TEST_ENSURE(bound.max() == Point((3 * 2 + 1) + 2 * 4, (5 - 4) + 1 * 4));
			}
		}

		void testSphere()
		{
			using Point = Vector2;

			{
				Sphere2 sphere(Point(3, 5), 4);
				AlignedBox2 bound =
					boundingAlignedBox(sphere);
				
				TEST_ENSURE(bound.min() == Point(3 - 4, 5 - 4));
				TEST_ENSURE(bound.max() == Point(3 + 4, 5 + 4));
			}
		}

		void testPoints()
		{
			using Point = Vector2i;

			{
				std::vector<Point> pointSet;

				AlignedBox2i bound =
					boundingAlignedBox(rangeInput(pointSet));

				TEST_ENSURE(bound.min() == Point(infinity<integer>()));
				TEST_ENSURE(bound.max() == Point(-infinity<integer>()));
			}
			{
				std::vector<Point> pointSet =
				{
					{3, 5}
				};

				AlignedBox2i bound =
					boundingAlignedBox(rangeInput(pointSet));

				TEST_ENSURE(bound.min() == Point(3, 5));
				TEST_ENSURE(bound.max() == Point(3, 5));
			}
			{
				// 5  
				// 4  *
				// 3   * *
				// 2  *
				// 1    *
				// 0*
				//  0123456

				std::vector<Point> pointSet =
				{
					{0, 0},
					{2, 2},
					{2, 4},
					{3, 3},
					{4, 1},
					{5, 3}
				};

				AlignedBox2i bound =
					boundingAlignedBox(rangeInput(pointSet));

				TEST_ENSURE(bound.min() == Point(0, 0));
				TEST_ENSURE(bound.max() == Point(5, 4));
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
		testRunner().add("bounding_alignedbox", test);
	}

	CallFunction run(addTest);

}
