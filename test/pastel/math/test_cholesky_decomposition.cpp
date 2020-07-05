// Description: Testing for CholeskyDecompositionInplace
// DocumentationOf: cholesky_decomposition.h

#include "test/test_init.h"

#include "pastel/math/matrix/cholesky_decomposition.h"
#include "pastel/math/matrix/matrix_norm.h"

TEST_CASE("cholesky_decomposition (cholesky_decomposition)")
{
	dreal matrix[2][2] = {
		{1, 0.5}, 
		{0.5, 1}
	};

	CholeskyDecompositionInplace cholesky(view(matrix));

	dreal correctLower[2][2] = {
		{1.000, 0.0000},
		{0.5000, 0.8660}
	};

	REQUIRE(maxNorm(asMatrix(cholesky.lower()) - asMatrix(view(correctLower))) < 0.001);

	REQUIRE(determinant(cholesky) - (1 - 0.5 * 0.5) < 0.001);
}

