// Description: Testing for distance between points
// DocumentationOf: distance_point_point.h

#include "test_pastelgeometry.h"

#include "pastel/sys/locators.h"

#include <pastel/geometry/distance_point_point.h>

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
			testCustom();
			testPointer();
			testArray();
			testVector();
		}

		class Custom_Point
		{
		public:
			Custom_Point(real x_, real y_)
				: x(x_)
				, y(y_)
			{

			}

			real x;
			real y;
		};

		class Custom_Locator
		{
		public:
			static PASTEL_CONSTEXPR integer N = 2;
			using Real = real;
			using Point = Custom_Point;

			const real& operator()(const Point& point, integer i) const
			{
				return i != 0 ? point.y : point.x;
			}

			integer n() const
			{
				return N;
			}
		};

		void testCustom()
		{
			Custom_Point a(1, 4);
			Custom_Point b(-5, 2);
			Custom_Locator locator;

			TEST_ENSURE(manhattanDistance(a, b, locator, locator) == 6 + 2);
		}

		void testPointer()
		{
			using Point = real*;

			real data[] = { 1, 4, -5, 2 };
		
			Point a = &data[0];
			Point b = &data[2];
			auto locator = Pointer_Locator<real, 2>();

			testCase(a, b, locator);
		}

		void testArray()
		{
			using Point = std::array<real, 2>;

			Point a = { 1, 4 };
			Point b = { -5, 2 };
			auto locator = Array_Locator<Point>();

			testCase(a, b, locator);
		}

		void testVector()
		{
			using Point = Vector<real, 2>;

			Point a(1, 4);
			Point b(-5, 2);
			auto locator = Vector_Locator<real, 2>();

			testCase(a, b, locator);
		}

		template <
			typename Point,
			typename Locator>
		void testCase(
			const Point& a,
			const Point& b,
			const Locator& locator)
		{
			using Real = typename Locator::Real;

			auto keepGoing = [&](const Real& that)
			{
				return that < 6;
			};

			{
				Real correct = 6 + 2;

				TEST_ENSURE(manhattanDistance(a, b) == correct);
				TEST_ENSURE(manhattanDistance(b, a) == correct);
				TEST_ENSURE(manhattanDistance(a, b, locator, locator, keepGoing) == 6);
			}

			{
				Real correct = std::max(6, 2);

				TEST_ENSURE(maximumDistance(a, b) == correct);
				TEST_ENSURE(maximumDistance(b, a) == correct);
				TEST_ENSURE(maximumDistance(a, b, locator, locator, keepGoing) == 6);
			}

			{
				Real correct = std::sqrt(square(6) + square(2));

				TEST_ENSURE(euclideanDistance(a, b) == correct);
				TEST_ENSURE(euclideanDistance(b, a) == correct);
				TEST_ENSURE(euclideanDistance(a, b, locator, locator, keepGoing) == 6);
			}

			{
				Real correct = std::sqrt(square(6) + square(2));

				TEST_ENSURE(minkowskiDistance(a, b, 2) == correct);
				TEST_ENSURE(minkowskiDistance(b, a, 2) == correct);
				TEST_ENSURE(minkowskiDistance(a, b, 2, locator, locator, keepGoing) == 6);
			}

			{
				Real correct = square(6) + square(2);

				TEST_ENSURE(euclideanDistance2(a, b) == correct);
				TEST_ENSURE(euclideanDistance2(b, a) == correct);
				TEST_ENSURE(euclideanDistance2(a, b, locator, locator, keepGoing) == square(6));
			}

			{
				Real correct = 6 + 2;

				TEST_ENSURE(minkowskiDistance2(a, b, 1) == correct);
				TEST_ENSURE(minkowskiDistance2(b, a, 1) == correct);
				TEST_ENSURE(minkowskiDistance2(a, b, 1, locator, locator, keepGoing) == 6);
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
		testRunner().add("distance_point_point", test);
	}

	CallFunction run(addTest);

}
