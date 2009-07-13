// Description: Tools for obtaining the index of the highest/lowest bit

/*!
\file
\brief Tools for obtaining the index of the highest/lowest bit.
*/

#ifndef PASTEL_BITQUERY_H
#define PASTEL_BITQUERY_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the index of the highest 1-bit of the given integer.

	/*!
	If the given integer is zero, returns -1.
	*/
	integer highestBit(uint8 data);

	//! Returns the index of the lowest 1-bit of the given integer.

	/*!
	If the given integer is zero, returns -1.
	*/
	integer lowestBit(uint8 data);

	//! Returns the index of the highest 1-bit of the given integer.

	/*!
	If the given integer is zero, returns -1.
	*/
	integer highestBit(uint16 data);

	//! Returns the index of the lowest 1-bit of the given integer.

	/*!
	If the given integer is zero, returns -1.
	*/
	integer lowestBit(uint16 data);

	//! Returns the index of the highest 1-bit of the given integer.

	/*!
	If the given integer is zero, returns -1.
	*/
	integer highestBit(uint32 data);

	//! Returns the index of the lowest 1-bit of the given integer.

	/*!
	If the given integer is zero, returns -1.
	*/
	integer lowestBit(uint32 data);

}

#include "pastel/sys/bitquery.hpp"

#endif
