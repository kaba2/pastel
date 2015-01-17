// Description: Orthogonal range search in a kd-tree
// Documentation: pointkdtree_search_range.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_H
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_H

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/shapes/alignedbox.h"

#include "pastel/sys/output/output_concept.h"

#include <vector>

namespace Pastel
{

	//! Finds all points contained in an aligned box using a kd-tree.
	/*!
	Preconditions:
	kdTree.n() == range.n()
	kdTree.n() <= 32
	*/
	template <
		typename Settings, template <typename> class Customization, 
		typename Point_ConstIterator_Output>
	void searchRange(
		const PointKdTree<Settings, Customization>& kdTree,
		const AlignedBox<typename Settings::Real, Settings::N>& range,
		Point_ConstIterator_Output report);

}

#include "pastel/geometry/pointkdtree/pointkdtree_search_range.hpp"

#endif
