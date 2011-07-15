// Description: Testing for CholeskyDecomposition
// DocumentationOf: cholesky_decomposition.h

#include "pastelmathtest.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/tuple_tools.h"

#include "pastel/math/cholesky_decomposition_tools.h"
#include "pastel/math/matrix_tools.h"

using namespace Pastel;

namespace
{

	void testCholesky()
	{
		{
			const CholeskyDecomposition2 cholesky(
				Matrix2(
				1, 0.5, 
				0.5, 1));

			const Matrix2 correctLower(
				1, 0,
				0.5, 0.8660);

			//REPORT(cholesky.lower() != correctLower);
			//REPORT(determinant(cholesky) != 1 - 0.5 * 0.5);

			std::cout << cholesky.lower() << std::endl;
		}
	}

	void addTest()
	{
		testRunner().add("cholesky_decomposition", testCholesky);
	}

	CallFunction run(addTest);

}
