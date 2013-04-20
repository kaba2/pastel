// Description: Pointer point-representation

#ifndef PASTELSYS_POINTER_POINTREP_H
#define PASTELSYS_POINTER_POINTREP_H

#include "pastel/sys/pointrep_concept.h"

namespace Pastel
{

	template <typename Real_, int N_ = Dynamic>
	class Pointer_PointRep
	{
	public:
		enum {N = N_};
		typedef Real_ Real;
		typedef const Real* Point;

		explicit Pointer_PointRep(integer n = N)
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
