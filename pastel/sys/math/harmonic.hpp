#ifndef PASTELSYS_HARMONIC_HPP
#define PASTELSYS_HARMONIC_HPP

#include "pastel/sys/math/harmonic.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/math/digamma.h"
#include "pastel/sys/math/constants.h"

namespace Pastel
{

	template <typename Real>
	Real harmonicNumber(integer n)
	{
		ENSURE_OP(n, >=, 0);

		return digamma<Real>(n + 1) + constantEulerMascheroni<real>();
	}

}

#endif
