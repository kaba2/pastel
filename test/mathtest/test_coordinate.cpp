// Description: Testing for coordinates
// Documentation: coordinate.h

#include "test_pastelmath.h"

#include <pastel/math/coordinate.h>

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
			test();
		}

		void test()
		{
			auto f = [](Vector3 spherical, Vector3 cartesian)
			{
				return norm(sphericalToCartesian(spherical) - cartesian) < 0.001;
			};

			TEST_ENSURE(f({ 1, 0, 0 }, { 1, 0, 0 }));
			TEST_ENSURE(f({ 1, constantPi<real>() / 2, 0 }, { 0, 1, 0 }));
			TEST_ENSURE(f({ 1, constantPi<real>() / 4, 0 }, { sqrt(2) / 2, sqrt(2) / 2, 0 }));
			TEST_ENSURE(f({ 1, constantPi<real>() / 4, constantPi<real>() / 2}, { sqrt(2) / 2, 0, sqrt(2) / 2 }));

			TEST_ENSURE(f({ 2, 0, 0 }, { 2, 0, 0 }));
			TEST_ENSURE(f({ 2, constantPi<real>() / 2, 0 }, { 0, 2, 0 }));
			TEST_ENSURE(f({ 2, constantPi<real>() / 4, 0 }, { 2 * sqrt(2) / 2, 2 * sqrt(2) / 2, 0 }));
			TEST_ENSURE(f({ 2, constantPi<real>() / 4, constantPi<real>() / 2}, { 2 * sqrt(2) / 2, 0, 2 * sqrt(2) / 2 }));

			auto g = [](Vector2 spherical, Vector2 cartesian)
			{
				return norm(sphericalToCartesian(spherical) - cartesian) < 0.001;
			};

			TEST_ENSURE(g({ 1, 0 }, { 1, 0 }));
			TEST_ENSURE(g({ 1, constantPi<real>() / 2 }, { 0, 1 }));
			TEST_ENSURE(g({ 1, constantPi<real>() }, {-1, 0}));
			TEST_ENSURE(g({ 1, 3 * constantPi<real>() / 2 }, { 0, -1 }));

			TEST_ENSURE(g({ 2, 0 }, { 2, 0 }));
			TEST_ENSURE(g({ 2, constantPi<real>() / 2 }, { 0, 2 }));
			TEST_ENSURE(g({ 2, constantPi<real>() }, {-2, 0}));
			TEST_ENSURE(g({ 2, 3 * constantPi<real>() / 2 }, { 0, -2 }));

			auto h = [](Vector2 cartesian)
			{
				return norm(sphericalToCartesian(cartesianToSpherical(cartesian)) - cartesian) < 0.001;
			};

			TEST_ENSURE(h({ 1, 0 }));
			TEST_ENSURE(h({ 1, 1 }));
			TEST_ENSURE(h({ 0, 1 }));
			TEST_ENSURE(h({ -1, 1 }));
			TEST_ENSURE(h({ -1, 0 }));
			TEST_ENSURE(h({ -1, -1 }));
			TEST_ENSURE(h({ 0, -1 }));
			TEST_ENSURE(h({ 1, -1 }));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("coordinate", test);
	}

	CallFunction run(addTest);

}
