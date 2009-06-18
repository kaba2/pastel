#include "pastelmathtest.h"

#include "pastel/math/qr_decomposition_tools.h"
#include "pastel/math/matrix_tools.h"

#include "pastel/sys/random_vector.h"

#include <iostream>

using namespace Pastel;

namespace
{

	template <int N, typename Real>
	void testQrCase()
	{
		const integer n = 
			(N == Dynamic) ? 10 : N;

		const integer attempts = 10000;
		integer errorCount = 0;

		for (integer i = 0;i < attempts;++i)
		{
			Matrix<N, N, Real> a(n, n);
			setRandomMatrix(a);

			const Vector<N, Real> b = randomVectorCube<N, Real>(n);

			const QrDecomposition<N, Real> qr(a);

			const Vector<N, Real> qrSolution =
				solveLinear(qr, b);

			const Real error = 
				norm(qrSolution * a  - b);

			if (error > 0.001)
			{
				++errorCount;
			}
		}
		
		REPORT2(errorCount > attempts - 100,
			errorCount, attempts);
	}

	void testQr()
	{
		testQrCase<1, real>();
		testQrCase<2, real>();
		testQrCase<3, real>();
		testQrCase<4, real>();
		testQrCase<Dynamic, real>();
	}

	void testAdd()
	{
		mathTestList().add("qr_decomposition", testQr);
	}

	CallFunction run(testAdd);

}
