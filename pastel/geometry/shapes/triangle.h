// Description: Triangle class

#ifndef PASTELGEOMETRY_TRIANGLE_H
#define PASTELGEOMETRY_TRIANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/shapes/simplex.h"

// This simulates an alias template.
#define PASTEL_TRIANGLE(Type, N) PASTEL_SIMPLEX(Type, (N), 2)

namespace Pastel
{

	using Triangle1 = PASTEL_TRIANGLE(real, 1);
	using Triangle2 = PASTEL_TRIANGLE(real, 2);
	using Triangle3 = PASTEL_TRIANGLE(real, 3);
	using Triangle4 = PASTEL_TRIANGLE(real, 4);
	using TriangleD = PASTEL_TRIANGLE(real, Dynamic);

}

#endif
