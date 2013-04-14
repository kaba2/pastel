// Description: Interval sequences

#ifndef PASTELSYS_INTERVAL_SEQUENCE_H
#define PASTELSYS_INTERVAL_SEQUENCE_H

#include "pastel/sys/integer_concept.h"
#include "pastel/sys/reporter_concept.h"

#include <vector>

namespace Pastel
{

	//! Set-difference A\B between interval sequences.
	/*!
	Preconditions:
	aSet is an interval sequence.
	bSet is an interval sequence.
	*/
	template <typename Integer, typename Reporter>
	void difference(
		const std::vector<Integer>& aSet,
		const std::vector<Integer>& bSet,
		Reporter report);

	//! Returns whether the given sequence is an interval sequence.
	template <typename Integer>
	bool isIntervalSequence(
		const std::vector<Integer>& aSet);

}

#include "pastel/sys/interval_sequence.hpp"

#endif
