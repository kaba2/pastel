// Description: Conditionally includes omp.h
// Documentation: environment.txt

#ifndef PASTEL_PASTELOMP_H
#define PASTEL_PASTELOMP_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Sets the number of maximum threads to use for OpenMP.
	/*!
	Preconditions:
	threads > 0
	*/
	void setNumberOfThreads(integer threads);

}

#include "pastel/sys/pastelomp.hpp"

#endif
