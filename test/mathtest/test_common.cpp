#include "pastelmathtest.h"

#include "pastel/sys/random.h"
#include "pastel/sys/math_functions.h"

#include "pastel/geometry/predicates.h"

using namespace Pastel;

namespace
{

	void testCcwAngle()
	{
		REPORT_OP(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(0, 1))), !=, 90);
		REPORT_OP(radiansToDegrees<real>(
			ccwAngle(Vector2(0, 1), Vector2(-1, 0))), !=, 90);
		REPORT_OP(radiansToDegrees<real>(
			ccwAngle(Vector2(-1, 0), Vector2(0, -1))), !=, 90);
		REPORT_OP(radiansToDegrees<real>(
			ccwAngle(Vector2(0, -1), Vector2(1, 0))), !=, 90);

		REPORT_OP(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(1, 1))), !=, 45);
		REPORT_OP(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(-1, 1))), !=, 135);
		REPORT_OP(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(-1, -1))), !=, 225);
		REPORT_OP(radiansToDegrees<real>(
			ccwAngle(Vector2(1, 0), Vector2(1, -1))), !=, 315);
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

			const real aDelta = mabs(aRoot - t0);
			const real bDelta = mabs(bRoot - t1);

			if (aDelta >= 0.1 || bDelta >= 0.1)
			{
				++failings;
			}
		}

		REPORT1(failings > 10, failings);
	}

	void testHarmonic()
	{
		for (integer i = 51200;i < 51250;++i)
		{
			log() << harmonicNumber<real>(i) << ", ";
			if ((i % 4) == 3)
			{
				log() << logNewLine;
			}
		}
	}

	void testSpecial()
	{
		log() << "Gamma function from 1 to 9" << logNewLine;
		for (real i = 1;i < 10;i += 0.5)
		{
			log() << gamma<real>(i) << ", ";
		}
		log() << logNewLine;
		log() << "Digamma function from 1 to 9" << logNewLine;
		for (integer i = 1;i < 10;++i)
		{
			log() << digamma<real>(i) << ", ";
		}
		log() << "Digamma function from 301 to 309" << logNewLine;
		for (integer i = 301;i < 310;++i)
		{
			log() << digamma<real>(i) << ", ";
		}
		log() << logNewLine;
	}

	void testAdd()
	{
		mathTestList().add("Special", testSpecial);
		mathTestList().add("Quadratic", testQuadratic);
		mathTestList().add("CcwAngle", testCcwAngle);
		mathTestList().add("Harmonic", testHarmonic);
	}

	CallFunction run(testAdd);

}
