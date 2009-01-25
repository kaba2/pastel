#ifndef PASTELGFX_FILTERRECONSTRUCTOR_H
#define PASTELGFX_FILTERRECONSTRUCTOR_H

#include "pastel/sys/view.h"

#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real, typename Data, typename Filter, typename Output_View>
	void reconstructFilter(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const Filter& filter,
		const PASTEL_NO_DEDUCTION(Real)& filterRadius,
		const View<N, Data, Output_View>& view);

}

#include "pastel/gfx/filterreconstructor.hpp"

#endif
