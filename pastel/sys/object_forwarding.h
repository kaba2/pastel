// Description: Object forwarding

#ifndef PASTEL_OBJECT_FORWARDING_H
#define PASTEL_OBJECT_FORWARDING_H

#include <type_traits>

namespace Pastel
{

	//! A forwarding class for non-class types.
	/*!
	Preconditions:
	!std::is_class<Type>

	Note:
	If Type is void, then the result is an empty class.
	This is useful for empty-base class optimization.

	Note:
	Only non-class types are allowed to be forwarded.
	The class types are already classes	so there is
	no sense in forwarding them. Use the MakeClass
	meta-function below to form Class's conditionally.
	*/
	template <typename Type>
	class Class;

	//! Turns a non-class type into a forwarding class.
	/*!
	Note:
	Preserves a class type.
	*/
	template <typename Type>
	class MakeClass
	{
	public:
		typedef typename std::conditional<
			std::is_class<Type>::value,
			Type,
			Class<Type>>::type type;
	};

}

#include "pastel/sys/object_forwarding.hpp"

#endif
