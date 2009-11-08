#ifndef PASTEL_BOX_SHAPE_H
#define PASTEL_BOX_SHAPE_H

#include "pastel/ray/shape.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	class PASTELRAY Box_Shape
		: public Shape
	{
	public:
		Box_Shape();
		explicit Box_Shape(
			const Vector3& radius);

		virtual ~Box_Shape();

		virtual bool intersect(
			const Ray3& ray,
			LocalGeometry& surface,
			real& tClosest) const;

		virtual AlignedBox3 bound() const;

	private:
		Vector3 radius_;
	};

}

#endif
