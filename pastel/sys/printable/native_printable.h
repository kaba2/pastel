// Description: Printable concept for native types

#ifndef PASTELSYS_NATIVE_PRINTABLE_H
#define PASTELSYS_NATIVE_PRINTABLE_H

#include "pastel/sys/printable/printable_concept.h"

#include <sstream>
#include <type_traits>

namespace Pastel
{

	// Printable

	template <Arithmetic_Concept Type>
	std::string asString(Type that)
	{
		std::stringstream stream;
		stream << that;
		
		std::string text;
		stream >> text;

		return text;
	}

	template <typename Type>
	requires (
		std::is_same_v<Type, char> ||
		std::is_same_v<Type, wchar_t> ||
		std::is_same_v<Type, char16_t> ||
		std::is_same_v<Type, char32_t>)
	std::string asString(const Type* that)
	{
		return {that};
	}

	template <typename Type>
	requires (std::is_enum<Type>::value)
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
