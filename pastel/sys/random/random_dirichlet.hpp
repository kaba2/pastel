#ifndef PASTELSYS_RANDOM_DIRICHLET_HPP
#define PASTELSYS_RANDOM_DIRICHLET_HPP

#include "pastel/sys/random/random_dirichlet.h"
#include "pastel/sys/random/random_gamma.h"
#include "pastel/sys/vector/vector_compare.h"

namespace Pastel
{

	template <typename Real, int N>
	Vector<Real, SubN<N>> randomDirichlet(
		const Vector<Real, N>& shape)
	{
		PENSURE(allGreater(shape, 0));

		integer n = shape.size() - 1;
		Vector<Real, SubN<N>> u(ofDimension(n));

		Real gammaSum = 0;
		for (integer i = 0;i < n;++i)
		{
			Real t = randomGamma<Real>(shape[i]);
			
			u[i] = t;
			gammaSum += t;
		}

		do
		{
			gammaSum += randomGamma<Real>(shape[n]);
		}
		while(gammaSum == 0);
		
		u /= gammaSum;

		return u;
	}

}

#endif
