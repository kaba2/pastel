#include "pastelmathtest.h"

#include "pastel/sys/random.h"
#include "pastel/sys/mathfunctions.h"

#include "pastel/math/mathcommon.h"

#include "pastel/geometry/predicates.h"

using namespace Pastel;

namespace
{

	void testCcwAngle()
	{
		REPORT(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(0, 1), 1, 1)) != 90);
		REPORT(radiansToDegrees<real>(
			ccwAngle(Vector2(0, 1), Vector2(-1, 0), 1, 1)) != 90);
		REPORT(radiansToDegrees<real>(
			ccwAngle(Vector2(-1, 0), Vector2(0, -1), 1, 1)) != 90);
		REPORT(radiansToDegrees<real>(
			ccwAngle(Vector2(0, -1), Vector2(1, 0), 1, 1)) != 90);

		REPORT(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(1, 1), 1, std::sqrt((real)2))) != 45);
		REPORT(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(-1, 1), 1, std::sqrt((real)2))) != 135);
		REPORT(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(-1, -1), 1, std::sqrt((real)2))) != 225);
		REPORT(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(1, -1), 1, std::sqrt((real)2))) != 315);
	}

	void testQuadratic()
	{
		integer failings = 0;

		for (integer i = 0;i < 10000;++i)
		{
			real aRoot = randomReal() * 100 - 50;
			real bRoot = randomReal() * 100 - 50;

			if (bRoot < aRoot)
			{
				std::swap(aRoot, bRoot);
			}

			const real aCoeff = randomReal() * 100 + 2;
			const real bCoeff = (-aRoot - bRoot) * aCoeff;
			const real cCoeff = (aRoot * bRoot) * aCoeff;

			real t0 = 0;
			real t1 = 0;

			quadratic(aCoeff, bCoeff, cCoeff, t0, t1);

			const real aDelta = std::abs(aRoot - t0);
			const real bDelta = std::abs(bRoot - t1);

			if (aDelta >= 0.1 || bDelta >= 0.1)
			{
				++failings;
			}
		}

		REPORT1(failings > 10, failings);
	}

	void testBegin()
	{
		testQuadratic();
		testCcwAngle();
	}

	void testAdd()
	{
		mathTestList().add("Common", testBegin);
	}

	CallFunction run(testAdd);

}
