// Description: Class wrapping

#ifndef PASTELSYS_CLASS_H
#define PASTELSYS_CLASS_H

#include <type_traits>

namespace Pastel
{

	//! A class wrapper for non-class types.
	/*!
	Preconditions:
	std::is_scalar<Type>::value || 
	std::is_void<Type>::value

	Type != void:
	The wrapper class contains a member of the given 
	Type, and is implicitly convertible to a (const/non-const)
	reference to that member. In addition, the 
	wrapper has a constructor which accepts an
	initializer of type Type. This way the wrapper
	acts as transparently as possible to make it
	possible to inherit from a native type.

	Type == void:
	The result is an empty class, with trivial comparison 
	operators, and a trivial hash function. This is useful 
	for empty base-class optimization.

	Note:
	Only non-class types are allowed to be forwarded.
	The class types are already classes	so there is
	no sense in wrapping them. Use the Class template 
	alias below to form Class's conditionally.
	*/
	template <typename Type>
	class Class;

	//! Wraps a non-class type into a class.
	template <typename Type>
	using As_Class = 
		typename std::conditional<
		std::is_scalar<Type>::value ||
		std::is_void<Type>::value,
		Class<Type>, Type
		>::type;

}

#include "pastel/sys/class.hpp"

#endif
