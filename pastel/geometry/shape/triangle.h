// Description: Triangle class

#ifndef PASTELGEOMETRY_TRIANGLE_H
#define PASTELGEOMETRY_TRIANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/shape/simplex.h"

// This simulates an alias template.
#define PASTEL_TRIANGLE(Type, N) PASTEL_SIMPLEX(Type, (N), 2)

namespace Pastel
{

	using Triangle1 = PASTEL_TRIANGLE(dreal, 1);
	using Triangle2 = PASTEL_TRIANGLE(dreal, 2);
	using Triangle3 = PASTEL_TRIANGLE(dreal, 3);
	using Triangle4 = PASTEL_TRIANGLE(dreal, 4);
	using TriangleD = PASTEL_TRIANGLE(dreal, Dynamic);

}

#endif
