// Description: Index of the highest bit
// Documentation: bit_tricks.txt

#ifndef PASTEL_HIGHEST_BIT_H
#define PASTEL_HIGHEST_BIT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the index of the highest 1-bit of the given integer.
	/*!
	If the given integer is zero, returns -1.
	*/
	integer highestBit(uint8 data);

	//! Returns the index of the highest 1-bit of the given integer.
	/*!
	If the given integer is zero, returns -1.
	*/
	integer highestBit(uint16 data);

	//! Returns the index of the highest 1-bit of the given integer.
	/*!
	If the given integer is zero, returns -1.
	*/
	integer highestBit(uint32 data);

}

#include "pastel/sys/highest_bit.hpp"

#endif
