// Description: Box shape

#ifndef PASTELRAY_BOX_SHAPE_H
#define PASTELRAY_BOX_SHAPE_H

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
			real& tClosest) const;

		virtual AlignedBox3 bound() const;

		void setRadius(const Vector3& radius);
		const Vector3& radius() const;

	private:
		AlignedBox3 box_;
	};

}

#endif
