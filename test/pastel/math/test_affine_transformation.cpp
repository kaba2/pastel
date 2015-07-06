// Description: Testing for affine transformations
// DocumentationOf: affine_transformation.h

#include "test_pastelmath.h"

#include "pastel/math/affine/affine_transformation.h"

#include "pastel/sys/view.h"

#include <algorithm>

using namespace Pastel;

namespace Pastel
{

	template class AffineTransformation<real>;

}

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

		using Affine = AffineTransformation<real>;

		void test()
		{
			// Constructs the identity transform.
			Affine a(3);
			{
				TEST_ENSURE(a.matrix() == identityMatrix<real>(3, 3));
				TEST_ENSURE(allEqual(a.translation(), 0));
				TEST_ENSURE_OP(a.n(), ==, 3);

				TEST_ENSURE(transformPoint(a, Vector3(0, 0, 0)) == Vector3(0, 0, 0));
				TEST_ENSURE(transformVector(a, Vector3(1, 1, 1)) == Vector3(1, 1, 1));
			}
			
			// Constructs from a matrix expression.
			Affine b(diagonalMatrix<real>(Vector3(1, 2, 3)));
			{
				TEST_ENSURE(b.matrix() == diagonalMatrix(Vector3(1, 2, 3)));
				TEST_ENSURE(allEqual(b.translation(), 0));
				TEST_ENSURE_OP(b.n(), ==, 3);

				TEST_ENSURE(transformPoint(b, Vector3(1, 1, 1)) == Vector3(1, 2, 3));
				TEST_ENSURE(transformVector(b, Vector3(1, 1, 1)) == Vector3(1, 2, 3));
			}

			// Constructs from a matrix and a vector expression.
			Affine c(diagonalMatrix(Vector3(1, 2, 3)), Vector3(-1, 0, 1));
			{
				TEST_ENSURE(c.translation() == Vector3(-1, 0, 1));
				TEST_ENSURE(c.matrix() == diagonalMatrix(Vector3(1, 2, 3)));
				TEST_ENSURE_OP(c.n(), ==, 3);

				TEST_ENSURE(transformPoint(c, Vector3(1, 1, 1)) == Vector3(1 * 1 - 1, 1 * 2 - 0, 1 * 3 + 1));
				TEST_ENSURE(transformVector(c, Vector3(1, 1, 1)) == Vector3(1 * 1, 1 * 2, 1 * 3));
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
		testRunner().add("affine_transformation", test);
	}

	CallFunction run(addTest);

}

