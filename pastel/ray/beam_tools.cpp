#include "pastel/ray/beam_tools.h"
#include "pastel/ray/optics.h"

namespace Pastel
{

	PASTELRAY Beam transform(
		const Beam& beam, 
		const AffineTransformation3& transformation)
	{
		const Beam result(
			Ray3(transformPoint(beam.ray().position(), transformation),
			transformVector(beam.ray().direction(), transformation)),
			Ray3(transformVector(beam.ddx().position(), transformation),
			transformVector(beam.ddx().direction(), transformation)),
			Ray3(transformVector(beam.ddy().position(), transformation),
			transformVector(beam.ddy().direction(), transformation)));

		return result;
	}

	// Reference:
	// "Tracing Ray Differentials", Homan Igehy, 
	// _Computer Graphics_ (SIGGRAPH 1999 proceedings), 1999.

	PASTELRAY Beam transferToSurface(
		const Beam& beam,
		const LocalGeometry& surface,
		real t)
	{
		Beam result;

		// Compute primary ray.

		result.ray().setPosition(surface.position);

		// Compute x-derivative.

		result.ddx().setPosition(beam.ddx().at(t));
		
		const real dtDx = -dot(result.ddx().position(), surface.normal) / 
			dot(beam.ray().direction(), surface.normal);
		
		result.ddx() += dtDx * beam.ray().direction();
		result.ddx().setDirection(beam.ddx().direction());

		// Compute y-derivative.

		result.ddy().setPosition(beam.ddy().at(t));
		
		const real dtDy = -dot(result.ddy().position(), surface.normal) / 
			dot(beam.ray().direction(), surface.normal);
		
		result.ddy() += dtDy * beam.ray().direction();
		result.ddy().setDirection(beam.ddy().direction());

		return result;
	}

	PASTELRAY Beam reflectFromSurface(
		const Beam& beam, 
		const LocalGeometry& surface)
	{
		Beam result;

		// Compute primary ray.

		result.ray().set(surface.position, 
			reflect(beam.ray().direction(), surface.normal));
		
		// Compute x-derivative.

		const real dDotDnDx = 
			dot(beam.ddx().direction(), surface.normal) + 
			dot(beam.ray().direction(), surface.dnDu);

		result.ddx().setPosition(beam.ddx().position());
		result.ddx().setDirection(
			beam.ddx().direction() - 
			2 * (dot(beam.ray().direction(), surface.normal) * surface.dnDu + 
			dDotDnDx * surface.normal));
		
		// Compute y-derivative.

		const real dDotDnDy = 
			dot(beam.ddy().direction(), surface.normal) + 
			dot(beam.ray().direction(), surface.dnDv);

		result.ddy().setPosition(beam.ddy().position());
		result.ddy().setDirection(
			beam.ddy().direction() - 
			2 * (dot(beam.ray().direction(), surface.normal) * surface.dnDv + 
			dDotDnDy * surface.normal));

		return result;
	}

}
