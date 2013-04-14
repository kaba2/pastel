// Description: Interval sequences

#ifndef PASTELSYS_INTERVAL_SEQUENCE_H
#define PASTELSYS_INTERVAL_SEQUENCE_H

#include "pastel/sys/integer_concept.h"
#include "pastel/sys/reporter_concept.h"

#include <vector>

namespace Pastel
{

	template <typename Integer, typename Reporter>
	void difference(
		const std::vector<Integer>& aSet,
		const std::vector<Integer>& bSet,
		Reporter report);

}

#include "pastel/sys/interval_sequence.hpp"

#endif
