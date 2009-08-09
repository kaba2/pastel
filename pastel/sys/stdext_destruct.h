// Description: Destruction of objects

#ifndef PASTEL_STDEXT_DESTRUCT_H
#define PASTEL_STDEXT_DESTRUCT_H

namespace Pastel
{

	namespace StdExt
	{

		//! Destructs all objects of type Type in the given range.
		/*!
		Preconditions:
		[begin, end[ forms a range.

		Time Complexity: 
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

}

#include "pastel/sys/stdext_destruct.hpp"

#endif
