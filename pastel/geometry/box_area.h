// Description: Area of a box
// Documentation: area.txt

#ifndef PASTELGEOMETRY_BOX_AREA_H
#define PASTELGEOMETRY_BOX_AREA_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <typename Real, int N>
	Real area(const AlignedBox<Real, N>& box);

	template <typename Real, int N>
	Real area(const Box<Real, N>& box);

	template <typename Real, typename Expression>
	Real boxArea(const VectorExpression<Real, 1, Expression>& width);

	template <typename Real, int N, typename Expression>
	Real boxArea(const VectorExpression<Real, N, Expression>& width);

}

#include "pastel/geometry/box_area.hpp"

#endif
