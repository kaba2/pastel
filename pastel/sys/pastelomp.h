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

	//! Returns the number of currently executing threads in OpenMP.
	/*!
	Note: In a serial section this is always 1.
	*/
	integer numberOfThreads();

	//! Returns the number of processors in the machine.
	integer numberOfProcessors();

}

#include "pastel/sys/pastelomp.hpp"

#endif
