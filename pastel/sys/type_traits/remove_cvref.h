// Description: Remove const-volatile-reference from a type
// Documentation: type_traits.txt

#ifndef PASTELSYS_REMOVE_CVREF_H
#define PASTELSYS_REMOVE_CVREF_H

#include <type_traits>

namespace Pastel
{

	//! Removes const-volatile-reference from a type.
	template <typename Type>
	using RemoveCvRef = std::remove_cvref_t<Type>;

}

#endif
