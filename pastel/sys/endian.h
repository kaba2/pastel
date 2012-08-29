// Description: Endian conversion

#ifndef PASTELSYS_ENDIAN_H
#define PASTELSYS_ENDIAN_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

namespace Pastel
{

	//! Returns true if the native format is little-endian.
	bool isLittleEndian();

	//! Converts a little-endian scalar to the native format and vice versa.
	template <typename Type>
	Type littleEndian(const Type& that);

	//! Converts a big-endian scalar to the native format and vice versa.
	template <typename Type>
	Type bigEndian(const Type& that);

}

#include "pastel/sys/endian.hpp"

#endif
