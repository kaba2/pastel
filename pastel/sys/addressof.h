// Description: Actual memory address of an object
// Documentation: memory.txt

#ifndef PASTEL_ADDRESSOF_H
#define PASTEL_ADDRESSOF_H

namespace Pastel
{

	//! Returns the memory address of the given object.
	/*!
	Because operator& can be overloaded, the memory address
	of a given object can't sometimes be obtained using it.
	This function does the required fiddling to return the
	real memory address of the object.
	*/
	template <typename Type>
	void* addressOf(Type& that);

	//! Returns the memory address of the given object.
	/*!
	See the documentation for the non-const version.
	*/
	template <typename Type>
	const void* addressOf(const Type& that);

}

#include "pastel/sys/addressof.hpp"

#endif
