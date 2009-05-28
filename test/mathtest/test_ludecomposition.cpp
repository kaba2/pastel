#include "pastelmathtest.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/nativeinteger_tools.h"
#include "pastel/sys/tuple_tools.h"

#include "pastel/math/rational_tools.h"
#include "pastel/math/ludecomposition.h"

using namespace Pastel;

namespace
{

	typedef NativeInteger<integer> Integer;
	typedef Rational<Integer> Real;

	void testLu()
	{
		{
			const LuDecomposition<2, Real> lu(
				Matrix<2, 2, Real>(
				1, Real(1, 2), 
				Real(1, 2), 1));
			const Matrix<2, 2, Real> correctPackedLu(
					1, Real(1, 2), 
					Real(1, 2), Real(3, 4));
			const Tuple<2, integer> correctRowPermutation(
				0, 1);
			REPORT(lu.packedLu() != correctPackedLu);
			REPORT(lu.rowPermutation() != correctRowPermutation);

			//std::cout << lu.packedLu() << std::endl;
			//std::cout << lu.rowPermutation() << std::endl;
		}
		{
			const LuDecomposition<3, Real> lu(
				Matrix<3, 3, Real>(
				1, 2, 3,
				4, 5, 6,
				7, 8, 9));
			const Matrix<3, 3, Real> correctPackedLu(
				7, 8, 9,
				Real(1, 7), Real(6, 7), Real(12, 7),
				Real(4, 7), Real(1, 2), 0);
			const Tuple<3, integer> correctRowPermutation(
				2, 0, 1);
			REPORT(lu.packedLu() != correctPackedLu);
			REPORT(lu.rowPermutation() != correctRowPermutation);

			//std::cout << lu.packedLu() << std::endl;
			//std::cout << lu.rowPermutation() << std::endl;
		}
	}

	void testAdd()
	{
		mathTestList().add("ludecomposition", testLu);
	}

	CallFunction run(testAdd);

}
