// Description: Maximum clique of an aligned box graph

#ifndef PASTELGEOMETRY_MAXIMUM_CLIQUE_ALIGNEDBOX_H
#define PASTELGEOMETRY_MAXIMUM_CLIQUE_ALIGNEDBOX_H

#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/range.h"

#include "pastel/geometry/shapes/alignedbox.h"

#include <vector>
#include <iterator>

namespace Pastel
{

	//! Finds an aligned box of maximum intersection among aligned boxes.
	/*!
	boxSet:
	An iterator range where AlignedBox_ConstIterator
	dereferences to AlignedBox<Real, N>.

	sweepDirection:
	The axis on which the width of the maximum clique box
	is secondarily maximized among all maximum cliques.

	result:
	An iterator which dereferences to something that can
	be assigned an AlignedBox_ConstIterator. Will be
	filled with iterators to the boxes which are part of
	the maximum clique.

	Preconditions:
	The dimension of the aligned boxes are 2.
	N == 2 || N == Dynamic.

	Time complexity:
	O(n log n)

	Let '(V, E)' be a graph where the elements of 'V' are the aligned 
	boxes. Let '(v_1, v_2) in E subset V^2' if and only if 'v_1' and
	'v_2' overlap. Then a maximum clique in this graph gives the set
	of those boxes whose common intersection contains a maximum
	number of boxes.
	*/
	template <
		typename AlignedBox_ConstRange,
		typename AlignedBox_Output>
	typename boost::range_value<AlignedBox_ConstRange>::type 
		maximumClique(
		const AlignedBox_ConstRange& boxSet,
		integer sweepDirection,
		const AlignedBox_Output& result);

	//! Finds an aligned box of maximum intersection among aligned boxes.
	/*!
	This is a convenience function that calls:
	maximumClique(boxSet, sweepDirection, Null_Output())

	See the documentation for the general function.
	*/
	template <typename AlignedBox_ConstRange>
	typename boost::range_value<AlignedBox_ConstRange>::type 
		maximumClique(
		const AlignedBox_ConstRange& boxSet,
		integer sweepDirection = 1);

}

#include "pastel/geometry/maximum_clique_alignedbox.hpp"

#endif
