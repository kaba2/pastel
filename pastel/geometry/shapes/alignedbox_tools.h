// Description: Algorithms for aligned boxes

#ifndef PASTELGEOMETRY_ALIGNEDBOX_TOOLS_H
#define PASTELGEOMETRY_ALIGNEDBOX_TOOLS_H

#include "pastel/sys/alignedbox_tools.h"

#include "pastel/math/affine_transformation.h"

namespace Pastel
{

	template <typename Real>
	AlignedBox<Real, 3> operator*(
		const AlignedBox<Real, 3>& left,
		const AffineTransformation<Real>& right);

}

#include "pastel/geometry/shapes/alignedbox_tools.hpp"

#endif
