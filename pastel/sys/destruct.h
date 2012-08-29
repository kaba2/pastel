// Description: Destruction of objects
// Documentation: sequence_algorithms.txt

#ifndef PASTELSYS_DESTRUCT_H
#define PASTELSYS_DESTRUCT_H

namespace Pastel
{

	//! Destructs all objects of type Type in the given range.
	/*!
	Preconditions:
	[begin, end[ forms a range.

	Time complexity: 
	* for types with trivial destructors: constant
	* otherwise: linear
	Exception safety: nothrow

	This function takes care of optimization in case the
	destructor of Type is trivial.
	*/

	template <typename Type>
	void destruct(Type* begin, Type* end);

	//! Destructs an object of type Type.
	/*!
	Exception safety: nothrow

	This function takes care of optimization in case the
	destructor of Type is trivial.
	*/
	template <typename Type>
	void destruct(Type* that);

}

#include "pastel/sys/destruct.hpp"

#endif
