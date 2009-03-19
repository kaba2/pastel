#ifndef PASTELRAY_LAMBERTIANBRDF_H
#define PASTELRAY_LAMBERTIANBRDF_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/brdf.h"
#include "pastel/ray/spectrum.h"

namespace Pastel
{

	class PASTELRAY LambertianBrdf
		: public Brdf
	{
	public:
		LambertianBrdf();
		virtual ~LambertianBrdf();

		virtual Spectrum evaluate(
			const Vector3& from,
			const Vector3& to) const;

		void setReflectance(
			const Spectrum& reflectance);

		Spectrum reflectance() const;

		Vector3 sampleDirection() const;

	private:
		// Prohibited
		LambertianBrdf(const LambertianBrdf&);
		// Prohibited
		LambertianBrdf& operator=(const LambertianBrdf&);

		Spectrum reflectance_;
	};

}

#endif
