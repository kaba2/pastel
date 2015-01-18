// Description: Algorithms for aligned boxes

#ifndef PASTELGEOMETRY_ALIGNEDBOX_TOOLS_H
#define PASTELGEOMETRY_ALIGNEDBOX_TOOLS_H

#include "pastel/sys/alignedbox/alignedbox_tools.h"

#include "pastel/math/affine/affine_transformation.h"

namespace Pastel
{

	template <typename Real>
	AlignedBox<Real, 3> operator*(
		const AlignedBox<Real, 3>& left,
		const AffineTransformation<Real>& right);

}

#include "pastel/geometry/shape/alignedbox_tools.hpp"

#endif
