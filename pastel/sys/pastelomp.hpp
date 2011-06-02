#ifndef PASTEL_PASTELOMP_HPP
#define PASTEL_PASTELOMP_HPP

#include "pastel/sys/pastelomp.h"

namespace Pastel
{

	inline void setNumberOfThreads(integer threads)
	{
		// FIX: There should be an ensure here.
		//ENSURE_OP(threads, >, 0);

#		if PASTEL_ENABLE_OMP != 0
		omp_set_num_threads(threads);
#		endif
	}

	inline integer numberOfThreads()
	{
		integer threads = 1;

#		if PASTEL_ENABLE_OMP != 0
		threads = omp_get_num_threads();
#		endif

		return threads;
	}

	inline integer numberOfProcessors()
	{
		integer processors = 1;

#		if PASTEL_ENABLE_OMP != 0
		processors = omp_get_num_procs();
#		endif

		return processors;
	}

}

#endif
