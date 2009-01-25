#include "pastel/ray/lambertianbrdf.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	namespace Yun
	{

		LambertianBrdf::LambertianBrdf()
			: reflectance_(1)
		{
		}

		LambertianBrdf::~LambertianBrdf()
		{
		}

		void LambertianBrdf::setReflectance(
			const Spectrum& reflectance)
		{
			reflectance_ = reflectance;
		}

		Spectrum LambertianBrdf::reflectance() const
		{
			return reflectance_;
		}

		Spectrum LambertianBrdf::compute(
			const Vector3& from,
			const Vector3& to) const
		{
			// Let
			// H^2 be the hemisphere
			// brdf be a constant function
			//
			// reflectance
			// = int[H^2] brdf * cos(theta) dw
			// = brdf int[0..pi/2] int[0..2pi]
			//   sin(theta) * cos(theta) d(phi) d(theta)
			// = (brdf / 2) * int[0..pi/2] int[0..2pi]
			//   sin(2 * theta) d(phi) d(theta)
			// = (brdf / 2) * 2 * pi *
			//   int[0..pi/2] sin(2 * theta) d(theta)
			// = brdf * pi * (1/2)
			//   [0..pi/2] -cos(2 * theta) d(theta)
			// = brdf * pi
			//
			// => brdf = reflectance / pi

			unused(from);
			unused(to);

			return reflectance_ / constantPi<real>();
		}

		Vector3 LambertianBrdf::sampleDirection() const
		{
			// FIX: implement
			ENSURE(false);
			return Vector3();
		}

	}

}
