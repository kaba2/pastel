// Description: Testing for math functions
// DocumentationOf: math_functions.h

#include "test_pastelmath.h"

#include "pastel/sys/random.h"
#include "pastel/sys/math_functions.h"

#include "pastel/geometry/predicate.h"

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
			testCcwAngle();
			testQuadratic();
		}

		void testCcwAngle()
		{
			TEST_ENSURE_OP(radiansToDegrees<real>(
				ccwAngle(Vector2(1, 0), Vector2(0, 1))) - 90, <, 0.001);
			TEST_ENSURE_OP(radiansToDegrees<real>(
				ccwAngle(Vector2(0, 1), Vector2(-1, 0))) - 90, <, 0.001);
			TEST_ENSURE_OP(radiansToDegrees<real>(
				ccwAngle(Vector2(-1, 0), Vector2(0, -1))) - 90, < , 0.001);
			TEST_ENSURE_OP(radiansToDegrees<real>(
				ccwAngle(Vector2(0, -1), Vector2(1, 0))) - 90, <, 0.001);

			TEST_ENSURE_OP(radiansToDegrees<real>(
				ccwAngle(Vector2(1, 0), Vector2(1, 1))) - 45, <, 0.001);
			TEST_ENSURE_OP(radiansToDegrees<real>(
				ccwAngle(Vector2(1, 0), Vector2(-1, 1))) - 135, <, 0.001);
			TEST_ENSURE_OP(radiansToDegrees<real>(
				ccwAngle(Vector2(1, 0), Vector2(-1, -1))) - 225, <, 0.001);
			TEST_ENSURE_OP(radiansToDegrees<real>(
				ccwAngle(Vector2(1, 0), Vector2(1, -1))) - 315, <, 0.001);
		}

		void testQuadratic()
		{
			integer failings = 0;

			for (integer i = 0;i < 10000;++i)
			{
				real aRoot = random<real>() * 100 - 50;
				real bRoot = random<real>() * 100 - 50;

				if (bRoot < aRoot)
				{
					std::swap(aRoot, bRoot);
				}

				const real aCoeff = random<real>() * 100 + 2;
				const real bCoeff = (-aRoot - bRoot) * aCoeff;
				const real cCoeff = (aRoot * bRoot) * aCoeff;

				real t0 = 0;
				real t1 = 0;

				quadratic(aCoeff, bCoeff, cCoeff, t0, t1);

				real aDelta = mabs(aRoot - t0);
				real bDelta = mabs(bRoot - t1);

				if (aDelta >= 0.1 || bDelta >= 0.1)
				{
					++failings;
				}
			}

			TEST_ENSURE_OP(failings, <=, 10);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Common", test);
	}

	CallFunction run(addTest);

}
