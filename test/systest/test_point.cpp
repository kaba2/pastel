// Description: Testing for points
// DocumentationOf: point_concept.h

#include "test_pastelsys.h"

#include <pastel/sys/point.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/rational.h>
#include <pastel/sys/vector.h>

namespace Pastel
{

	template <typename Real>
	class Default_Locator<Real* const&, void>
	{
	public:
		using Point = Real*;
		using Locator = Pointer_Locator<Real>;

		Locator operator()(Point point)
		{
			return Locator(2);
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
			testConcept();
			testAsVector();
			testPoint();
			testLocator();
			testReal();
			testAxis();
			testDimension();
		}

		void testConcept()
		{
			PASTEL_CONCEPT_CHECK(real*, Point_Concept);
			PASTEL_STATIC_ASSERT((Models<std::array<real, 2>, Point_Concept>::value));
			PASTEL_STATIC_ASSERT((Models<Vector<real, 2>, Point_Concept>::value));

			PASTEL_CONCEPT_CHECK(real, Point_Concept);
			PASTEL_CONCEPT_CHECK(integer, Point_Concept);
			PASTEL_CONCEPT_CHECK(Rational<integer>, Point_Concept);
			PASTEL_STATIC_ASSERT((Models<Location<real, Scalar_Locator<real>>, Point_Concept>::value));

			struct Something_Else {};

			PASTEL_STATIC_ASSERT(!HasDefaultLocator<Something_Else>::value);
			PASTEL_STATIC_ASSERT(HasDefaultLocator<real*>::value);
			PASTEL_CONCEPT_REJECT(Something_Else, Point_Concept);

			PASTEL_STATIC_ASSERT((EqualDimension<Integer<1>, Point_N<Vector<real, 1>>>::value));
			PASTEL_STATIC_ASSERT((EqualDimension<Integer<1>, Point_N<Vector<real, -1>>>::value));
			PASTEL_STATIC_ASSERT((EqualDimension<Integer<-1>, Point_N<Vector<real, 1>>>::value));
			PASTEL_STATIC_ASSERT((EqualDimension<Integer<-1>, Point_N<Vector<real, -1>>>::value));

			PASTEL_STATIC_ASSERT((EqualDimension_C<1, 1>::value));
			PASTEL_STATIC_ASSERT((EqualDimension_C<0, 0>::value));
			
			PASTEL_STATIC_ASSERT((EqualDimension_C<0, 0>::value));
			PASTEL_STATIC_ASSERT((EqualDimension_C<0, -1>::value));
			PASTEL_STATIC_ASSERT((EqualDimension_C<-1, 0>::value));
			PASTEL_STATIC_ASSERT((EqualDimension_C<-1, -1>::value));
			
			PASTEL_STATIC_ASSERT((EqualDimension_C<5, 5>::value));
			PASTEL_STATIC_ASSERT((EqualDimension_C<-1, 5>::value));
			PASTEL_STATIC_ASSERT((EqualDimension_C<5, -1>::value));

			PASTEL_STATIC_ASSERT(!(EqualDimension_C<0, 1>::value));
			PASTEL_STATIC_ASSERT(!(EqualDimension_C<1, 0>::value));
			PASTEL_STATIC_ASSERT(!(EqualDimension_C<1, 2>::value));
			PASTEL_STATIC_ASSERT(!(EqualDimension_C<2, 1>::value));
		}

		void testAsVector()
		{
			std::array<real, 2> p = { {1, 2} };
			Vector<real, 2> q = pointAsVector(p);
			TEST_ENSURE((q == Vector<real, 2>(1, 2)));
		}

		void testPoint()
		{
			// Default-locator for native arithmetic types.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_PointId<integer>, integer>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_PointId<real>, real>::value));

			// Default-locator for rational numbers.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_PointId<Rational<integer>>, Rational<integer>>::value));

			// Default-locator for pointers, which we defined above.
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_PointId<real*>, real*>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_PointId<const real*>, const real*>::value));
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

				TEST_ENSURE_OP(pointAxis(x, 0), == , x[0]);
				TEST_ENSURE_OP(pointAxis(x, 1), == , x[1]);

				TEST_ENSURE_OP(pointAxis(&x[0], 0), == , x[0]);
				TEST_ENSURE_OP(pointAxis(&x[0], 1), == , x[1]);

				TEST_ENSURE_OP(pointAxis(arrayPoint(x), 0), == , x[0]);
				TEST_ENSURE_OP(pointAxis(arrayPoint(x), 1), == , x[1]);
			}
			{
				real x = 5;

				TEST_ENSURE_OP(pointAxis(x, 0), == , 5);
				TEST_ENSURE_OP(pointAxis(x, 1), == , 5);
			}
			{
				Vector<int, 2> x = { 1, 2 };

				TEST_ENSURE_OP(pointAxis(x, 0), == , x[0]);
				TEST_ENSURE_OP(pointAxis(x, 1), == , x[1]);

				TEST_ENSURE_OP(pointAxis(vectorPoint(x), 0), == , x[0]);
				TEST_ENSURE_OP(pointAxis(vectorPoint(x), 1), == , x[1]);
			}
			{
				Vector<int> x(ofDimension(2));

				TEST_ENSURE_OP(pointAxis(x, 0), == , x[0]);
				TEST_ENSURE_OP(pointAxis(x, 1), == , x[1]);

				TEST_ENSURE_OP(pointAxis(vectorPoint(x), 0), == , x[0]);
				TEST_ENSURE_OP(pointAxis(vectorPoint(x), 1), == , x[1]);
			}
		}

		void testDimension()
		{
			{
				using Point = std::array<int, 2>;
				Point x = { {1, 2} };
				TEST_ENSURE_OP(dimension(x), == , 2);
				PASTEL_STATIC_ASSERT(Point_N<Point>::value == 2);
			}
			{
				using Point = real;
				Point x = 5;
				TEST_ENSURE_OP(dimension(x), == , 1);
				PASTEL_STATIC_ASSERT(Point_N<Point>::value == 1);
			}
			{
				using Point = Vector<int, 2>;
				Point x = { 1, 2 };
				TEST_ENSURE_OP(dimension(x), == , 2);
				PASTEL_STATIC_ASSERT(Point_N<Point>::value == 2);
			}
			{
				using Point = Vector<int>;
				Point x(ofDimension(2));
				TEST_ENSURE_OP(dimension(x), == , 2);
				PASTEL_STATIC_ASSERT(Point_N<Point>::value == Dynamic);
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
