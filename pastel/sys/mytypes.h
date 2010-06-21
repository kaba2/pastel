// Description: Typedefs for types used in Pastel
// Documentation: environment.txt

#ifndef PASTEL_MYTYPES_H
#define PASTEL_MYTYPES_H

#include "pastel/sys/environment.h"
#include "pastel/sys/syslibrary.h"
#include "pastel/sys/pastelomp.h"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/static_assert.hpp>

#define PASTEL_NO_DEDUCTION(x) typename Pastel::ParenthesesRemover<void (x)>::Type
//#define PASTEL_STATIC_ASSERT(x) static_assert((x), #x);
#define PASTEL_STATIC_ASSERT(x) BOOST_STATIC_ASSERT(x);

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
	typedef uint32             pointer_integer;

	template <typename T>
	class ParenthesesRemover
	{
	};

	template <typename T>
	class ParenthesesRemover<void (T)>
	{
	public:
		typedef T Type;
	};

	enum
	{
		Dynamic = -1
	};

	class PASTELSYS UserDefinedType {};
	class PASTELSYS EmptyClass {};
	class PASTELSYS TrueType {};
	class PASTELSYS FalseType {};

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
	typename boost::enable_if<boost::is_arithmetic<Type>, Type>::type mabs(Type that);

	void deallocateRaw(const void* data);
	void* allocateRaw(integer size);

}

#include "pastel/sys/mytypes.hpp"

#include "pastel/sys/nativereal.h"

#endif
