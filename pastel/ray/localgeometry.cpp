#include "pastel/ray/localgeometry.h"
#include "pastel/ray/shape_thing.h"

namespace Pastel
{

	PASTELRAY void transform(
		LocalGeometry& surface,
		const AffineBijection3& transformation)
	{
		surface.position = 
			transformPoint(surface.position, transformation.forward());
		surface.normal = 
			transformNormal(surface.normal, transformation.inverse());
		surface.dpDu =
			transformVector(surface.dpDu, transformation.forward());
		surface.dpDv =
			transformVector(surface.dpDv, transformation.forward());
		surface.dnDu =
			transformVector(surface.dnDu, transformation.forward());
		surface.dnDv =
			transformVector(surface.dnDv, transformation.forward());
	}

	PASTELRAY LocalGeometry localGeometry(
		const ShapeIntersection& intersection)
	{
		LocalGeometry surface = 
			intersection.thing->shape()->localGeometry(
			intersection.position);
		AffineBijection3 transformation(
			intersection.shapeToWorld);

		transform(surface, transformation);

		return surface;
	}

}
