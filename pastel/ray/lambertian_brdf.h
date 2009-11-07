#ifndef PASTEL_LAMBERTIAN_BRDF_H
#define PASTEL_LAMBERTIAN_BRDF_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/brdf.h"
#include "pastel/ray/spectrum.h"

namespace Pastel
{

	class PASTELRAY Lambertian_Brdf
		: public Brdf
	{
	public:
		Lambertian_Brdf();
		virtual ~Lambertian_Brdf();

		virtual Spectrum evaluate(
			const Vector3& from,
			const Vector3& to) const;

		void setReflectance(
			const Spectrum& reflectance);

		Spectrum reflectance() const;

		Vector3 sampleDirection() const;

	private:
		// Prohibited
		Lambertian_Brdf(const Lambertian_Brdf&);
		// Prohibited
		Lambertian_Brdf& operator=(const Lambertian_Brdf&);

		Spectrum reflectance_;
	};

}

#endif
