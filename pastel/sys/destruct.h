/*!
\file
\brief A function for destructing a range of objects.
*/

#ifndef PASTELSYS_DESTRUCT_H
#define PASTELSYS_DESTRUCT_H

namespace Pastel
{

	//! Destructs all objects of type Type in the given range.
	/*!
	Preconditions:
	[from, to[ forms a range.

	Time Complexity: constant for pod types, linear
	for other types
	Exception safety: nothrow
	*/

	template <typename Type, typename InputIterator>
	void destruct(InputIterator from, InputIterator to);

}

#include "pastel/sys/destruct.hpp"

#endif
