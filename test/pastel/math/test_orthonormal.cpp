// Description: Testing for orthonormal.h
// DocumentationOf: orthonormal.h

#include "test/test_init.h"

#include "pastel/math/matrix/orthonormal.h"
#include "pastel/math/matrix.h"

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
			Matrix<real> m =
				diagonalMatrix(Vector3(1, 2, 3));

			m = orthonormalize(m);
			{
				REQUIRE(frobeniusNorm(m - identityMatrix<real>(3, 3)) < 0.0001);
			}

			m = randomMatrix<real>(3, 3);
			m = orthonormalize(m);
			{
				REQUIRE(frobeniusNorm(transpose(m) * m - 
					identityMatrix<real>(3, 3)) < 0.0001);
			}
		}
	};

	TEST_CASE("orthonormal", "[orthonormal]")
	{
	}

}
