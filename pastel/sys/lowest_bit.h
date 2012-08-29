// Description: Index of the lowest bit
// Documentation: bit_tricks.txt

#ifndef PASTEL_LOWEST_BIT_H
#define PASTEL_LOWEST_BIT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the index of the lowest 1-bit of the given integer.
	/*!
	If the given integer is zero, returns -1.
	*/
	integer lowestBit(uint8 data);

	//! Returns the index of the lowest 1-bit of the given integer.
	/*!
	If the given integer is zero, returns -1.
	*/
	integer lowestBit(uint16 data);

	//! Returns the index of the lowest 1-bit of the given integer.
	/*!
	If the given integer is zero, returns -1.
	*/
	integer lowestBit(uint32 data);

}

#include "pastel/sys/lowest_bit.hpp"

#endif
