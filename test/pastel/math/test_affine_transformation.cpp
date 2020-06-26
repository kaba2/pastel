// Description: Testing for affine transformations
// DocumentationOf: affine_transformation.h

#include "test/test_init.h"

#include "pastel/math/affine/affine_transformation.h"

namespace Pastel
{

	template class AffineTransformation<dreal>;

}

namespace
{

		using Affine = AffineTransformation<dreal>;

}

TEST_CASE("affine_transformation (affine_transformation)")
{
	// Constructs the identity transform.
	Affine a(3);
	{
		REQUIRE(a.matrix() == identityMatrix<dreal>(3, 3));
		REQUIRE(allEqual(a.translation(), 0));
		REQUIRE(a.n() == 3);

		REQUIRE(transformPoint(a, Vector3(0, 0, 0)) == Vector3(0, 0, 0));
		REQUIRE(transformVector(a, Vector3(1, 1, 1)) == Vector3(1, 1, 1));
	}

	// Constructs from a matrix expression.
	Affine b(diagonalMatrix<dreal>(Vector3(1, 2, 3)));
	{
		REQUIRE(b.matrix() == diagonalMatrix(Vector3(1, 2, 3)));
		REQUIRE(allEqual(b.translation(), 0));
		REQUIRE(b.n() == 3);

		REQUIRE(transformPoint(b, Vector3(1, 1, 1)) == Vector3(1, 2, 3));
		REQUIRE(transformVector(b, Vector3(1, 1, 1)) == Vector3(1, 2, 3));
	}

	// Constructs from a matrix and a vector expression.
	Affine c(diagonalMatrix(Vector3(1, 2, 3)), Vector3(-1, 0, 1));
	{
		REQUIRE(c.translation() == Vector3(-1, 0, 1));
		REQUIRE(c.matrix() == diagonalMatrix(Vector3(1, 2, 3)));
		REQUIRE(c.n() == 3);

		REQUIRE(transformPoint(c, Vector3(1, 1, 1)) == Vector3(1 * 1 - 1, 1 * 2 - 0, 1 * 3 + 1));
		REQUIRE(transformVector(c, Vector3(1, 1, 1)) == Vector3(1 * 1, 1 * 2, 1 * 3));
	}
}
