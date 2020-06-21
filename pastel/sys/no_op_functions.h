// Description: No-op functions

#ifndef PASTELSYS_NO_OP_FUNCTIONS_H
#define PASTELSYS_NO_OP_FUNCTIONS_H

namespace Pastel
{

	//! Suppresses warnings from unused variables.
	/*!
	The purpose of this function is to supress warnings
	that are caused by not using a local variable.
	It doesn't do anything by itself.
	*/
	template <typename... Type>
	void unused(const Type&...)
	{
		// Do nothing.
	}

	//! Casts a reference to a const-reference.
	/*!
	This function is useful, for example, in a data-structure
	which provides a find() function. Depending on the constness
	of the data-structure the find() should return either an
	iterator or a const-iterator. The addConst() is used
	to redirect the iterator-version to the const-iterator
	version, after which the const-iterator is casted to an
	iterator.
	*/
	template <typename Type>
	const Type& addConst(Type& that)
	{
		return (const Type&)that;
	}

	//! Casts a const-reference to a reference.
	/*!
	See the documentation for addConst().
	*/
	template <typename Type>
	Type& removeConst(const Type& that)
	{
		return (Type&)that;
	}

	//! Casts a const-reference to a value.
	template <typename Type>
	Type removeReference(const Type& that)
	{
		return that;
	}

}

#endif
