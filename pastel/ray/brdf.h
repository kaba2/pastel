#ifndef PASTELRAY_BRDF_H
#define PASTELRAY_BRDF_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/spectrum.h"

#include "pastel/sys/countedptr.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	namespace Yun
	{

		class PASTELRAY Brdf
			: public ReferenceCounted
		{
		public:
			// Using default destructor.

			Brdf();

			virtual ~Brdf();

			virtual Spectrum compute(
				const Vector3& from,
				const Vector3& to) const = 0;

		private:
			// Prohibited
			Brdf(const Brdf&);
			// Prohibited
			Brdf& operator=(const Brdf&);
		};

	}

}

#endif
