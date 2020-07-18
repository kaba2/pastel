// Description: Testing for QrDecomposition
// DocumentationOf: qr_decomposition.h

#include "test/test_init.h"

#include "pastel/math/matrix/qr_decomposition.h"
#include "pastel/math/matrix/random_matrix.h"
#include "pastel/math/sampling.h"

#include <iostream>

TEST_CASE("qr_decomposition", "[qr_decomposition]")
{
	integer n = 10;

	integer attempts = 10000;
	integer errorCount = 0;

	for (integer i = 0;i < attempts;++i)
	{
		Matrix<dreal> a = randomMatrix<dreal>(n, n);
		Vector<dreal> b = randomVectorCube<dreal, Dynamic>(n);
		QrDecomposition<dreal> qr(view(a));
		Vector<dreal> qrSolution =
			solveLinear(qr, b);

		dreal error =
			norm(a * qrSolution  - b);

		if (error > 0.001)
		{
			++errorCount;
		}
	}

	REQUIRE(errorCount < attempts - 100);
}
