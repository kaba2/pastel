// Description: Tools for computing the area of a box
// Documentation: area.txt

#ifndef PASTEL_BOXAREA_H
#define PASTEL_BOXAREA_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <int N, typename Real>
	Real area(const AlignedBox<Real, N>& box);

	template <int N, typename Real>
	Real area(const Box<Real, N>& box);

	template <typename Real, typename Expression>
	Real boxArea(const VectorExpression<Real, 1, Expression>& width);

	template <int N, typename Real, typename Expression>
	Real boxArea(const VectorExpression<Real, N, Expression>& width);

}

#include "pastel/geometry/boxarea.hpp"

#endif
