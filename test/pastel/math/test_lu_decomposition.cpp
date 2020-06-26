// Description: Testing for PluDecomposition
// DocumentationOf: lu_decomposition.h

#include "test/test_init.h"

#include "pastel/sys/view.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/rational.h"

#include "pastel/math/matrix.h"
#include "pastel/math/sampling/uniform_sampling.h"

namespace
{

	using Real = Rational<integer>;

	template <integer N>
	void testLuCase()
	{
		integer bad = 0;

		integer n = 10;
		for (integer i = 0;i < 10000;++i)
		{
			Matrix<dreal> m = randomMatrix<dreal>(n, n);

			LuDecomposition<dreal> lu(m);

			if (!lu.singular())
			{
				Vector<dreal> b = randomVectorCube<dreal, Dynamic>(n);
				Vector<dreal> x = solveLinear(lu, b);

				const Vector<dreal> residual = m * x - b;
				dreal normResidual = norm(residual);

				if (normResidual > 0.0001)
				{
					++bad;
				}
			}
		}

		REQUIRE(bad < 100);
	}

}

TEST_CASE("lu_decomposition (lu_decomposition)")
{
	{
		LuDecomposition<Real> lu(
			matrix2x2<Real>(
			1, Real(1, 2), 
			Real(1, 2), 1));
		Matrix<Real> correctPackedLu =
			matrix2x2<Real>(
			1, Real(1, 2), 
			Real(1, 2), Real(3, 4));
		Tuple<integer, 2> correctRowPermutation(
			0, 1);
		REQUIRE(lu.packedLu() == correctPackedLu);
		REQUIRE(lu.rowPermutation() == correctRowPermutation);

		//std::cout << lu.packedLu() << std::endl;
		//std::cout << lu.rowPermutation() << std::endl;
	}
	{
		LuDecomposition<Real> lu(
			matrix3x3<Real>(
			1, 2, 3,
			4, 5, 6,
			7, 8, 9));
		Matrix<Real> correctPackedLu =
			matrix3x3<Real>(
			7, 8, 9,
			Real(1, 7), Real(6, 7), Real(12, 7),
			Real(4, 7), Real(1, 2), 0);
		Tuple<integer, 3> correctRowPermutation(
			2, 0, 1);
		REQUIRE(lu.packedLu() == correctPackedLu);
		REQUIRE(lu.rowPermutation() == correctRowPermutation);

		//std::cout << lu.packedLu() << std::endl;
		//std::cout << lu.rowPermutation() << std::endl;
	}

	testLuCase<1>();
	testLuCase<2>();
	testLuCase<3>();
	testLuCase<4>();
	testLuCase<5>();
}

