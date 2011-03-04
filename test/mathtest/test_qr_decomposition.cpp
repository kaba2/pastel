// Description: Testing for QrDecomposition
// DocumentationOf: qr_decomposition.h

#include "pastelmathtest.h"

#include "pastel/math/qr_decomposition_tools.h"
#include "pastel/math/matrix_tools.h"

#include "pastel/math/uniform_sampling.h"

#include <iostream>

using namespace Pastel;

namespace
{

	class TestQrDecomposition
		: public TestSuite
	{
	public:
		TestQrDecomposition()
			: TestSuite(&mathTestReport())
		{
		}

		virtual void run()
		{
			testQrCase<real, 1>();
			testQrCase<real, 2>();
			testQrCase<real, 3>();
			testQrCase<real, 4>();
			//testQrCase<real, Dynamic>();
		}

		template <typename Real, int N>
		void testQrCase()
		{
			const integer n = 
				(N == Dynamic) ? 10 : N;

			const integer attempts = 10000;
			integer errorCount = 0;

			for (integer i = 0;i < attempts;++i)
			{
				Matrix<Real, N, N> a(n, n);
				setRandomMatrix(a);

				const Vector<Real, N> b = randomVectorCube<Real, N>(n);

				const QrDecomposition<Real, N> qr(a);

				const Vector<Real, N> qrSolution =
					solveLinear(qr, b);

				const Real error = 
					norm(qrSolution * a  - b);

				if (error > 0.001)
				{
					++errorCount;
				}
			}
			
			TEST_ENSURE_OP(errorCount, <, attempts - 100);
		}
	};

	void testQrDecomposition()
	{
		TestQrDecomposition test;
		test.run();
	}

	void addTest()
	{
		mathTestList().add("qr_decomposition", testQrDecomposition);
	}

	CallFunction run(addTest);

}
