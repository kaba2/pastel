// Description: Testing for math functions
// DocumentationOf: math_functions.h

#include "test/test_init.h"

#include "pastel/sys/math_functions.h"

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			test();
		}

		bool check(real measured, real correct)
		{
			real epsilon = 0.001;
			return REPORT3((absoluteError<real>(measured, correct) > epsilon),
				measured, correct, epsilon);
		}

		bool relativeCheck(real measured, real correct)
		{
			real epsilon = 0.001;
			return REPORT3((relativeError<real>(measured, correct) > epsilon),
				measured, correct, epsilon);
		}

		void test()
		{
			REQUIRE(std::abs(relativeError<real>(2, 1) - 1) < 0.001);
			REQUIRE(std::abs(relativeError<real>(3, 1) - 2) < 0.001);
			REQUIRE(std::abs(relativeError<real>(4, 1) - 3) < 0.001);
			REQUIRE(std::abs(relativeError<real>(5, 1) - 4) < 0.001);
			REQUIRE(std::abs(relativeError<real>(4, 2) - 1) < 0.001);
			REQUIRE(std::abs(relativeError<real>(6, 2) - 2) < 0.001);
			REQUIRE(std::abs(relativeError<real>(8, 2) - 3) < 0.001);

			check(radiansToDegrees<real>(-3 * constantPi<real>()), -540);
			check(radiansToDegrees<real>(-2 * constantPi<real>()), -360);
			check(radiansToDegrees<real>(-constantPi<real>()), -180);
			check(radiansToDegrees<real>(-constantPi<real>() / 2), -90);
			check(radiansToDegrees<real>(-constantPi<real>() / 4), -45);
			check(radiansToDegrees<real>(0), 0);
			check(radiansToDegrees<real>(constantPi<real>() / 4), 45);
			check(radiansToDegrees<real>(constantPi<real>() / 2), 90);
			check(radiansToDegrees<real>(constantPi<real>()), 180);
			check(radiansToDegrees<real>(2 * constantPi<real>()), 360);
			check(radiansToDegrees<real>(3 * constantPi<real>()), 540);

			check(degreesToRadians<real>(-540), -3 * constantPi<real>());
			check(degreesToRadians<real>(-360), -2 * constantPi<real>());
			check(degreesToRadians<real>(-180), -constantPi<real>());
			check(degreesToRadians<real>(-90), -constantPi<real>() / 2);
			check(degreesToRadians<real>(-45), -constantPi<real>() / 4);
			check(degreesToRadians<real>(0), 0);
			check(degreesToRadians<real>(45), constantPi<real>() / 4);
			check(degreesToRadians<real>(90), constantPi<real>() / 2);
			check(degreesToRadians<real>(180), constantPi<real>());
			check(degreesToRadians<real>(360), 2 * constantPi<real>());
			check(degreesToRadians<real>(540), 3 * constantPi<real>());

			check(positiveDegrees<real>(-180), 360 - 180);
			check(positiveDegrees<real>(-90), 360 - 90);
			check(positiveDegrees<real>(-45), 360 - 45);
			check(positiveDegrees<real>(0), 0);
			check(positiveDegrees<real>(45), 45);
			check(positiveDegrees<real>(90), 90);
			check(positiveDegrees<real>(180), 180);

			check(positiveRadians<real>(-constantPi<real>()), 
				2 * constantPi<real>() - constantPi<real>());
			check(positiveRadians<real>(-constantPi<real>() / 2), 
				2 * constantPi<real>() - constantPi<real>() / 2);
			check(positiveRadians<real>(-constantPi<real>() / 4), 
				2 * constantPi<real>() - constantPi<real>() / 4);
			check(positiveRadians<real>(0), 0); 
			check(positiveRadians<real>(constantPi<real>() / 4), 
				constantPi<real>() / 4);
			check(positiveRadians<real>(constantPi<real>() / 2), 
				constantPi<real>() / 2);
			check(positiveRadians<real>(constantPi<real>()), 
				constantPi<real>());

			check(sinc<real>(-3), 0);
			check(sinc<real>(-2), 0);
			check(sinc<real>(-1), 0);
			check(sinc<real>(0), 1);
			check(sinc<real>(1), 0);
			check(sinc<real>(2), 0);
			check(sinc<real>(3), 0);

			check(signedArea(Vector2(0), Vector2(0, 1), Vector2(1, 0)), -0.5);
			check(signedArea(Vector2(0), Vector2(1, 0), Vector2(0, 1)), 0.5);
			check(signedArea(Vector2(0), Vector2(1, 1), Vector2(1, 0)), -0.5);
			check(signedArea(Vector2(0), Vector2(1, 0), Vector2(1, 1)), 0.5);

			check(ccwAngle(Vector2(1, 0)), 0);
			check(ccwAngle(Vector2(1, 1)), 1 * constantPi<real>() / 4);
			check(ccwAngle(Vector2(0, 1)), 2 * constantPi<real>() / 4);
			check(ccwAngle(Vector2(-1, 1)), 3 * constantPi<real>() / 4);
			check(ccwAngle(Vector2(-1, 0)), 4 * constantPi<real>() / 4);
			check(ccwAngle(Vector2(-1, -1)), 5 * constantPi<real>() / 4);
			check(ccwAngle(Vector2(0, -1)), 6 * constantPi<real>() / 4);
			check(ccwAngle(Vector2(1, -1)), 7 * constantPi<real>() / 4);

			{
				real correct = 0;

				for (integer i = 1;i < 256;++i)
				{
					correct += (real)1 / i;
					check(harmonicNumber<real>(i), correct);
				}
			}

			{
				real correct = -constantEulerMascheroni<real>();

				for (integer i = 1;i < 256;++i)
				{
					check(digamma<real>(i), correct);
					correct += (real)1 / i;
				}
			}
			{
				real correct = 1;
				for (integer i = 1;i < 50;++i)
				{
					relativeCheck(gamma<real>(i + 1), correct);
					relativeCheck(lnGamma<real>(i + 1), std::log(correct));
					correct *= i + 1;
				}
			}

			check(digamma<real>(10000), 9.210290371142850);
		}
	};

}
