// Description: copy_n
// Documentation: sequence_algorithms.txt

#ifndef PASTELSYS_COPY_N_H
#define PASTELSYS_COPY_N_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Source_ForwardIterator,
	typename Target_ForwardIterator>
	void copy_n(
		const Source_ForwardIterator& sourceBegin,
		integer count,
		const Target_ForwardIterator& targetBegin);

}

#include "pastel/sys/copy_n.hpp"

#endif
