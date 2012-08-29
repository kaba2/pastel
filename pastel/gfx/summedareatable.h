// Description: Summed-area tables
// Documentation: summedareatable.txt

#ifndef PASTELGFX_SUMMEDAREATABLE_H
#define PASTELGFX_SUMMEDAREATABLE_H

#include "pastel/sys/view.h"
#include "pastel/sys/alignedbox.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Computes a summed area table.
	/*!
	A summed area table is such that the 'sumImage' element in position P
	contains the sum of all the 'image' elements in positions {Q | allLessEqual(Q, P)}.
	*/

	template <int N, typename Type, typename Image_ConstView, typename Sum_View>
	void computeSummedAreaTable(
		const ConstView<N, Type, Image_ConstView>& image,
		const View<N, Type, Sum_View>& sumImage);

	//! Returns the sum of the elements on a given region.
	/*!
	sumImage:
	A summed area table computed with 'computeSummedAreaTable()'.

	region:
	The rectangular region to sum over.
	*/

	template <typename Type, typename Sum_ConstView>
	Type summedAreaTable(
		const ConstView<2, Type, Sum_ConstView>& sumImage,
		const AlignedBox2i& region);

}

#include "pastel/gfx/summedareatable.hpp"

#endif
