// Description: Class wrapping

#ifndef PASTELSYS_CLASS_H
#define PASTELSYS_CLASS_H

#include <type_traits>

namespace Pastel
{

	//! A tagged class-wrapper using membership.
	/*!
	Preconditions:
	!std::is_class<Type>::value

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
	*/
	template <
		typename Type, 
		typename Tag = void>
	class Member_Class;

	//! A tagged class-wrapper using inheritance.
	/*!
	Preconditions:
	std::is_class<Type>::value

	The Type is made a base-class of the Inherited_Class.
	Constructors are perfectly forwarded to Type.
	*/
	template <
		typename Type, 
		typename Tag = void>
	class Inherited_Class;

	//! Wraps class -> Inherited_Class, non-class -> Member_Class.
	/*!
	This class is needed to get around the buggy support for
	alias templates in Visual Studio 14 CTP4. That is, while
	typename As_Class<Type, Tag>::type usually works ok,
	Class<Type, Tag> sometimes causes a bug.
	*/
	template <
		typename Type,
		typename Tag = void>
	struct As_Class
	: std::conditional<
		std::is_class<Type>::value,
		Inherited_Class<Type, Tag>,
		Member_Class<Type, Tag>
	>
	{};

	template <
		typename Type,
		typename Tag = void>
	using Class = 
		typename As_Class<Type, Tag>::type;

}

#include "pastel/sys/generic/class.hpp"

#endif
