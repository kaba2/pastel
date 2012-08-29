// Description: An abstract material
// Documentation: materials.txt

#ifndef PASTELRAY_MATERIAL_H
#define PASTELRAY_MATERIAL_H

#include "pastel/sys/countedptr.h"
#include "pastel/sys/vector.h"

#include "pastel/ray/spectrum.h"

namespace Pastel
{

	class PASTELRAY Material
		: public ReferenceCounted
	{
	public:
		virtual ~Material();

		virtual Spectrum brdf(
			const Vector2& q,
			const Vector2& qx,
			const Vector2& qy,
			const Vector3& in,
			const Vector3& out) const = 0;
	};

	typedef CountedPtr<Material> MaterialPtr;

}

#endif
