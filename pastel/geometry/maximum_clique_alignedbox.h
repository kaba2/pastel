// Description: Maximum clique of an aligned box graph

#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_H
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_H

#include "pastel/sys/iteratorrange.h"

#include <vector>
#include <iterator>

namespace Pastel
{
	
	class MaximumClique_BoxType
	{
	public:
		enum Enum
		{
			Open,
			Closed
		};
	};
	
	//! Finds an aligned box of maximum intersection among aligned boxes.
	/*!
	boxSet:
	An iterator range where AlignedBox_ConstIterator
	dereferences to AlignedBox<Real, N>.

	boxType:
	Whether to interpret boxes as open or closed sets.
	Notice that if in x-direction a box is half-open of
	the form [a, b), or (a, b], then the result of this 
	algorithm is the same as using an x-open box.
	The same goes for the y-direction. Thus no generality 
	is lost by concentrating just on open and closed boxes.

	sweepDirection:
	The axis on which the width of the maximum clique box
	is secondarily maximized among all maximum cliques.

	result:
	An iterator which dereferences to something that can
	be assigned an AlignedBox_ConstIterator.

	Preconditions:
	The dimension of the aligned boxes are 2.
	N == 2 || N == Dynamic.

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
		maximumClique(
		const ForwardIterator_Range<AlignedBox_ConstIterator>& boxSet,
		MaximumClique_BoxType::Enum boxType,
		integer sweepDirection,
		AlignedBox_ConstIterator_Iterator result);

	//! Finds an aligned box of maximum intersection among aligned boxes.
	/*!
	This is a convenience function that calls:
	maximumClique(boxSet, boxType, sweepDirection, NullIterator())

	See the documentation for the general function.
	*/
	template <typename AlignedBox_ConstIterator>
	typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(
		const ForwardIterator_Range<AlignedBox_ConstIterator>& boxSet,
		MaximumClique_BoxType::Enum boxType = MaximumClique_BoxType::Closed,
		integer sweepDirection = 1);

}

#include "pastel/geometry/maximum_clique_alignedbox.hpp"

#endif
