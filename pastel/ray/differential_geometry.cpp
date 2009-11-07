#include "pastel/ray/differential_geometry.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	PASTELRAY void computeNormalDerivatives(
		const LocalGeometry& surface,
		Vector3& dnDu,
		Vector3& dnDv)
	{
		// Coefficients of the first fundamental form.

		const real E = dot(surface.dpDu);
		const real F = dot(surface.dpDu, surface.dpDv);
		const real G = dot(surface.dpDv);

		// Coefficients of the second fundamental form.

		const real e = dot(surface.normal, surface.ddpDuu);
		const real f = dot(surface.normal, surface.ddpDuv);
		const real g = dot(surface.normal, surface.ddpDvv);

		// The derivatives are contained in the plane
		// spanned by the tangent vectors dpDu and dpDv.
		// The Weingarten equations give the derivatives
		// of the normal w.r.t. the tangent vector basis 
		// (dpDu, dpDv).

		const real factor = inverse(E * G - F * F);

		dnDu = ((f * F - e * G) * surface.dpDu + 
			(e * F - f * E) * surface.dpDv) * factor;
		dnDv = ((g * F - f * G) * surface.dpDu + 
			(f * F - g * E) * surface.dpDv) * factor;
	}

}
