// Description: Pointer locator

#ifndef PASTELSYS_POINTER_LOCATOR_H
#define PASTELSYS_POINTER_LOCATOR_H

#include "pastel/sys/locator_concept.h"

namespace Pastel
{

	template <typename Real_, int N_ = Dynamic>
	class Pointer_Locator
	{
	public:
		static PASTEL_CONSTEXPR int N = N_;
		typedef Real_ Real;
		typedef const Real* Point;

		explicit Pointer_Locator(integer n = N)
			: n_(n)
		{
			ENSURE(N == Dynamic || n == N);
			ENSURE_OP(n, >=, 0);
		}

		integer n() const
		{
			return n_;
		}

		const Real& operator()(Point point, integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, n_);
			return point[i];
		}

	private:
		integer n_;
	};

}

#endif
