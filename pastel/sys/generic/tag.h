// Description: A compile-time tag parametrized by a string

#ifndef PASTELSYS_TAG_H
#define PASTELSYS_TAG_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

#define PASTEL_TAG(name) Pastel::Tag<#name##_tag>()

namespace Pastel
{

	using tag_integer = uint32;
	PASTEL_STATIC_ASSERT(std::is_unsigned<tag_integer>::value);

	template <tag_integer Hash>
	struct Tag {};

	//! Returns the hash for a tag-name.
	inline constexpr tag_integer operator "" _tag(const char* tagName, std::size_t n)
	{
		// This is the Fowler-Noll-Vo 1a hash-function (FNV-1a) for 32-bit integers.
		return n > 0
			? (operator""_tag(tagName, n - 1) ^ (tag_integer)tagName[n - 1]) * 16777619UL
			: 2166136261UL;
	}

}

#include "pastel/sys/generic/is_tag.h"
#include "pastel/sys/generic/tag_hash.h"

#endif
