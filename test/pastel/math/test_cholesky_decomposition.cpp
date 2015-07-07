// Description: Testing for CholeskyDecomposition
// DocumentationOf: cholesky_decomposition.h

#include "test/test_init.h"

#include "pastel/math/matrix/cholesky_decomposition.h"
#include "pastel/math/matrix/matrix_norm.h"

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
			CholeskyDecomposition<real> cholesky(
				matrix2x2<real>(
				1, 0.5, 
				0.5, 1));
			{
				Matrix<real> correctLower =
					matrix2x2<real>(
					1.000, 0.0000,
					0.5000, 0.8660);

				REQUIRE(maxNorm(cholesky.lower() - correctLower) < 0.001);

				REQUIRE(determinant(cholesky) - (1 - 0.5 * 0.5) < 0.001);
			}
		}
	};

	TEST_CASE("cholesky_decomposition", "[cholesky_decomposition]")
	{
	}

}
