// Description: Printable concept for native types

#ifndef PASTELSYS_NATIVE_PRINTABLE_H
#define PASTELSYS_NATIVE_PRINTABLE_H

#include "pastel/sys/printable/printable_concept.h"

#include <sstream>
#include <type_traits>

namespace Pastel
{

	// Printable

	// Visual Studio 2015 CTP6 has a bug in SFINAE overloading,
	// which is triggered by using EnableIf here. The use of 
	// EnableIfC instead avoids the bug.

	template <
		typename Type,
		EnableIfC<
			std::is_arithmetic<Type>::value
		> = 0
		>
	std::string asString(Type that)
	{
		std::stringstream stream;
		stream << that;
		
		std::string text;
		stream >> text;

		return text;
	}

	template <
		typename Type,
		EnableIfC<
			std::is_pointer<Type>::value
		> = 0
		>
	std::string asString(Type that)
	{
		return {that};
	}

	template <
		typename Type,
		EnableIf<
			std::is_array<std::remove_reference_t<Type>>
		> = 0
		>
	std::string asString(Type that)
	{
		// Reduce arrays to pointers.
		return Pastel::asString((std::decay_t<Type>)that);
	}

	template <
		typename Type,
		EnableIf<
			std::is_enum<Type>
		> = 0
		>
	std::string asString(Type that)
	{
		// Reduce enums to integers.
		return Pastel::asString((std::underlying_type_t<Type>)that);
	}

	inline std::string asString(std::string that)
	{
		return that;
	}

}

#endif
