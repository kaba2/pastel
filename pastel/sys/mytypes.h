// Description: Typedefs for types used in Pastel
// Documentation: environment.txt

#ifndef PASTEL_MYTYPES_H
#define PASTEL_MYTYPES_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/pastelomp.h"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/if.hpp>

#define PASTEL_REMOVE_BRACKETS(x) typename Pastel::RemoveBrackets<void (x)>::Type
#define PASTEL_NO_DEDUCTION(x) PASTEL_REMOVE_BRACKETS(x)

//#define PASTEL_STATIC_ASSERT(x) static_assert((x), #x);
#define PASTEL_STATIC_ASSERT(x) BOOST_STATIC_ASSERT(x);

#define PASTEL_ENABLE_IF(Condition, ReturnType) \
	typename boost::enable_if< \
	PASTEL_REMOVE_BRACKETS(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type

#define PASTEL_ENABLE_IF_C(Condition, ReturnType) \
	typename boost::enable_if_c< \
	(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_DISABLE_IF(Condition, ReturnType) \
	typename boost::disable_if< \
	PASTEL_REMOVE_BRACKETS(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_DISABLE_IF_C(Condition, ReturnType) \
	typename boost::disable_if_c< \
	(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_RANGE_ALGORITHM(FunctionName, Name) \
	class Name \
	{ \
	public: \
		template <typename Iterator> \
		void operator()(const ForwardRange<Iterator>& range) const \
		{ \
			FunctionName(range); \
		} \
	};

namespace Pastel
{

	// PORTABILITY: long long is not standard C++

	// Shortened native types

	typedef signed char        schar;

	typedef long long          longlong;

	typedef unsigned char      uchar;
	typedef unsigned short     ushort;
	typedef unsigned int       uint;
	typedef unsigned long      ulong;
	typedef unsigned long long ulonglong;

	// Absolute size types

	// PORTABILITY: the size of the native
	// types is not defined in standard C++.

	typedef char               int8;
	typedef short              int16;
	typedef int                int32;
	typedef long long          int64;

	typedef unsigned char      uint8;
	typedef unsigned short     uint16;
	typedef unsigned int       uint32;
	typedef unsigned long long uint64;

	typedef float              real32;
	typedef double             real64;
	typedef float              real32_ieee;
	typedef double             real64_ieee;

	// Compile-time decision types
	/*!
	'integer' type must be able to hold at least the same
	numbers as a 32-bit signed native integer.
	*/
	typedef int                integer;
	typedef double	           real;

	typedef boost::mpl::if_c<
		(sizeof(void*) == 4), uint32, uint64>::type 
		pointer_integer;

	//! Removes brackets around a type.
	/*!
	To transmit a type containing commas through a macro,
	it needs to be enclosed in brackets. However, then it
	is possible that the type does not make sense anymore.
	Therefore, PASTEL_REMOVE_BRACKETS macro turns this
	bracketed type into a function type 'void (T)'. From
	this form we can again regain the type inside the
	brackets using RemoveBrackets.
	*/
	template <typename T>
	class RemoveBrackets
	{
	};

	template <typename T>
	class RemoveBrackets<void (T)>
	{
	public:
		typedef T Type;
	};

	template <>
	class RemoveBrackets<void ()>
	{
	public:
		typedef void Type;
	};

	// A literal for documenting concepts.
	static const integer UserDefinedInteger = 0;
	// A type for documenting concepts.
	class PASTELSYS UserDefinedType {};

	static const integer Dynamic = -1;
	//enum {Dynamic = -1};

	class PASTELSYS EmptyClass {};

	//! A no-op function taking a variable.
	/*!
	The purpose of this function is to supress warnings
	that are caused by not using a local variable.
	Just call this function with that variable to
	get rid of the warning message.
	*/
	template <typename Type>
	void unused(const Type&);

	template <typename Type>
	PASTEL_ENABLE_IF(boost::is_arithmetic<Type>, Type) mabs(Type that);

	//! Allocates a raw memory block.
	/*!
	size:
	The size of the memory block in bytes.

	Preconditions:
	size > 0
	*/
	void* allocateRaw(integer size);

	//! Deallocates a raw memory block.
	void deallocateRaw(const void* data);

	//! Tests two variables for equivalence.
	/*!
	The elements 'left' and 'right' are equivalent 
	under a comparison 'compare' if it holds that
	!compare(left, right) && !compare(right, left).
	*/
	template <typename Type, typename Compare>
	bool equivalent(
		const Type& left, const Type& right,
		const Compare& compare);

	//! Tests two variables for equivalence.
	/*!
	This is a convenience function that uses
	std::less<Type> as the comparison functor
	for the more general equivalent() function.
	See the documentation for that.
	*/
	template <typename Type>
	bool equivalent(
		const Type& left, const Type& right);

}

#include "pastel/sys/mytypes.hpp"

#include "pastel/sys/integer_integer.h"
#include "pastel/sys/float_real.h"
#include "pastel/sys/double_real.h"

#endif
