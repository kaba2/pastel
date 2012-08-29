// Description: Local geometry of a shape
// Documentation: pastelray.txt

#ifndef PASTELRAY_LOCALGEOMETRY_H
#define PASTELRAY_LOCALGEOMETRY_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/shapeintersection.h"

#include "pastel/sys/vector.h"

#include "pastel/math/affinebijection.h"

namespace Pastel
{

	class PASTELRAY LocalGeometry
	{
	public:
		// A surface is parametrized with a twice 
		// differentiable function

		// p : R^2 -> R^3 : p(u, v) = (x, y, z)

		// This structure describes the differential
		// geometric properties of a shape at a
		// specific point on its surface.
		//
		// Its normal is given by:
		// n : R^2 -> R^3: 
		// n(u, v) = cross((dp / du)(u, v), (dp / dv)(u, v))
		
		// (u, v)
		Vector2 q;

		// p(u, v)
		Vector3 position;

		// n(u, v)
		Vector3 normal;

		// (dp / du)(u, v)
		Vector3 dpDu;

		// (dp / dv)(u, v)
		Vector3 dpDv;

		// (dn / du)(u, v)
		Vector3 dnDu;

		// (dn / dv)(u, v)
		Vector3 dnDv;
	};

	PASTELRAY void transform(
		LocalGeometry& surface,
		const AffineBijection3& transformation);

	PASTELRAY LocalGeometry localGeometry(
		const ShapeIntersection& intersection);

}

#endif
