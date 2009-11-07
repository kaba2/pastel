#include "pastel/ray/beam.h"

namespace Pastel
{

	Beam::Beam()
		: ray_()
		, xRay_()
		, yRay_()
	{
	}

	Beam::Beam(const Ray3& ray,
		const Ray3& ddx,
		const Ray3& ddy)
		: ray_(ray)
		, xRay_(ddx)
		, yRay_(ddy)
	{
	}
		
	Ray3& Beam::ray()
	{
		return ray_;
	}

	const Ray3& Beam::ray() const
	{
		return ray_;
	}

	Ray3& Beam::ddx()
	{
		return xRay_;
	}

	const Ray3& Beam::ddx() const
	{
		return xRay_;
	}

	Ray3& Beam::ddy()
	{
		return yRay_;
	}

	const Ray3& Beam::ddy() const
	{
		return yRay_;
	}

}
