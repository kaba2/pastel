// Description: Non-uniform reconstruction using normalized convolution
// Documentation: reconstruction.txt

#ifndef PASTELGFX_RECONSTRUCT_FILTER_H
#define PASTELGFX_RECONSTRUCT_FILTER_H

#include "pastel/sys/view/view.h"

#include "pastel/geometry/shape/alignedbox.h"

#include <vector>

namespace Pastel
{

	template <typename Real, int N, typename Data, typename Filter, typename Output_View>
	void reconstructFilter(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const Filter& filter,
		const NoDeduction<Real>& filterRadius,
		const View<N, Data, Output_View>& view);

}

#include "pastel/gfx/reconstruct_filter.hpp"

#endif
