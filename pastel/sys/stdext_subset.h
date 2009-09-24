#ifndef PASTEL_STDEXT_SUBSET_H
#define PASTEL_STDEXT_SUBSET_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace StdExt
	{

		template <typename RandomAccess_Iterator>
		void subset(
			RandomAccess_Iterator begin,
			RandomAccess_Iterator end,
			integer k);
	
	}

}

#include "pastel/sys/stdext_subset.hpp"

#endif
