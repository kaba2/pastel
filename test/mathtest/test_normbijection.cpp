// Description: Testing for norm-bijections
// DocumentationOf: normbijection.h

#include "test_pastelmath.h"

#include <pastel/math/normbijection.h>

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
				TEST_ENSURE_OP(n.axis(x), == , x * x);
				TEST_ENSURE_OP(n.signedAxis(x), == , x * x);
				TEST_ENSURE_OP(n.signedAxis(-x), == , (-x) * (-x));
				TEST_ENSURE_OP(n.addAxis(100, x), == , 100 + x);
				TEST_ENSURE_OP(n.replaceAxis(100, 10, x), == , (100 - 10) + x);
				TEST_ENSURE_OP(n.toNorm(x * x), == , x);
				TEST_ENSURE_OP(n.toLnNorm(x * x), == , std::log(x));
				TEST_ENSURE_OP(n.toBijection(x), == , x * x);
				TEST_ENSURE_OP(n.scalingFactor(x), == , x * x);
			}

			TEST_ENSURE_OP(n.lnVolumeUnitSphere(1), == , std::log(2));
			TEST_ENSURE_OP(n.lnVolumeUnitSphere(2), == , std::log(constantPi<real>()));
		}

		void testManhattan()
		{
			using NormBijection = Manhattan_NormBijection<real>;
			PASTEL_CONCEPT_CHECK(NormBijection, NormBijection_Concept);

			NormBijection n;

			real valueSet[] = { 0, 1, 2, 3.5, 5, 7, 9.25, 13 };

			for (auto&& x : valueSet)
			{
				TEST_ENSURE_OP(n.axis(x), == , x);
				TEST_ENSURE_OP(n.signedAxis(x), == , x);
				TEST_ENSURE_OP(n.signedAxis(-x), == , x);
				TEST_ENSURE_OP(n.addAxis(100, x), == , 100 + x);
				TEST_ENSURE_OP(n.replaceAxis(100, 10, x), == , (100 - 10) + x);
				TEST_ENSURE_OP(n.toNorm(x), == , x);
				TEST_ENSURE_OP(n.toLnNorm(x), == , std::log(x));
				TEST_ENSURE_OP(n.toBijection(x), == , x);
				TEST_ENSURE_OP(n.scalingFactor(x), == , x);
			}

			TEST_ENSURE_OP(n.lnVolumeUnitSphere(1), == , std::log(2));
			TEST_ENSURE_OP(n.lnVolumeUnitSphere(2), == , std::log(2));
		}

		void testMaximum()
		{
			using NormBijection = Maximum_NormBijection<real>;
			PASTEL_CONCEPT_CHECK(NormBijection, NormBijection_Concept);

			NormBijection n;

			real valueSet[] = { 0, 1, 2, 3.5, 5, 7, 9.25, 13 };

			for (auto&& x : valueSet)
			{
				TEST_ENSURE_OP(n.axis(x), == , x);
				TEST_ENSURE_OP(n.signedAxis(x), == , x);
				TEST_ENSURE_OP(n.signedAxis(-x), == , x);
				TEST_ENSURE_OP(n.addAxis(100, x), == , std::max((real)100, x));
				TEST_ENSURE_OP(n.replaceAxis(100, 10, x), == ,  std::max((real)100, x));
				TEST_ENSURE_OP(n.toNorm(x), == , x);
				TEST_ENSURE_OP(n.toLnNorm(x), == , std::log(x));
				TEST_ENSURE_OP(n.toBijection(x), == , x);
				TEST_ENSURE_OP(n.scalingFactor(x), == , x);
			}

			TEST_ENSURE_OP(n.lnVolumeUnitSphere(1), == , std::log(2));
			TEST_ENSURE_OP(n.lnVolumeUnitSphere(2), == , std::log(4));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("NormBijection", test);
	}

	CallFunction run(addTest);

}
