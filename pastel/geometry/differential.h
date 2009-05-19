#ifndef PASTEL_DIFFERENTIAL_H
#define PASTEL_DIFFERENTIAL_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real>
	void computeNormalDerivatives(
		const Vector<3, Real>& dpDu, const Vector<3, Real>& dpDv, const Vector<3, Real>& normal,
		const Vector<3, Real>& ddpDuu, const Vector<3, Real>& ddpDuv, const Vector<3, Real>& ddpDvv,
		Vector<3, Real>& dnDu, Vector<3, Real>& dnDv);

}

#include "pastel/geometry/differential.hpp"

#endif
