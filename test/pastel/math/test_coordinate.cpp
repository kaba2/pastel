// Description: Testing for coordinates
// Documentation: coordinate.h

#include "test/test_init.h"

#include <pastel/math/coordinate.h>

namespace
{

	class Test
	{
	public:
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

			REQUIRE(f({ 1, 0, 0 }, { 1, 0, 0 }));
			REQUIRE(f({ 1, constantPi<real>() / 2, 0 }, { 0, 1, 0 }));
			REQUIRE(f({ 1, constantPi<real>() / 4, 0 }, { sqrt(2) / 2, sqrt(2) / 2, 0 }));
			REQUIRE(f({ 1, constantPi<real>() / 4, constantPi<real>() / 2}, { sqrt(2) / 2, 0, sqrt(2) / 2 }));

			REQUIRE(f({ 2, 0, 0 }, { 2, 0, 0 }));
			REQUIRE(f({ 2, constantPi<real>() / 2, 0 }, { 0, 2, 0 }));
			REQUIRE(f({ 2, constantPi<real>() / 4, 0 }, { 2 * sqrt(2) / 2, 2 * sqrt(2) / 2, 0 }));
			REQUIRE(f({ 2, constantPi<real>() / 4, constantPi<real>() / 2}, { 2 * sqrt(2) / 2, 0, 2 * sqrt(2) / 2 }));

			auto g = [](Vector2 spherical, Vector2 cartesian)
			{
				return norm(sphericalToCartesian(spherical) - cartesian) < 0.001;
			};

			REQUIRE(g({ 1, 0 }, { 1, 0 }));
			REQUIRE(g({ 1, constantPi<real>() / 2 }, { 0, 1 }));
			REQUIRE(g({ 1, constantPi<real>() }, {-1, 0}));
			REQUIRE(g({ 1, 3 * constantPi<real>() / 2 }, { 0, -1 }));

			REQUIRE(g({ 2, 0 }, { 2, 0 }));
			REQUIRE(g({ 2, constantPi<real>() / 2 }, { 0, 2 }));
			REQUIRE(g({ 2, constantPi<real>() }, {-2, 0}));
			REQUIRE(g({ 2, 3 * constantPi<real>() / 2 }, { 0, -2 }));

			auto h = [](Vector2 cartesian)
			{
				return norm(sphericalToCartesian(cartesianToSpherical(cartesian)) - cartesian) < 0.001;
			};

			REQUIRE(h({ 1, 0 }));
			REQUIRE(h({ 1, 1 }));
			REQUIRE(h({ 0, 1 }));
			REQUIRE(h({ -1, 1 }));
			REQUIRE(h({ -1, 0 }));
			REQUIRE(h({ -1, -1 }));
			REQUIRE(h({ 0, -1 }));
			REQUIRE(h({ 1, -1 }));
		}
	};

	TEST_CASE("coordinate", "[coordinate]")
	{
	}

}
