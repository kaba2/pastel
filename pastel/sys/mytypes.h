// Description: Fundamental types and macros
// Documentation: environment.txt

#ifndef PASTELSYS_MYTYPES_H
#define PASTELSYS_MYTYPES_H

#include "pastel/sys/deduction_macros.h"
#include "pastel/sys/no_op_functions.h"
#include "pastel/sys/sfinae_macros.h"
#include "pastel/sys/iterator_macros.h"
#include "pastel/sys/settings_type.h"
#include "pastel/sys/named_parameter.h"

#ifdef PASTEL_ENABLE_OMP
#include "omp.h"
#endif

#include <cstddef>
#include <climits>
#include <type_traits>

#define PASTEL_FWD(member) using member = typename Fwd::member

#define PASTEL_CALL_BRACKETS ()

#define PASTEL_STATIC_ASSERT(x) static_assert((x), #x);

#define PASTEL_TYPE_FUNCTION(F) \
	template <typename... TypeSet> \
	using F##_t = \
		typename F##<TypeSet...>::type;

#if _MSC_VER <= 1900
#	define PASTEL_CONSTEXPR const
#	define PASTEL_NOEXCEPT_C(x)
#	define PASTEL_NOEXCEPT
#else
#	define PASTEL_CONSTEXPR constexpr
#	define PASTEL_NOEXCEPT_C(x) noexcept(x)
#	define PASTEL_NOEXCEPT noexcept
#endif

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

		// Parametrized absolute size types.

		namespace Int_
		{

			template <int N> struct Int {};
			template <> struct Int<8> { using type = int8; };
			template <> struct Int<16> { using type = int16; };
			template <> struct Int<32> { using type = int32; };
			template <> struct Int<64> { using type = int64; };

		}

		template <int N>
		using Int = typename Int_::Int<N>::type;

		namespace Uint_
		{

			template <int N> 
			struct Uint 
			{
				using type = typename std::make_unsigned<Int<N>>::type;
			};

		}

		template <int N>
		using Uint = typename Uint_::Uint<N>::type;

		namespace Real_
		{

			template <int N> struct Real {};
			template <> struct Real<32> { using type = real32; };
			template <> struct Real<64> { using type = real64; };

		}

		template <int N>
		using Real = typename Real_::Real<N>::type;

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
		using uinteger = unsigned int;
		using integer = std::make_signed<uinteger>::type;

		//! An integer with half the number of bits as in 'integer'.
		using integer_half = Int<(sizeof(integer) * CHAR_BIT) / 2>;
		using uinteger_half = std::make_unsigned<integer_half>::type;
	
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

	}

	namespace Types
	{

		// An integer literal for documenting concepts.
		static PASTEL_CONSTEXPR integer UserDefinedInteger = 0;
		// A boolean literal for documenting concepts.
		static PASTEL_CONSTEXPR bool UserDefinedBoolean = true;
		// A type for documenting concepts.
		class UserDefinedType {};

		static PASTEL_CONSTEXPR int Dynamic = -1;

		class EmptyClass {};

		namespace MyTypes_
		{

			struct Enabler {};

		}

	}

	using namespace Pastel::Types;

	//! Returns the number of bits a type takes.
	/*!
	This is equal to sizeof(Type) * CHAR_BIT.
	*/
	template <typename Type>
	integer sizeInBits();

	template <typename Type, EnableIf<std::is_arithmetic<Type>> = 0>
	Type mabs(Type that);

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

#include "pastel/sys/integer/native_integer.h"
#include "pastel/sys/real/native_real.h"
#include "pastel/sys/predicate.h"
#include "pastel/sys/range.h"
#include "pastel/sys/type_checks.h"
#include "pastel/sys/type_traits.h"

#include "pastel/sys/mytypes.hpp"

#endif
