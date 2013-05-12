// Description: Typedefs for types used in Pastel
// Documentation: environment.txt

#ifndef PASTELSYS_MYTYPES_H
#define PASTELSYS_MYTYPES_H

#include "pastel/sys/syslibrary.h"

#ifdef PASTEL_ENABLE_OMP
#include "omp.h"
#endif

#include <cstddef>
#include <type_traits>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>

#define PASTEL_PARAMETER(Type, name) \
	Self& name(Type name##__) \
	{ \
		name##_ = name##__; \
		return *this; \
	} \
	Type name##_

#define PASTEL_REMOVE_BRACKETS(x) typename Pastel::RemoveBrackets<void (x)>::Type
// FIX: Remove this version after C++11 typename-fix becomes available in Visual Studio.
#define PASTEL_REMOVE_BRACKETS_(x) Pastel::RemoveBrackets<void (x)>::Type

#define PASTEL_NO_DEDUCTION(x) PASTEL_REMOVE_BRACKETS(x)

#define PASTEL_FWD(member) typedef typename Fwd::member member
// FIX: Remove this version after C++11 typename-fix becomes available in Visual Studio.
#define PASTEL_FWD_(member) typedef Fwd::member member

#define PASTEL_ITERATOR_FUNCTIONS_PREFIX(prefix, name, assign) \
	prefix##Iterator name() \
	{ \
		return prefix##Iterator(assign); \
	} \
	\
	prefix##ConstIterator name() const \
	{ \
		return prefix##ConstIterator(assign); \
	} \
	\
	prefix##ConstIterator c##name() const \
	{ \
		return prefix##ConstIterator(assign); \
	}

#define PASTEL_ITERATOR_FUNCTIONS(name, assign) \
	PASTEL_ITERATOR_FUNCTIONS_PREFIX(, name, assign)

#define PASTEL_RANGE_FUNCTIONS_PREFIX(prefix, name, begin_, end_) \
	prefix##Range name() \
	{ \
		return Pastel::range( begin_ (), end_ ()); \
	} \
	\
	prefix##ConstRange c##name() const \
	{ \
		return Pastel::range( c##begin_ (), c##end_ ()); \
	}

#define PASTEL_RANGE_FUNCTIONS(name, begin, end) \
	PASTEL_RANGE_FUNCTIONS_PREFIX(, name, begin, end)
	
#define PASTEL_CALL_BRACKETS ()

#define PASTEL_STATIC_ASSERT(x) static_assert((x), #x);

#if (defined _WIN32 || defined _WIN64)
#	define PASTEL_DELETE
#else
#	define PASTEL_DELETE = delete
#endif

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

#define PASTEL_VISIBLE_IF(Condition, ReturnType) \
	typename boost::mpl::if_< \
	(Condition), \
	EmptyClass, PASTEL_REMOVE_BRACKETS(ReturnType)>::type

#define PASTEL_VISIBLE_IF_C(Condition, ReturnType) \
	typename boost::mpl::if_c< \
	(Condition), \
	EmptyClass, PASTEL_REMOVE_BRACKETS(ReturnType)>::type

#define PASTEL_RANGE_ALGORITHM(FunctionName, Name) \
	class Name \
	{ \
	public: \
		template <typename Range> \
		void operator()(const Range& range) const \
		{ \
			FunctionName(range); \
		} \
	};

namespace Pastel
{

	namespace Types
	{

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
		// We check these assumptions at compile-time
		// in pastelsys.cpp.

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

		namespace MyTypes_
		{

			template <int N>
			struct IntegerOfSize {};

			template <>
			struct IntegerOfSize<2>
			{
				typedef int16 type;
			};

			template <>
			struct IntegerOfSize<4>
			{
				typedef int32 type;
			};

			template <>
			struct IntegerOfSize<8>
			{
				typedef int64 type;
			};

		}

		//! Integer capable of holding a pointer.
		/*!
		Preconditions:
		sizeof(void*) == sizeof(pointer_integer)
		'pointer_integer' is a native signed integer type.
		*/
		typedef MyTypes_::IntegerOfSize<sizeof(void*)>::type 
			pointer_integer;

		//! Abstract native integer type
		/*!
		Preconditions:
		sizeof(integer) >= sizeof(int32)
		sizeof(integer) <= sizeof(int64)
		'integer' is a native signed integer type.
	
		If PASTEL_LARGE_INTEGER is defined, then additionally:
		sizeof(integer) >= min(sizeof(void*), sizeof(int64)) 
		*/
	#	ifdef PASTEL_LARGE_INTEGER
			typedef std::conditional<
				sizeof(void*) <= sizeof(int64),
				pointer_integer, int64>::type integer;
	#	else
			typedef int32 integer;
	#	endif

		typedef std::make_unsigned<integer>::type uinteger;
	
		//! Abstract native real type
		/*!
		Preconditions:
		'real' is a native floating point type.
		*/
		typedef real64 real;

		//! Integer for holding hash integers.
		/*!
		An integer of this type is used to hold hash
		values.
		*/
		typedef std::size_t hash_integer;

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

		// An integer literal for documenting concepts.
		static const integer UserDefinedInteger = 0;
		// A boolean literal for documenting concepts.
		static const bool UserDefinedBoolean = true;
		// A type for documenting concepts.
		class PASTELSYS UserDefinedType {};

		static const integer Dynamic = -1;
		//enum {Dynamic = -1};

		class PASTELSYS EmptyClass {};

	}

	using namespace Pastel::Types;

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
	PASTEL_ENABLE_IF(std::is_arithmetic<Type>, Type) mabs(Type that);

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

}

#include "pastel/sys/ensure.h"
#include "pastel/sys/integer_integer.h"
#include "pastel/sys/float_real.h"
#include "pastel/sys/double_real.h"
#include "pastel/sys/predicates.h"
#include "pastel/sys/range.h"

#include "pastel/sys/mytypes.hpp"

#endif
