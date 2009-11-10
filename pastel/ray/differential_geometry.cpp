#include "pastel/ray/differential_geometry.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	PASTELRAY void computeNormalDerivatives(
		const Vector3& normal,
		const Vector3& dpDu,
		const Vector3& dpDv,
		const Vector3& ddpDuu,
		const Vector3& ddpDuv,
		const Vector3& ddpDvv,
		Vector3& dnDu,
		Vector3& dnDv)
	{
		// Coefficients of the first fundamental form.

		const real E = dot(dpDu);
		const real F = dot(dpDu, dpDv);
		const real G = dot(dpDv);

		// Coefficients of the second fundamental form.

		const real e = dot(normal, ddpDuu);
		const real f = dot(normal, ddpDuv);
		const real g = dot(normal, ddpDvv);

		// The derivatives are contained in the plane
		// spanned by the tangent vectors dpDu and dpDv.
		// The Weingarten equations give the derivatives
		// of the normal w.r.t. the tangent vector basis 
		// (dpDu, dpDv).

		const real factor = inverse(E * G - F * F);

		dnDu = ((f * F - e * G) * dpDu + 
			(e * F - f * E) * dpDv) * factor;
		dnDv = ((g * F - f * G) * dpDu + 
			(f * F - g * E) * dpDv) * factor;
	}

}
