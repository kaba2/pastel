#ifndef PASTEL_BRDF_H
#define PASTEL_BRDF_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/spectrum.h"

#include "pastel/sys/countedptr.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	class PASTELRAY Brdf
		: public ReferenceCounted
	{
	public:
		// Using default destructor.

		Brdf();

		virtual ~Brdf();

		virtual Spectrum evaluate(
			const Vector3& from,
			const Vector3& to) const = 0;

	private:
		Brdf(const Brdf&) PASTEL_DELETE;
		Brdf& operator=(const Brdf&) PASTEL_DELETE;
	};

}

#endif
