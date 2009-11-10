#ifndef PASTEL_THING_H
#define PASTEL_THING_H

#include "pastel/ray/rayforward.h"
#include "pastel/ray/localgeometry.h"
#include "pastel/ray/shapeintersection.h"

#include "pastel/math/affinebijection.h"

#include "pastel/sys/countedptr.h"

#include "pastel/geometry/ray.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	class PASTELRAY Thing
		: public ReferenceCounted
	{
	public:
		Thing();
		virtual ~Thing();

		virtual AlignedBox3 bound() const = 0;

		bool intersect(
			const Ray3& ray,
			ShapeIntersection& shapeIntersection) const;

		void setTransformation(
			const AffineTransformation3& transformation);
		const AffineBijection3& transformation() const;

	private:
		virtual bool intersect_(
			const Ray3& ray,
			ShapeIntersection& shapeIntersection) const = 0;

		AffineBijection3 transformation_;
	};

}

#endif
