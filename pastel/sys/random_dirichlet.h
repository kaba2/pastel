#ifndef PASTEL_RANDOM_DIRICHLET_H
#define PASTEL_RANDOM_DIRICHLET_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Generates a vector from the Dirichlet distribution.
	/*!
	Preconditions:
	allGreater(shape, 0)

	Dirichlet distribution is a multi-dimensional 
	generalization of the beta-distribution.
	*/
	template <typename Real, int N>
	Vector<Real, PASTEL_ADD_N(N, -1)> randomDirichlet(
		const Vector<Real, N>& shape);

}

#include "pastel/sys/random_dirichlet.hpp"

#endif
