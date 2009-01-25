#ifndef PASTELGEOMETRY_DIFFERENTIAL_HPP
#define PASTELGEOMETRY_DIFFERENTIAL_HPP

#include "pastel/geometry/differential.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real>
	void computeNormalDerivatives(
		const Vector<3, Real>& dpDu, const Vector<3, Real>& dpDv, const Vector<3, Real>& normal,
		const Vector<3, Real>& ddpDuu, const Vector<3, Real>& ddpDuv, const Vector<3, Real>& ddpDvv,
		Vector<3, Real>& dnDu, Vector<3, Real>& dnDv)
	{
		const Real E = dot(dpDu);
		const Real F = dot(dpDu, dpDv);
		const Real G = dot(dpDv);
		const Real e = dot(normal, ddpDuu);
		const Real f = dot(normal, ddpDuv);
		const Real g = dot(normal, ddpDvv);

		const Real factor = inverse(E * G - F * F);
		
		dnDu = (f * F - e * G) * factor * dpDu + (e * F - f * E) * factor * dpDv;
		dnDv = (g * F - f * G) * factor * dpDu + (f * F - g * E) * factor * dpDv;
	}

}

#endif
