#include "pastel/ray/shape_thing.h"

namespace Pastel
{

	Shape_Thing::Shape_Thing()
		: shape_()
		, material_()
	{
	}

	Shape_Thing::Shape_Thing(
		const ShapePtr& shape,
		const MaterialPtr& material)
		: shape_(shape)
		, material_(material)
	{
	}

	Shape_Thing::~Shape_Thing()
	{
	}

	AlignedBox3 Shape_Thing::bound() const
	{
		return shape_->bound(transformation().forward());
	}

	void Shape_Thing::setShape(const ShapePtr& shape)
	{
		shape_ = shape;
	}

	ShapePtr Shape_Thing::shape() const
	{
		return shape_;
	}

	void Shape_Thing::setMaterial(const MaterialPtr& material)
	{
		material_ = material;
	}

	MaterialPtr Shape_Thing::material() const
	{
		return material_;
	}

	// Private

	bool Shape_Thing::intersect_(
		const Ray3& ray,
		ShapeIntersection& shapeIntersection) const
	{
		real t = 0;
		if (shape_->intersect(ray, t))
		{
			shapeIntersection.t = t;
			shapeIntersection.position = ray.at(t);
			shapeIntersection.shapeToWorld = AffineTransformation3();
			shapeIntersection.thing = this;
			return true;
		}

		return false;
	}

}
