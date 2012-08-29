// Description: Shape thing
// Documentation: things.txt

#ifndef PASTELRAY_SHAPE_THING_H
#define PASTELRAY_SHAPE_THING_H

#include "pastel/ray/thing.h"
#include "pastel/ray/shape.h"
#include "pastel/ray/material.h"

namespace Pastel
{

	class PASTELRAY Shape_Thing
		: public Thing
	{
	public:
		Shape_Thing();

		Shape_Thing(
			const ShapePtr& shape,
			const MaterialPtr& material);

		virtual ~Shape_Thing();

		virtual AlignedBox3 bound() const;

		void setShape(const ShapePtr& shape);
		ShapePtr shape() const;

		void setMaterial(const MaterialPtr& material);
		MaterialPtr material() const;
	
	private:
		virtual bool intersect_(
			const Ray3& ray,
			ShapeIntersection& intersection) const;

		ShapePtr shape_;
		MaterialPtr material_;
	};
	
}

#endif
