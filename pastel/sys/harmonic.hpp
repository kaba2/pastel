#ifndef PASTEL_HARMONIC_HPP
#define PASTEL_HARMONIC_HPP

#include "pastel/sys/harmonic.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/digamma.h"
#include "pastel/sys/constants.h"

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
