// Description: Testing for PluDecomposition
// DocumentationOf: lu_decomposition.h

#include "test_pastelmath.h"

#include "pastel/sys/view.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/rational.h"

#include "pastel/math/matrix.h"
#include "pastel/math/sampling/uniform_sampling.h"

using namespace Pastel;

namespace
{

	using Real = Rational<integer>;

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
			testLu();
		}

		template <int N>
		void testLuCase()
		{
			integer bad = 0;

			integer n = 10;
			for (integer i = 0;i < 10000;++i)
			{
				Matrix<real> m = randomMatrix<real>(n, n);

				LuDecomposition<real> lu(m);

				if (!lu.singular())
				{
					Vector<real> b = randomVectorCube<real, Dynamic>(n);
					Vector<real> x = solveLinear(lu, b);

					const Vector<real> residual = m * x - b;
					real normResidual = norm(residual);

					if (normResidual > 0.0001)
					{
						++bad;
					}
				}
			}

			TEST_ENSURE_OP(bad, <, 100);
		}

		void testLu()
		{
			{
				LuDecomposition<Real> lu(
					matrix2x2<Real>(
					1, Real(1, 2), 
					Real(1, 2), 1));
				Matrix<Real> correctPackedLu =
					matrix2x2<Real>(
					1, Real(1, 2), 
					Real(1, 2), Real(3, 4));
				Tuple<integer, 2> correctRowPermutation(
					0, 1);
				TEST_ENSURE(lu.packedLu() == correctPackedLu);
				TEST_ENSURE(lu.rowPermutation() == correctRowPermutation);

				//std::cout << lu.packedLu() << std::endl;
				//std::cout << lu.rowPermutation() << std::endl;
			}
			{
				LuDecomposition<Real> lu(
					matrix3x3<Real>(
					1, 2, 3,
					4, 5, 6,
					7, 8, 9));
				Matrix<Real> correctPackedLu =
					matrix3x3<Real>(
					7, 8, 9,
					Real(1, 7), Real(6, 7), Real(12, 7),
					Real(4, 7), Real(1, 2), 0);
				Tuple<integer, 3> correctRowPermutation(
					2, 0, 1);
				TEST_ENSURE(lu.packedLu() == correctPackedLu);
				TEST_ENSURE(lu.rowPermutation() == correctRowPermutation);

				//std::cout << lu.packedLu() << std::endl;
				//std::cout << lu.rowPermutation() << std::endl;
			}

			testLuCase<1>();
			testLuCase<2>();
			testLuCase<3>();
			testLuCase<4>();
			testLuCase<5>();
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("lu_decomposition", test);
	}

	CallFunction run(addTest);

}
