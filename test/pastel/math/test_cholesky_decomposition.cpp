// Description: Testing for CholeskyDecomposition
// DocumentationOf: cholesky_decomposition.h

#include "test_pastelmath.h"

#include "pastel/math/matrix/cholesky_decomposition.h"
#include "pastel/math/matrix/matrix_norm.h"

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

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

				TEST_ENSURE_OP(maxNorm(cholesky.lower() - correctLower), <, 0.001);

				TEST_ENSURE_OP(determinant(cholesky) - (1 - 0.5 * 0.5), <, 0.001);
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("cholesky_decomposition", test);
	}

	CallFunction run(addTest);

}


