// Description: Testing for affine transformations
// DocumentationOf: affine_transformation.h

#include "test_pastelmath.h"

#include "pastel/math/affine_transformation.h"

#include "pastel/sys/views.h"

#include <algorithm>

using namespace Pastel;

namespace
{

	template class AffineTransformation<real>;

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

		typedef AffineTransformation<real> Affine;

		void test()
		{
			// Constructs the identity transform.
			Affine a(3);
			{
				TEST_ENSURE(a.matrix() == identityMatrix<real>(3, 3));
				TEST_ENSURE(allEqual(a.translation(), 0));
				TEST_ENSURE_OP(a.n(), ==, 3);
			}
			
			// Constructs from a matrix expression.
			Affine b(diagonalMatrix<real>(Vector3(1, 2, 3)));
			{
				TEST_ENSURE(b.matrix() == diagonalMatrix(Vector3(1, 2, 3)));
				TEST_ENSURE(allEqual(b.translation(), 0));
				TEST_ENSURE_OP(b.n(), ==, 3);
			}

			// Constructs from a matrix and a vector expression.
			Affine c(diagonalMatrix(Vector3(1, 2, 3)), Vector3(-1, 0, 1));
			{
				TEST_ENSURE(c.translation() == Vector3(-1, 0, 1));
				TEST_ENSURE(c.matrix() == diagonalMatrix(Vector3(1, 2, 3)));
				TEST_ENSURE_OP(c.n(), ==, 3);
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

