// Description: Testing for CholeskyDecomposition
// DocumentationOf: cholesky_decomposition.h

#include "test_pastelmath.h"

#include "pastel/math/cholesky_decomposition.h"
#include "pastel/math/matrix_norm.h"

using namespace Pastel;
using namespace std;

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
			const CholeskyDecomposition2 cholesky(
				Matrix2(
				1, 0.5, 
				0.5, 1));
			{
				const Matrix2 correctLower(
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


