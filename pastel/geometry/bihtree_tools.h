// Description: Tools for working with BihTree

#ifndef PASTEL_BIHTREETOOLS_H
#define PASTEL_BIHTREETOOLS_H

#include "pastel/geometry/bihtree.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/intersect_line_alignedbox.h"
#include "pastel/geometry/intersect_line_alignedplane.h"

namespace Pastel
{

	//! Intersects a line with the bih tree.

	template <int N, typename Real, typename Shape,
		typename IntersectionQuery>
		bool intersect(
		const BihTree<N, Real, Shape>& bihTree,
		const Line<N, Real>& line,
		IntersectionQuery& shapeQuery);

	//! Tests the invariants in the bih tree.

	/*!
	The invariants tested are the following:
	- The splitting position must be inside
	  the node aligned box.
	- The child aligned boxes must be inside the
	  parent aligned box.
	*/

	template <int N, typename Real, typename Shape>
	bool check(const BihTree<N, Real, Shape>& bihTree);

}


#include "pastel/geometry/bihtree_tools.hpp"

#endif
