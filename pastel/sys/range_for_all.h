// Description: Check if a predicate holds between corresponding elements
// Documentation: range_algorithms.txt

#ifndef PASTEL_RANGE_FOR_ALL_H
#define PASTEL_RANGE_FOR_ALL_H

#include "pastel/sys/range_concepts.h"

namespace Pastel
{

	//! Returns if a predicate holds between corresponding elements.
	template <
		typename A_ForwardRange,
		typename B_ForwardRange,
		typename RelationAlgorithm>
	bool rangeForAll(
		A_ForwardRange aRange,
		B_ForwardRange bRange,
		RelationAlgorithm predicate);

}

#include "pastel/sys/range_for_all.hpp"

#endif
