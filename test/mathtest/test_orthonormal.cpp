// Description: Testing for orthonormal.h
// DocumentationOf: orthonormal.h

#include "test_pastelmath.h"

#include "pastel/math/orthonormal.h"
#include "pastel/math/matrix.h"

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
			Matrix<real> m =
				diagonalMatrix(Vector3(1, 2, 3));

			m = orthonormalize(m);
			{
				real correctSet[] = 
				{
					1, 0, 0,
					0, 1, 0,
					0, 0, 1
				};

				TEST_ENSURE(frobeniusNorm(m - identityMatrix<real>(3, 3)) < 0.0001);
			}

			m = randomMatrix<real>(3, 3);
			m = orthonormalize(m);
			{
				TEST_ENSURE(frobeniusNorm(transpose(m) * m - 
					identityMatrix<real>(3, 3)) < 0.0001);
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
		testRunner().add("orthonormal", test);
	}

	CallFunction run(addTest);

}
