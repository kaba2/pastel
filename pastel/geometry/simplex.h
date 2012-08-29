// Description: Simplex class

#ifndef PASTELGEOMETRY_SIMPLEX_H
#define PASTELGEOMETRY_SIMPLEX_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"

// This simulates an alias template.
#define PASTEL_SIMPLEX(Type, N, M) Tuple<Vector<Type, (N)>, (M) + 1>

namespace Pastel
{

	typedef PASTEL_SIMPLEX(real, 1, 1) Simplex1;
	typedef PASTEL_SIMPLEX(real, 2, 2) Simplex2;
	typedef PASTEL_SIMPLEX(real, 3, 3) Simplex3;
	typedef PASTEL_SIMPLEX(real, 4, 4) Simplex4;

	typedef PASTEL_SIMPLEX(real, 1, 1) Simplex11;
	typedef PASTEL_SIMPLEX(real, 2, 1) Simplex21;
	typedef PASTEL_SIMPLEX(real, 3, 1) Simplex31;
	typedef PASTEL_SIMPLEX(real, 4, 1) Simplex41;

	typedef PASTEL_SIMPLEX(real, 2, 2) Simplex22;
	typedef PASTEL_SIMPLEX(real, 3, 2) Simplex32;
	typedef PASTEL_SIMPLEX(real, 4, 2) Simplex42;

	typedef PASTEL_SIMPLEX(real, 3, 3) Simplex33;
	typedef PASTEL_SIMPLEX(real, 4, 3) Simplex43;

	typedef PASTEL_SIMPLEX(real, 4, 4) Simplex44;

}

#endif
