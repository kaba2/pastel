// Description: Tools for computing the area of a box
// Documentation: area.txt

#ifndef PASTEL_BOXAREA_H
#define PASTEL_BOXAREA_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <int N, typename Real>
	Real area(const AlignedBox<N, Real>& box);

	template <int N, typename Real>
	Real area(const Box<N, Real>& box);

	template <typename Real, typename Expression>
	Real boxArea(const VectorExpression<1, Real, Expression>& width);

	template <int N, typename Real, typename Expression>
	Real boxArea(const VectorExpression<N, Real, Expression>& width);

}

#include "pastel/geometry/boxarea.hpp"

#endif
