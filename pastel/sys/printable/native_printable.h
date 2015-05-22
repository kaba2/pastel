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
	// which is triggered by using Requires here. The use of 
	// RequiresC instead avoids the bug.

	template <
		typename Type,
		RequiresC<
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
		RequiresC<
			Or<
				std::is_same<Type, char>,
				std::is_same<Type, wchar_t>,
				std::is_same<Type, char16_t>,
				std::is_same<Type, char32_t>
			>::value
		> = 0
		>
	std::string asString(const Type* that)
	{
		return {that};
	}

	template <
		typename Type,
		RequiresC<
			std::is_enum<Type>::value
		> = 0
		>
	std::string asString(Type that)
	{
		using Pastel::asString;
		// Reduce enums to integers.
		return asString((std::underlying_type_t<Type>)that);
	}

	inline std::string asString(std::string that)
	{
		return that;
	}

}

#endif
