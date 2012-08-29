// Description: Lambertian brdf
// Documentation: brdfs.txt

#ifndef PASTELRAY_LAMBERTIAN_BRDF_H
#define PASTELRAY_LAMBERTIAN_BRDF_H

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
		Lambertian_Brdf(const Lambertian_Brdf&) PASTEL_DELETE;
		Lambertian_Brdf& operator=(const Lambertian_Brdf&) PASTEL_DELETE;

		Spectrum reflectance_;
	};

}

#endif
