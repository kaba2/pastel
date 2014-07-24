// Description: Testing for QrDecomposition
// DocumentationOf: qr_decomposition.h

#include "test_pastelmath.h"

#include "pastel/math/qr_decomposition.h"
#include "pastel/math/random_matrix.h"
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
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testQrCase();
		}

		void testQrCase()
		{
			integer n = 10;

			integer attempts = 10000;
			integer errorCount = 0;

			for (integer i = 0;i < attempts;++i)
			{
				Matrix<real> a = randomMatrix<real>(n, n);
				Vector<real> b = randomVectorCube<real, Dynamic>(n);
				QrDecomposition<real> qr(a);
				Vector<real> qrSolution =
					solveLinear(qr, b);

				real error =
					norm(a * qrSolution  - b);

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
		testRunner().add("qr_decomposition", testQrDecomposition);
	}

	CallFunction run(addTest);

}
