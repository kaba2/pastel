// Description: Actual memory address of an object
// Documentation: memory.txt

#ifndef PASTELSYS_ADDRESSOF_H
#define PASTELSYS_ADDRESSOF_H

namespace Pastel
{

	//! Returns the memory address of the given object.
	/*!
	Because operator& can be overloaded, the memory address
	of a given object can't sometimes be obtained using it.
	This function does the required fiddling to return the
	dreal memory address of the object.
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

#include "pastel/sys/generic/addressof.hpp"

#endif
