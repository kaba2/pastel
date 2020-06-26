// Description: Testing for CholeskyDecomposition
// DocumentationOf: cholesky_decomposition.h

#include "test/test_init.h"

#include "pastel/math/matrix/cholesky_decomposition.h"
#include "pastel/math/matrix/matrix_norm.h"

TEST_CASE("cholesky_decomposition (cholesky_decomposition)")
{
	CholeskyDecomposition<dreal> cholesky(
		matrix2x2<dreal>(
		1, 0.5, 
		0.5, 1));
	{
		Matrix<dreal> correctLower =
			matrix2x2<dreal>(
			1.000, 0.0000,
			0.5000, 0.8660);

		REQUIRE(maxNorm(cholesky.lower() - correctLower) < 0.001);

		REQUIRE(determinant(cholesky) - (1 - 0.5 * 0.5) < 0.001);
	}
}

