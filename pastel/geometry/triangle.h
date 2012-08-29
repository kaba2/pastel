// Description: Triangle class

#ifndef PASTELGEOMETRY_TRIANGLE_H
#define PASTELGEOMETRY_TRIANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/simplex.h"

// This simulates an alias template.
#define PASTEL_TRIANGLE(Type, N) PASTEL_SIMPLEX(Type, (N), 2)

namespace Pastel
{

	typedef PASTEL_TRIANGLE(real, 1) Triangle1;
	typedef PASTEL_TRIANGLE(real, 2) Triangle2;
	typedef PASTEL_TRIANGLE(real, 3) Triangle3;
	typedef PASTEL_TRIANGLE(real, 4) Triangle4;
	typedef PASTEL_TRIANGLE(real, Dynamic) TriangleD;

}

#endif
