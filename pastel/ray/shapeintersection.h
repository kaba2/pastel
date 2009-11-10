#ifndef PASTEL_SHAPEINTERSECTION_H
#define PASTEL_SHAPEINTERSECTION_H

#include "pastel/sys/vector.h"

#include "pastel/ray/rayforward.h"
#include "pastel/ray/raylibrary.h"

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	class PASTELRAY ShapeIntersection
	{
	public:
		real t;
		Vector3 position;
		AffineTransformation3 shapeToWorld;
		const Shape_Thing* thing;
	};

}

#endif
