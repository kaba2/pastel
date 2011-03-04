// Description: Testing for PluDecomposition
// DocumentationOf: lu_decomposition.h

#include "pastelmathtest.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/tuple_tools.h"

#include "pastel/sys/rational_tools.h"
#include "pastel/math/lu_decomposition_tools.h"

using namespace Pastel;

namespace
{

	typedef Rational<integer> Real;

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&mathTestReport())
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

			for (integer i = 0;i < 10000;++i)
			{
				Matrix<real, N, N> m;
				setRandomMatrix(m);

				LuDecomposition<real, N> lu(m);

				if (!lu.singular())
				{
					const Vector<real, N> b = randomVectorCube<real, N>();

					const Vector<real, N> x = solveLinear(lu, b);

					const Vector<real, N> residual = x * m - b;
					const real normResidual = norm(residual);

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
				const LuDecomposition<Real, 2> lu(
					Matrix<Real, 2, 2>(
					1, Real(1, 2), 
					Real(1, 2), 1));
				const Matrix<Real, 2, 2> correctPackedLu(
						1, Real(1, 2), 
						Real(1, 2), Real(3, 4));
				const Tuple<integer, 2> correctRowPermutation(
					0, 1);
				TEST_ENSURE(lu.packedLu() == correctPackedLu);
				TEST_ENSURE(lu.rowPermutation() == correctRowPermutation);

				//std::cout << lu.packedLu() << std::endl;
				//std::cout << lu.rowPermutation() << std::endl;
			}
			{
				const LuDecomposition<Real, 3> lu(
					Matrix<Real, 3, 3>(
					1, 2, 3,
					4, 5, 6,
					7, 8, 9));
				const Matrix<Real, 3, 3> correctPackedLu(
					7, 8, 9,
					Real(1, 7), Real(6, 7), Real(12, 7),
					Real(4, 7), Real(1, 2), 0);
				const Tuple<integer, 3> correctRowPermutation(
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
		mathTestList().add("lu_decomposition", test);
	}

	CallFunction run(addTest);

}
