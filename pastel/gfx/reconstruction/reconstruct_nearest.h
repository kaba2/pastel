// Description: Non-uniform reconstruction using the k:th nearest neighbor.
// Documentation: reconstruction.txt

#ifndef PASTELGFX_RECONSTRUCT_NEAREST_H
#define PASTELGFX_RECONSTRUCT_NEAREST_H

#include "pastel/geometry/shape/alignedbox.h"

#include "pastel/sys/view/view.h"

#include <vector>

namespace Pastel
{

	/*!
	Preconditions:
	positionList.size() == dataList.size()
	kNearest > 0
	*/

	template <typename Real, int N, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const View<N, Data, Output_View>& view,
		integer kNearest,
		const NoDeduction<Real>& maxRelativeError);

	template <typename Real, int N, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const View<N, Data, Output_View>& view,
		integer kNearest);

	template <typename Real, int N, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const View<N, Data, Output_View>& view);

}

#include "pastel/gfx/reconstruct_nearest.hpp"

#endif
