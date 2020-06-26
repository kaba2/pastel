// Description: Simplex class

#ifndef PASTELGEOMETRY_SIMPLEX_H
#define PASTELGEOMETRY_SIMPLEX_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"

// This simulates an alias template.
#define PASTEL_SIMPLEX(Type, N, M) Tuple<Vector<Type, (N)>, (M) + 1>

namespace Pastel
{

	using Simplex1 = PASTEL_SIMPLEX(dreal, 1, 1);
	using Simplex2 = PASTEL_SIMPLEX(dreal, 2, 2);
	using Simplex3 = PASTEL_SIMPLEX(dreal, 3, 3);
	using Simplex4 = PASTEL_SIMPLEX(dreal, 4, 4);

	using Simplex11 = PASTEL_SIMPLEX(dreal, 1, 1);
	using Simplex21 = PASTEL_SIMPLEX(dreal, 2, 1);
	using Simplex31 = PASTEL_SIMPLEX(dreal, 3, 1);
	using Simplex41 = PASTEL_SIMPLEX(dreal, 4, 1);

	using Simplex22 = PASTEL_SIMPLEX(dreal, 2, 2);
	using Simplex32 = PASTEL_SIMPLEX(dreal, 3, 2);
	using Simplex42 = PASTEL_SIMPLEX(dreal, 4, 2);

	using Simplex33 = PASTEL_SIMPLEX(dreal, 3, 3);
	using Simplex43 = PASTEL_SIMPLEX(dreal, 4, 3);

	using Simplex44 = PASTEL_SIMPLEX(dreal, 4, 4);

}

#endif
