// Description: Maximum clique of an aligned box graph
// Documentation: intersection_graph.txt

#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_H
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_H

#include "pastel/sys/forwardrange.h"

#include <vector>
#include <iterator>

namespace Pastel
{
	
	//! Finds an aligned box of maximum intersection among aligned boxes.
	/*!
	Preconditions:
	The dimension of the aligned boxes are 2.
	AlignedBox_ConstIterator dereferences to AlignedBox<Real, N>,
	where N == 2 or N == Dynamic.

	Time complexity:
	O(n log n)

	Let '(V, E)' be a graph where the elements of 'V' are the aligned 
	boxes. Let '(v_1, v_2) in E subset V^2' if and only if 'v_1' and
	'v_2' overlap. Then a maximum clique in this graph gives the set
	of those boxes whose common intersection contains a maximum
	number of boxes
	*/
	template <
		typename AlignedBox_ConstIterator,
		typename AlignedBox_ConstIterator_Iterator>
	typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(const ForwardRange<AlignedBox_ConstIterator>& boxSet,
		AlignedBox_ConstIterator_Iterator result);

	//! Finds an aligned box of maximum intersection among aligned boxes.
	/*!
	This is a convenience function that calls:
	maximumClique(boxSet, NullIterator())

	See the documentation for the general function.
	*/
	template <typename AlignedBox_ConstIterator>
	typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(
		const ForwardRange<AlignedBox_ConstIterator>& boxSet);

}

#include "pastel/geometry/maximum_clique_alignedbox.hpp"

#endif
