#ifndef PASTELGEOMETRY_GRIDTOOLS_H
#define PASTELGEOMETRY_GRIDTOOLS_H

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/grid.h"

#include "pastel/sys/point.h"

namespace Pastel
{

	template <int N, typename Real, typename Object, typename VisitorFunctor>
	void visit(
		const Grid<N, Real, Object>& grid,
		const Rectangle<N>& region,
		VisitorFunctor& visitor);

	template <int N, typename Real, typename Object, typename PositionFunctor, typename Metric>
	std::pair<typename Grid<N, Real, Object>::ConstObjectIterator, Real>
		findNearest(
		const Grid<N, Real, Object>& grid,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& radius,
		const PositionFunctor& positionFunctor,
		const Metric& metric);

}

#include "pastel/geometry/grid_tools.hpp"

#endif
