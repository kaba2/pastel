#ifndef PASTEL_PASTELOMP_HPP
#define PASTEL_PASTELOMP_HPP

#include "pastel/sys/pastelomp.h"

namespace Pastel
{

	inline void setNumberOfThreads(integer threads)
	{
		//ENSURE_OP(threads, >=, 0);

		if (threads == 0)
		{
#			if PASTEL_ENABLE_OMP != 0
			threads = omp_get_num_procs();
#			endif
		}

#		if PASTEL_ENABLE_OMP != 0
		omp_set_num_threads(threads);
#		endif
	}

}

#endif
