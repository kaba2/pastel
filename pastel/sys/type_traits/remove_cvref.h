// Description: Remove const-volatile-reference from a type
// Documentation: type_traits.txt

#ifndef PASTELSYS_REMOVE_CVREF_H
#define PASTELSYS_REMOVE_CVREF_H

#include <type_traits>

namespace Pastel
{

	//! Removes const-volatile-reference from a type.
	template <typename Type>
	struct RemoveCvRef_F
	: std::remove_cv<
		std::remove_reference_t<Type>
	>
	{};

	template <typename Type>
	using RemoveCvRef =
		typename RemoveCvRef_F<Type>::type;

}

#endif
