#ifndef PASTEL_STDEXT_COPY_N_H
#define PASTEL_STDEXT_COPY_N_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace StdExt
	{

		template <typename Source_ForwardIterator,
		typename Target_ForwardIterator>
		void copy_n(
			const Source_ForwardIterator& sourceBegin,
			integer count,
			const Target_ForwardIterator& targetBegin);

	}

}

#include "pastel/sys/stdext_copy_n.hpp"

#endif
