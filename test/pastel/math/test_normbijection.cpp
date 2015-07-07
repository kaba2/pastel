// Description: Testing for norm-bijections
// DocumentationOf: normbijection.h

#include "test/test_init.h"

#include <pastel/math/normbijection.h>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testEuclidean();
			testManhattan();
			testMaximum();
		}

		void testEuclidean()
		{
			using NormBijection = Euclidean_NormBijection<real>;
			PASTEL_CONCEPT_CHECK(NormBijection, NormBijection_Concept);

			NormBijection n;

			real valueSet[] = { 0, 1, 2, 3.5, 5, 7, 9.25, 13 };

			for (auto&& x : valueSet)
			{
				REQUIRE(n.axis(x) == x * x);
				REQUIRE(n.signedAxis(x) == x * x);
				REQUIRE(n.signedAxis(-x) == (-x) * (-x));
				REQUIRE(n.addAxis(100, x) == 100 + x);
				REQUIRE(n.replaceAxis(100, 10, x) == (100 - 10) + x);
				REQUIRE(n.toNorm(x * x) == x);
				REQUIRE(n.toLnNorm(x * x) == std::log(x));
				REQUIRE(n.toBijection(x) == x * x);
				REQUIRE(n.scalingFactor(x) == x * x);
			}

			REQUIRE(n.lnVolumeUnitSphere(1) == std::log(2));
			REQUIRE(n.lnVolumeUnitSphere(2) == std::log(constantPi<real>()));
		}

		void testManhattan()
		{
			using NormBijection = Manhattan_NormBijection<real>;
			PASTEL_CONCEPT_CHECK(NormBijection, NormBijection_Concept);

			NormBijection n;

			real valueSet[] = { 0, 1, 2, 3.5, 5, 7, 9.25, 13 };

			for (auto&& x : valueSet)
			{
				REQUIRE(n.axis(x) == x);
				REQUIRE(n.signedAxis(x) == x);
				REQUIRE(n.signedAxis(-x) == x);
				REQUIRE(n.addAxis(100, x) == 100 + x);
				REQUIRE(n.replaceAxis(100, 10, x) == (100 - 10) + x);
				REQUIRE(n.toNorm(x) == x);
				REQUIRE(n.toLnNorm(x) == std::log(x));
				REQUIRE(n.toBijection(x) == x);
				REQUIRE(n.scalingFactor(x) == x);
			}

			REQUIRE(n.lnVolumeUnitSphere(1) == std::log(2));
			REQUIRE(n.lnVolumeUnitSphere(2) == std::log(2));
		}

		void testMaximum()
		{
			using NormBijection = Maximum_NormBijection<real>;
			PASTEL_CONCEPT_CHECK(NormBijection, NormBijection_Concept);

			NormBijection n;

			real valueSet[] = { 0, 1, 2, 3.5, 5, 7, 9.25, 13 };

			for (auto&& x : valueSet)
			{
				REQUIRE(n.axis(x) == x);
				REQUIRE(n.signedAxis(x) == x);
				REQUIRE(n.signedAxis(-x) == x);
				REQUIRE(n.addAxis(100, x) == std::max((real)100, x));
				REQUIRE(n.replaceAxis(100, 10, x) ==  std::max((real)100, x));
				REQUIRE(n.toNorm(x) == x);
				REQUIRE(n.toLnNorm(x) == std::log(x));
				REQUIRE(n.toBijection(x) == x);
				REQUIRE(n.scalingFactor(x) == x);
			}

			REQUIRE(n.lnVolumeUnitSphere(1) == std::log(2));
			REQUIRE(n.lnVolumeUnitSphere(2) == std::log(4));
		}
	};

	TEST_CASE("NormBijection", "[NormBijection]")
	{
	}

}
