// Description: Vector locator

#ifndef PASTELSYS_VECTOR_LOCATOR_H
#define PASTELSYS_VECTOR_LOCATOR_H

#include "pastel/sys/locator_concept.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real_, int N_ = Dynamic>
	class Vector_Locator
	{
	public:
		static PASTEL_CONSTEXPR int N = N_;
		using Real = Real_ ;
		using Point = Vector<Real, N>;

		explicit Vector_Locator(integer n = N)
			: n_(n)
		{
			ENSURE(N == Dynamic || n == N);
			ENSURE_OP(n, >=, 0);
		}

		integer n() const
		{
			return n_;
		}

		const Real& operator()(const Point& point, integer i) const
		{
			return point[i];
		}

	private:
		integer n_;
	};

}

#endif
