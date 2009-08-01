// Description: Destruction of a sequence of objects

#ifndef PASTEL_STDEXT_DESTRUCT_H
#define PASTEL_STDEXT_DESTRUCT_H

namespace Pastel
{

	namespace StdExt
	{

		//! Destructs all objects of type Type in the given range.
		/*!
		Preconditions:
		[from, to[ forms a range.

		Time Complexity: constant for pod types, linear
		for other types
		Exception safety: nothrow
		*/

		template <typename Type>
		void destruct(Type* from, Type* to);

	}

}

#include "pastel/sys/stdext_destruct.hpp"

#endif
