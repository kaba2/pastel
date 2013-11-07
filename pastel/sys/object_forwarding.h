// Description: Object forwarding

#ifndef PASTELSYS_OBJECT_FORWARDING_H
#define PASTELSYS_OBJECT_FORWARDING_H

#include <type_traits>

namespace Pastel
{

	//! A forwarding class for non-class types.
	/*!
	Note:
	If Type is void, then the result is an empty class.
	This is useful for empty-base class optimization.

	Note:
	Only non-class types are allowed to be forwarded.
	The class types are already classes	so there is
	no sense in forwarding them. Use the AsClass
	meta-function below to form Class's conditionally.
	*/
	template <typename Type>
	class Class_Forward;

	template <>
	class Class_Forward<void> {};

	//! Turns a non-class type into a forwarding class.
	/*!
	Note:
	Preserves a class type.
	*/
	template <typename Type>
	class AsClass
	{
	public:
		typedef typename std::conditional<
			std::is_scalar<Type>::value ||
			std::is_void<Type>::value,
			Class_Forward<Type>, Type
			>::type type;
	};

	//! Turns a non-class type into a forwarding class.
	template <typename Type>
	using Class = typename AsClass<Type>::type;

}

#include "pastel/sys/object_forwarding.hpp"

#endif
