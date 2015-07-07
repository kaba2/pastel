// Description: Testing for QrDecomposition
// DocumentationOf: qr_decomposition.h

#include "test/test_init.h"

#include "pastel/math/matrix/qr_decomposition.h"
#include "pastel/math/matrix/random_matrix.h"
#include "pastel/math/sampling.h"

#include <iostream>

namespace
{

	class TestQrDecomposition
	{
	public:
		TestQrDecomposition()
		{
		}

		virtual void run()
		{
			testQrCase();
		}

		void testQrCase()
		{
			integer n = 10;

			integer attempts = 10000;
			integer errorCount = 0;

			for (integer i = 0;i < attempts;++i)
			{
				Matrix<real> a = randomMatrix<real>(n, n);
				Vector<real> b = randomVectorCube<real, Dynamic>(n);
				QrDecomposition<real> qr(a);
				Vector<real> qrSolution =
					solveLinear(qr, b);

				real error =
					norm(a * qrSolution  - b);

				if (error > 0.001)
				{
					++errorCount;
				}
			}

			REQUIRE(errorCount < attempts - 100);
		}
	};

	TEST_CASE("qr_decomposition", "[qr_decomposition]")
	{
	}

}