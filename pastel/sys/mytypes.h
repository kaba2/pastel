// Description: Fundamental types and macros
// Documentation: environment.txt

#ifndef PASTELSYS_MYTYPES_H
#define PASTELSYS_MYTYPES_H

// FIX: Aim to remove this dependency.
#ifdef PASTEL_ENABLE_OMP
#include "omp.h"
#endif

#include <cstddef>
#include <climits>
#include <type_traits>
#include <functional>

#define PASTEL_FWD(member) using member = typename Fwd::member

// FIX: Remove when default error-message becomes available in C++17.
#define PASTEL_STATIC_ASSERT(...) static_assert((__VA_ARGS__), #__VA_ARGS__);

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

		//! A native signed integer with the given number of bits.
		template <int N> struct Int_F {};
		template <> struct Int_F<8> { using type = int8; };
		template <> struct Int_F<16> { using type = int16; };
		template <> struct Int_F<32> { using type = int32; };
		template <> struct Int_F<64> { using type = int64; };

		//! A native signed integer with the given number of bits.
		template <int N>
		using Int = typename Int_F<N>::type;

		//! A native unsigned integer with the given number of bits.
		template <int N> 
		struct Uint_F
		{
			using type = typename std::make_unsigned<Int<N>>::type;
		};

		//! A native unsigned integer with the given number of bits.
		template <int N>
		using Uint = typename Uint_F<N>::type;

		//! A native floating-point number with the given number of bits.
		template <int N> struct Real_F {};
		template <> struct Real_F<32> { using type = real32; };
		template <> struct Real_F<64> { using type = real64; };

		//! A native floating-point number with the given number of bits.
		template <int N>
		using Real = typename Real_F<N>::type;

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

		template <typename Type>
		struct SizeInBits
		{
			// Note that 
			// std::numeric_limits<Type>::digits is 
			// CHAR_BIT for unsigned integers, and
			// CHAR_BIT - 1 for signed integers.
			// So CHAR_BIT is really what we want 
			// to use here.
			static constexpr integer value =
				sizeof(Type) * CHAR_BIT;
		};

		//! Returns the number of bits a type takes.
		/*!
		This is equal to sizeof(Type) * CHAR_BIT.
		*/
		template <typename Type>
		constexpr integer sizeInBits()
		{
			return SizeInBits<Type>::value;
		}

		//! An integer with half the number of bits as in 'integer'.
		using integer_half = Int<SizeInBits<integer>::value / 2>;
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

		template <bool Value>
		using Bool = 
			std::integral_constant<bool, Value>;

		template <integer Value>
		using Integer = 
			std::integral_constant<integer, Value>;

	}

	// FIX: Get rid of these by substituting proper concepts
	// where they are used.
	namespace Types
	{

		// An integer literal for documenting concepts.
		static constexpr integer UserDefinedInteger = 0;
		// A boolean literal for documenting concepts.
		static constexpr bool UserDefinedBoolean = true;
		// A type for documenting concepts.
		class UserDefinedType {};

	}

	namespace Types
	{

		static constexpr int Dynamic = -1;

	}

	using namespace Pastel::Types;

}

// Note that the order of the includes here is important.
// We are bringing basic functionality in one header at a time.

#include "pastel/sys/type_checks.h"
#include "pastel/sys/sfinae.h"
#include "pastel/sys/settings_type.h"
#include "pastel/sys/iterator_macros.h"
#include "pastel/sys/no_op_functions.h"
#include "pastel/sys/allocation.h"
#include "pastel/sys/named_parameter.h"
#include "pastel/sys/range.h"
#include "pastel/sys/predicate/operator_predicates.h"
#include "pastel/sys/predicate/derived_predicates.h"

#endif
