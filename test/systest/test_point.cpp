// Description: Testing for points
// DocumentationOf: point_concept.h

#include "test_pastelsys.h"

#include <pastel/sys/point_concept.h>
#include <pastel/sys/locators.h>

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
			testLocator();
			testReal();
			testAxis();
		}

		void testLocator()
		{
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Locator<real*>, Pointer_Locator<real>>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Locator<const real*>, Pointer_Locator<const real>>::value));
		}

		void testReal()
		{
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
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<real*, integer*>, real>::value));
			PASTEL_STATIC_ASSERT(
				(std::is_same<Point_Real<integer*, real*>, real>::value));
		}

		void testAxis()
		{
			std::array<int, 2> x = {{1, 2}};
			
			TEST_ENSURE_OP(axis(x, 0), ==, x[0]);
			TEST_ENSURE_OP(axis(x, 1), ==, x[1]);

			TEST_ENSURE_OP(axis(&x[0], 0), ==, x[0]);
			TEST_ENSURE_OP(axis(&x[0], 1), ==, x[1]);

			TEST_ENSURE_OP(axis(arrayPoint(x), 0), ==, x[0]);
			TEST_ENSURE_OP(axis(arrayPoint(x), 1), ==, x[1]);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("point_concept", test);
	}

	CallFunction run(addTest);

}
