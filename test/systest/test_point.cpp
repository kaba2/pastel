// Description: Testing for points
// DocumentationOf: point_concept.h

#include "test_pastelsys.h"

#include <pastel/sys/point.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/rational.h>

namespace Pastel
{

	template <typename Real>
	class Default_Locator<Real* const&>
	{
	public:
		Pointer_Locator<Real> operator()(Real* point)
		{
			return Pointer_Locator<Real>(2);
		}
	};

}

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
			testPoint();
			testLocator();
			testReal();
			testAxis();
			testDimension();
		}

		void testPoint()
		{
			// Default-locator for native arithmetic types.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Point<integer>, integer>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Point<real>, real>::value));

			// Default-locator for rational numbers.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Point<Rational<integer>>, Rational<integer>>::value));

			// Default-locator for pointers, which we defined above.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Point<real*>, real*>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Point<const real*>, const real*>::value));
		}

		void testLocator()
		{
			// Default-locator for native arithmetic types.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Locator<integer>, Scalar_Locator<integer>>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Locator<real>, Scalar_Locator<real>>::value));

			// Default-locator for rational numbers.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Locator<Rational<integer>>, Scalar_Locator<Rational<integer>>>::value));

			// Default-locator for pointers, which we defined above.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Locator<real*>, Pointer_Locator<real>>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Locator<const real*>, Pointer_Locator<const real>>::value));
		}

		void testReal()
		{
			// Default-locator for native arithmetic types.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<integer>, integer>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<real>, real>::value));

			// Default-locator for rational numbers.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<Rational<integer>>, Rational<integer>>::value));

			// Default-locator for pointers, which we defined above.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<real*>, real>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<real*>, real>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<real* const &>, real>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<real* const &>, real>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<const real*>, real>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<const real* const>, real>::value));

			// Multiple points. Here the type should be promoted
			// to the one which can hold them all.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<real*, integer*>, real>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<integer*, real*>, real>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<char, short, long>, long>::value));
		}

		void testAxis()
		{
			{
				std::array<int, 2> x = { {1, 2} };

				TEST_ENSURE_OP(axis(x, 0), == , x[0]);
				TEST_ENSURE_OP(axis(x, 1), == , x[1]);

				TEST_ENSURE_OP(axis(&x[0], 0), == , x[0]);
				TEST_ENSURE_OP(axis(&x[0], 1), == , x[1]);

				TEST_ENSURE_OP(axis(arrayPoint(x), 0), == , x[0]);
				TEST_ENSURE_OP(axis(arrayPoint(x), 1), == , x[1]);
			}
			{
				real x = 5;

				TEST_ENSURE_OP(axis(x, 0), == , 5);
				TEST_ENSURE_OP(axis(x, 1), == , 5);
			}
			{
				Vector<int, 2> x = { 1, 2 };

				TEST_ENSURE_OP(axis(x, 0), == , x[0]);
				TEST_ENSURE_OP(axis(x, 1), == , x[1]);

				TEST_ENSURE_OP(axis(vectorPoint(x), 0), == , x[0]);
				TEST_ENSURE_OP(axis(vectorPoint(x), 1), == , x[1]);
			}
			{
				Vector<int> x(ofDimension(2));

				TEST_ENSURE_OP(axis(x, 0), == , x[0]);
				TEST_ENSURE_OP(axis(x, 1), == , x[1]);

				TEST_ENSURE_OP(axis(vectorPoint(x), 0), == , x[0]);
				TEST_ENSURE_OP(axis(vectorPoint(x), 1), == , x[1]);
			}
		}

		void testDimension()
		{
			{
				std::array<int, 2> x = { {1, 2} };
				TEST_ENSURE_OP(dimension(x), == , 2);
			}
			{
				real x = 5;
				TEST_ENSURE_OP(dimension(x), == , 1);
			}
			{
				Vector<int, 2> x = { 1, 2 };
				TEST_ENSURE_OP(dimension(x), == , 2);
			}
			{
				Vector<int> x(ofDimension(2));
				TEST_ENSURE_OP(dimension(x), == , 2);
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
		testRunner().add("point", test);
	}

	CallFunction run(addTest);

}
