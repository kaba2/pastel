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

#define PASTEL_NO_DEDUCTION(x) PASTEL_REMOVE_BRACKETS(x)

#define PASTEL_FWD(member) typedef typename Fwd::member member

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

#define PASTEL_ENABLE_IF(Condition, ReturnType) \
	typename boost::enable_if< \
	PASTEL_REMOVE_BRACKETS(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type

#define PASTEL_ENABLE_IF_P(Condition) \
	PASTEL_ENABLE_IF(Condition, MyTypes_::Enabler) = MyTypes_::Enabler()

#define PASTEL_ENABLE_IF_C(Condition, ReturnType) \
	typename boost::enable_if_c< \
	(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_ENABLE_IF_C_P(Condition) \
	PASTEL_ENABLE_IF_C(Condition, MyTypes_::Enabler) = MyTypes_::Enabler()

#define PASTEL_DISABLE_IF(Condition, ReturnType) \
	typename boost::disable_if< \
	PASTEL_REMOVE_BRACKETS(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_DISABLE_IF_P(Condition) \
	PASTEL_DISABLE_IF(Condition, MyTypes_::Enabler) = MyTypes_::Enabler()

#define PASTEL_DISABLE_IF_C(Condition, ReturnType) \
	typename boost::disable_if_c< \
	(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_DISABLE_IF_C_P(Condition) \
	PASTEL_DISABLE_IF_C(Condition, MyTypes_::Enabler) = MyTypes_::Enabler()

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

		using schar = signed char;
		using longlong = long long;
		
		using uchar = unsigned char;
		using ushort = unsigned short;
		using uint = unsigned int;
		using ulong = unsigned long;
		using ulonglong = unsigned long long;

		// Absolute size types

		// PORTABILITY: the size of the native
		// types is not defined in standard C++.
		// We check these assumptions at compile-time
		// in pastelsys.cpp.

		using int8 = char;
		using int16 = short;
		using int32 = int;
		using int64 = long long;

		using uint8 = std::make_unsigned<int8>::type;
		using uint16 = std::make_unsigned<int16>::type;
		using uint32 = std::make_unsigned<int32>::type;
		using uint64 = std::make_unsigned<int64>::type;

		using real32 = float;
		using real64 = double;
		using real32_ieee = real32;
		using real64_ieee = real64;

		namespace MyTypes_
		{

			struct Enabler {};


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
		using pointer_integer = std::ptrdiff_t;

		//! Abstract native integer type
		/*!
		This should be the native integer type on the computer.
		For example, on 32-bit computers we expect this to be 32-bit,
		and on 64-bit computers we expect this to be 64-bit.
		*/
		using integer = std::make_signed<std::size_t>::type;
		using uinteger = std::size_t;
	
		//! Abstract native real type
		/*!
		Preconditions:
		'real' is a native floating point type.
		*/
		using real = real64;

		//! Integer for holding hash integers.
		/*!
		An integer of this type is used to hold hash
		values.
		*/
		using hash_integer = std::size_t;

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
		class RemoveBrackets {};

		template <typename T>
		class RemoveBrackets<void (T)>
		{
		public:
			using Type = T;
		};

		template <>
		class RemoveBrackets<void ()>
		{
		public:
			using Type = void;
		};

		// An integer literal for documenting concepts.
		static const integer UserDefinedInteger = 0;
		// A boolean literal for documenting concepts.
		static const bool UserDefinedBoolean = true;
		// A type for documenting concepts.
		class PASTELSYS UserDefinedType {};

		static const int Dynamic = -1;
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

	//! Casts a reference to a const-reference.
	template <typename Type>
	const Type& addConst(Type& that);

	//! Casts a const-reference to a reference.
	template <typename Type>
	Type& removeConst(const Type& that);

	//! Returns the number of bits a type takes.
	/*!
	This is equal to sizeof(Type) * CHAR_BIT.
	*/
	template <typename Type>
	integer sizeInBits();

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
