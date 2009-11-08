#ifndef PASTEL_SHAPE_H
#define PASTEL_SHAPE_H

#include "pastel/sys/countedptr.h"

#include "pastel/ray/localgeometry.h"

#include "pastel/geometry/ray.h"
#include "pastel/geometry/alignedbox.h"

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	class PASTELRAY Shape
		: public ReferenceCounted
	{
	public:
		Shape();
		virtual ~Shape();

		virtual bool intersect(
			const Ray3& ray,
			LocalGeometry& surface,
			real& tClosest) const = 0;

		virtual AlignedBox3 bound() const = 0;

		virtual AlignedBox3 bound(
			const AffineTransformation3& transformation) const;
	};

}

#endif
