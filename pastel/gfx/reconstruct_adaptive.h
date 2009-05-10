#ifndef PASTELGFX_RECONSTRUCT_ADAPTIVE_H
#define PASTELGFX_RECONSTRUCT_ADAPTIVE_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/sys/view.h"

#include <vector>

namespace Pastel
{

	/*!
	Preconditions:
	positionList.size() == dataList.size()
	kNearest > 0
	*/

	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructAdaptive(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const View<N, Data, Output_View>& view,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

}

#include "pastel/gfx/reconstruct_adaptive.hpp"

#endif
