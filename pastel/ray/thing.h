#ifndef PASTEL_THING_H
#define PASTEL_THING_H

#include "pastel/ray/localgeometry.h"

#include "pastel/math/affinebijection.h"

#include "pastel/sys/countedptr.h"

#include "pastel/geometry/ray.h"

namespace Pastel
{

	class PASTELRAY Thing
		: public ReferenceCounted
	{
	public:
		Thing();
		virtual ~Thing();

		bool intersect(
			const Ray3& ray,
			LocalGeometry& surface,
			real& tClosest) const;

		void setTransformation(
			const AffineTransformation3& transformation);
		const AffineBijection3& transformation() const;

	private:
		virtual bool intersect_(
			const Ray3& ray,
			LocalGeometry& surface,
			real& tClosest) const = 0;

		AffineBijection3 transformation_;
	};

}

#endif
