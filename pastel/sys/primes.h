// Description: Prime numbers

#ifndef PASTELSYS_PRIMES_H
#define PASTELSYS_PRIMES_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

namespace Pastel
{

	//! Returns some prime following the given number.
	/*!
	The returned prime is not necessarily the next 
	following prime.
	*/
	PASTELSYS ulong followingPrime(ulong minimum);

}


#endif
