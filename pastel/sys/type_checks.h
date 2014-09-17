// Description: Native type checks
// Documentation: portability.txt

#ifndef PASTELSYS_TYPE_CHECKS_H
#define PASTELSYS_TYPE_CHECKS_H

#include "pastel/sys/mytypes.h"

#include <climits>
#include <type_traits>

namespace Pastel
{

	// Check that 'char' is 8 bits.
	PASTEL_STATIC_ASSERT(
		CHAR_MIN == (-127 - 1) && CHAR_MAX == 127);
	PASTEL_STATIC_ASSERT(
		UCHAR_MAX == 0xFF);

	// Check that 'short int' is 16 bits.
	PASTEL_STATIC_ASSERT(
		SHRT_MIN == (-32767 - 1) && SHRT_MAX == 32767);
	PASTEL_STATIC_ASSERT(
		USHRT_MAX == 0xFFFF);

	// Check that 'int' is 32 bits.
	PASTEL_STATIC_ASSERT(
		INT_MIN == (-2147483647 - 1) && INT_MAX == 2147483647);
	PASTEL_STATIC_ASSERT(
		UINT_MAX == 0xFFFFFFFF);

	// Check that 'real' is a native floating point type.
	PASTEL_STATIC_ASSERT(
		std::is_floating_point<real>::value);

	// Check that 'integer' is at least 32 bits wide.
	PASTEL_STATIC_ASSERT(
		sizeof(integer) >= sizeof(int32));

	// Check that 'integer' is a native signed integer type.
	PASTEL_STATIC_ASSERT(
		std::is_signed<integer>::value);
	
	// Check that 'pointer_integer' is of the same size as a pointer.
	PASTEL_STATIC_ASSERT(
		sizeof(pointer_integer) == sizeof(void*));

	// Check that 'pointer_integer' is a native signed integer type.
	PASTEL_STATIC_ASSERT(
		std::is_signed<pointer_integer>::value);

	PASTEL_STATIC_ASSERT(
		sizeof(uint8) == sizeof(uchar));
	PASTEL_STATIC_ASSERT(
		sizeof(uint16) == 2 * sizeof(uint8));
	PASTEL_STATIC_ASSERT(
		sizeof(uint32) == 4 * sizeof(uint8));
	PASTEL_STATIC_ASSERT(
		sizeof(uint64) == 8 * sizeof(uint8));

	PASTEL_STATIC_ASSERT(
		sizeof(real32) == 4 * sizeof(uint8));
	PASTEL_STATIC_ASSERT(
		sizeof(real64) == 8 * sizeof(uint8));

	PASTEL_STATIC_ASSERT(
		sizeof(int8) == sizeof(uint8));
	PASTEL_STATIC_ASSERT(
		sizeof(int16) == sizeof(uint16));
	PASTEL_STATIC_ASSERT(
		sizeof(int32) == sizeof(uint32));
	PASTEL_STATIC_ASSERT(
		sizeof(int64) == sizeof(uint64));

}

#endif
