// Description: Testing for math functions
// DocumentationOf: math_functions.h

#include "test/test_init.h"

#include "pastel/sys/math_functions.h"

namespace
{

	bool check(dreal measured, dreal correct)
	{
		dreal epsilon = 0.001;
		return REPORT3((absoluteError<dreal>(measured, correct) > epsilon),
			measured, correct, epsilon);
	}

	bool relativeCheck(dreal measured, dreal correct)
	{
		dreal epsilon = 0.001;
		return REPORT3((relativeError<dreal>(measured, correct) > epsilon),
			measured, correct, epsilon);
	}

}

TEST_CASE("math_functions (math_functions)")
{
	REQUIRE(std::abs(relativeError<dreal>(2, 1) - 1) < 0.001);
	REQUIRE(std::abs(relativeError<dreal>(3, 1) - 2) < 0.001);
	REQUIRE(std::abs(relativeError<dreal>(4, 1) - 3) < 0.001);
	REQUIRE(std::abs(relativeError<dreal>(5, 1) - 4) < 0.001);
	REQUIRE(std::abs(relativeError<dreal>(4, 2) - 1) < 0.001);
	REQUIRE(std::abs(relativeError<dreal>(6, 2) - 2) < 0.001);
	REQUIRE(std::abs(relativeError<dreal>(8, 2) - 3) < 0.001);

	check(radiansToDegrees<dreal>(-3 * constantPi<dreal>()), -540);
	check(radiansToDegrees<dreal>(-2 * constantPi<dreal>()), -360);
	check(radiansToDegrees<dreal>(-constantPi<dreal>()), -180);
	check(radiansToDegrees<dreal>(-constantPi<dreal>() / 2), -90);
	check(radiansToDegrees<dreal>(-constantPi<dreal>() / 4), -45);
	check(radiansToDegrees<dreal>(0), 0);
	check(radiansToDegrees<dreal>(constantPi<dreal>() / 4), 45);
	check(radiansToDegrees<dreal>(constantPi<dreal>() / 2), 90);
	check(radiansToDegrees<dreal>(constantPi<dreal>()), 180);
	check(radiansToDegrees<dreal>(2 * constantPi<dreal>()), 360);
	check(radiansToDegrees<dreal>(3 * constantPi<dreal>()), 540);

	check(degreesToRadians<dreal>(-540), -3 * constantPi<dreal>());
	check(degreesToRadians<dreal>(-360), -2 * constantPi<dreal>());
	check(degreesToRadians<dreal>(-180), -constantPi<dreal>());
	check(degreesToRadians<dreal>(-90), -constantPi<dreal>() / 2);
	check(degreesToRadians<dreal>(-45), -constantPi<dreal>() / 4);
	check(degreesToRadians<dreal>(0), 0);
	check(degreesToRadians<dreal>(45), constantPi<dreal>() / 4);
	check(degreesToRadians<dreal>(90), constantPi<dreal>() / 2);
	check(degreesToRadians<dreal>(180), constantPi<dreal>());
	check(degreesToRadians<dreal>(360), 2 * constantPi<dreal>());
	check(degreesToRadians<dreal>(540), 3 * constantPi<dreal>());

	check(positiveDegrees<dreal>(-180), 360 - 180);
	check(positiveDegrees<dreal>(-90), 360 - 90);
	check(positiveDegrees<dreal>(-45), 360 - 45);
	check(positiveDegrees<dreal>(0), 0);
	check(positiveDegrees<dreal>(45), 45);
	check(positiveDegrees<dreal>(90), 90);
	check(positiveDegrees<dreal>(180), 180);

	check(positiveRadians<dreal>(-constantPi<dreal>()), 
		2 * constantPi<dreal>() - constantPi<dreal>());
	check(positiveRadians<dreal>(-constantPi<dreal>() / 2), 
		2 * constantPi<dreal>() - constantPi<dreal>() / 2);
	check(positiveRadians<dreal>(-constantPi<dreal>() / 4), 
		2 * constantPi<dreal>() - constantPi<dreal>() / 4);
	check(positiveRadians<dreal>(0), 0); 
	check(positiveRadians<dreal>(constantPi<dreal>() / 4), 
		constantPi<dreal>() / 4);
	check(positiveRadians<dreal>(constantPi<dreal>() / 2), 
		constantPi<dreal>() / 2);
	check(positiveRadians<dreal>(constantPi<dreal>()), 
		constantPi<dreal>());

	check(sinc<dreal>(-3), 0);
	check(sinc<dreal>(-2), 0);
	check(sinc<dreal>(-1), 0);
	check(sinc<dreal>(0), 1);
	check(sinc<dreal>(1), 0);
	check(sinc<dreal>(2), 0);
	check(sinc<dreal>(3), 0);

	check(signedArea(Vector2(0), Vector2(0, 1), Vector2(1, 0)), -0.5);
	check(signedArea(Vector2(0), Vector2(1, 0), Vector2(0, 1)), 0.5);
	check(signedArea(Vector2(0), Vector2(1, 1), Vector2(1, 0)), -0.5);
	check(signedArea(Vector2(0), Vector2(1, 0), Vector2(1, 1)), 0.5);

	check(ccwAngle(Vector2(1, 0)), 0);
	check(ccwAngle(Vector2(1, 1)), 1 * constantPi<dreal>() / 4);
	check(ccwAngle(Vector2(0, 1)), 2 * constantPi<dreal>() / 4);
	check(ccwAngle(Vector2(-1, 1)), 3 * constantPi<dreal>() / 4);
	check(ccwAngle(Vector2(-1, 0)), 4 * constantPi<dreal>() / 4);
	check(ccwAngle(Vector2(-1, -1)), 5 * constantPi<dreal>() / 4);
	check(ccwAngle(Vector2(0, -1)), 6 * constantPi<dreal>() / 4);
	check(ccwAngle(Vector2(1, -1)), 7 * constantPi<dreal>() / 4);

	{
		dreal correct = 0;

		for (integer i = 1;i < 256;++i)
		{
			correct += (dreal)1 / i;
			check(harmonicNumber<dreal>(i), correct);
		}
	}

	{
		dreal correct = -constantEulerMascheroni<dreal>();

		for (integer i = 1;i < 256;++i)
		{
			check(digamma<dreal>(i), correct);
			correct += (dreal)1 / i;
		}
	}
	{
		dreal correct = 1;
		for (integer i = 1;i < 50;++i)
		{
			relativeCheck(gamma<dreal>(i + 1), correct);
			relativeCheck(lnGamma<dreal>(i + 1), std::log(correct));
			correct *= i + 1;
		}
	}

	check(digamma<dreal>(10000), 9.210290371142850);
}
