#ifndef PASTEL_GJKSPHERE_H
#define PASTEL_GJKSPHERE_H

#include "pastel/geometry/gjkshape.h"

#include "pastel/geometry/sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	class GjkSphere
		: public GjkShape<N, Real>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		GjkSphere()
			: GjkShape<N, Real>()
			, sphere_()
		{
		}

		explicit GjkSphere(
			const Sphere<N, Real>& sphere)
			: GjkShape<N, Real>()
			, sphere_(sphere)
		{
		}

		virtual ~GjkSphere()
		{
		}

		virtual Point<N, Real> support(const Vector<N, Real>& direction) const
		{
			const Real normDirection = norm(direction);

			// EPSILON
			if (normDirection == 0)
			{
				return Point<N, Real>(unitAxis(0) * sphere_.radius());
			}

			return sphere_.position() +
				sphere_.radius() * direction / normDirection;
		}

	private:
		Sphere<N, Real> sphere_;
	};

}

#endif
