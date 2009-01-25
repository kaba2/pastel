#ifndef PASTELGFX_NEARESTRECONSTRUCTOR_H
#define PASTELGFX_NEARESTRECONSTRUCTOR_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/sys/view.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const View<N, Data, Output_View>& view);

}

#include "pastel/gfx/nearestreconstructor.hpp"

#endif
