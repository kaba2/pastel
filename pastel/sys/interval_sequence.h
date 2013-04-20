// Description: Interval sequences

#ifndef PASTELSYS_INTERVAL_SEQUENCE_H
#define PASTELSYS_INTERVAL_SEQUENCE_H

#include "pastel/sys/integer_concept.h"
#include "pastel/sys/output_concept.h"
#include "pastel/sys/range.h"

#include <vector>

namespace Pastel
{

	//! Set-difference A\B between interval sequences.
	/*!
	Preconditions:
	aSet is an interval sequence.
	bSet is an interval sequence.
	*/
	template <
		typename A_Range, 
		typename B_Range,
		typename Output>
	void difference(
		const A_Range& aSet,
		const B_Range& bSet,
		Output report);

	//! Returns whether the given sequence is an interval sequence.
	template <typename A_Range>
	bool isIntervalSequence(
		const A_Range& aSet);

}

#include "pastel/sys/interval_sequence.hpp"

#endif
