// Description: Testing for orthonormal.h
// DocumentationOf: orthonormal.h

#include "test/test_init.h"

#include "pastel/math/matrix/orthonormal.h"
#include "pastel/math/matrix/matrix_norm.h"
#include "pastel/math/matrix/random_matrix.h"

TEST_CASE("orthonormal (orthonormal)")
{
	Matrix<dreal, 3, 3> m =
		diagonalMatrix(Vector3(1, 2, 3));

	m = orthonormalize(m);
	{
		REQUIRE(frobeniusNorm2(m - identityMatrix<dreal, 3, 3>(3, 3)) < square(0.0001));
	}

	m = randomMatrix<dreal, 3, 3>(3, 3);
	m = orthonormalize(m);
	{
		REQUIRE(frobeniusNorm2(transpose(m) * m - 
			identityMatrix<dreal, 3, 3>(3, 3)) < square(0.0001));
	}
}
