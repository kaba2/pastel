#include "pastel/ray/lambertian_brdf.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	Lambertian_Brdf::Lambertian_Brdf()
		: reflectance_(1)
	{
	}

	Lambertian_Brdf::~Lambertian_Brdf()
	{
	}

	void Lambertian_Brdf::setReflectance(
		const Spectrum& reflectance)
	{
		reflectance_ = reflectance;
	}

	Spectrum Lambertian_Brdf::reflectance() const
	{
		return reflectance_;
	}

	Spectrum Lambertian_Brdf::evaluate(
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

	Vector3 Lambertian_Brdf::sampleDirection() const
	{
		// FIX: implement
		ENSURE(false);
		return Vector3();
	}

}
