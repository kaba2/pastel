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
		LocalGeometry& surface,
		real& tClosest) const
	{
		if (shape_->intersect(ray, surface, tClosest))
		{
			surface.thing = this;
			return true;
		}

		tClosest = infinity<real>();

		return false;
	}

}
