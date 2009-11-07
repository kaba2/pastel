#ifndef PASTEL_LOCALGEOMETRY_H
#define PASTEL_LOCALGEOMETRY_H

#include "pastel/sys/vector.h"

#include "pastel/ray/rayforward.h"
#include "pastel/ray/raylibrary.h"

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
		
		// (u, v)
		Vector2 q;

		// p(u, v)
		Vector3 position;

		// cross((dp / du)(u, v), (dp / dv)(u, v)) 
		// (within a factor)
		Vector3 normal;

		// (dp / du)(u, v)
		Vector3 dpDu;

		// (dp / dv)(u, v)
		Vector3 dpDv;

		// (d^2p / du^2)(u, v)
		Vector3 ddpDuu;

		// (d^2p / (du dv))(u, v)
		Vector3 ddpDuv;

		// (d^2p / dv^2)(u, v)
		Vector3 ddpDvv;

		// Shape
		const Shape_Thing* thing;
	};

}

#endif
