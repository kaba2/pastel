#ifndef PASTEL_RANDOM_DIRICHLET_HPP
#define PASTEL_RANDOM_DIRICHLET_HPP

#include "pastel/sys/random_dirichlet.h"
#include "pastel/sys/random_gamma.h"
#include "pastel/sys/vector_compare.h"

namespace Pastel
{

	template <typename Real, int N>
	Vector<Real, PASTEL_ADD_N(N, -1)> randomDirichlet(
		const Vector<Real, N>& shape)
	{
		PENSURE(allGreater(shape, 0));

		const integer n = shape.size() - 1;
		Vector<Real, PASTEL_ADD_N(N, -1)> u(ofDimension(n));

		Real gammaSum = randomGamma<Real>(shape[n]);
		for (integer i = 0;i < n;++i)
		{
			const Real t = randomGamma<Real>(shape[i]);
			
			u[i] = t;
			gammaSum += t;
		}
		
		u /= gammaSum;

		return u;
	}

}

#endif
