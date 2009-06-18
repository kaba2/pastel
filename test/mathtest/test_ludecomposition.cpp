#include "pastelmathtest.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/nativeinteger_tools.h"
#include "pastel/sys/tuple_tools.h"

#include "pastel/math/rational_tools.h"
#include "pastel/math/ludecomposition_tools.h"

using namespace Pastel;

namespace
{

	typedef NativeInteger<integer> Integer;
	typedef Rational<Integer> Real;

	template <int N>
	void testLuCase()
	{
		integer bad = 0;

		for (integer i = 0;i < 10000;++i)
		{
			Matrix<N, N, real> m;
			setRandomMatrix(m);

			LuDecomposition<N, real> lu(m);

			if (!lu.singular())
			{
				const Vector<N, real> b = randomVectorCube<N, real>();

				const Vector<N, real> x = solveLinear(lu, b);

				const Vector<N, real> residual = x * m - b;
				const real normResidual = norm(residual);

				if (normResidual > 0.0001)
				{
					++bad;
				}
			}
		}

		REPORT1(bad >= 100, bad);
	}

	void testLu()
	{
		{
			const LuDecomposition<2, Real> lu(
				Matrix<2, 2, Real>(
				1, Real(1, 2), 
				Real(1, 2), 1));
			const Matrix<2, 2, Real> correctPackedLu(
					1, Real(1, 2), 
					Real(1, 2), Real(3, 4));
			const Tuple<2, integer> correctRowPermutation(
				0, 1);
			REPORT(lu.packedLu() != correctPackedLu);
			REPORT(lu.rowPermutation() != correctRowPermutation);

			//std::cout << lu.packedLu() << std::endl;
			//std::cout << lu.rowPermutation() << std::endl;
		}
		{
			const LuDecomposition<3, Real> lu(
				Matrix<3, 3, Real>(
				1, 2, 3,
				4, 5, 6,
				7, 8, 9));
			const Matrix<3, 3, Real> correctPackedLu(
				7, 8, 9,
				Real(1, 7), Real(6, 7), Real(12, 7),
				Real(4, 7), Real(1, 2), 0);
			const Tuple<3, integer> correctRowPermutation(
				2, 0, 1);
			REPORT(lu.packedLu() != correctPackedLu);
			REPORT(lu.rowPermutation() != correctRowPermutation);

			//std::cout << lu.packedLu() << std::endl;
			//std::cout << lu.rowPermutation() << std::endl;
		}

		testLuCase<1>();
		testLuCase<2>();
		testLuCase<3>();
		testLuCase<4>();
		testLuCase<5>();
	}

	void testAdd()
	{
		mathTestList().add("lu_decomposition", testLu);
	}

	CallFunction run(testAdd);

}
