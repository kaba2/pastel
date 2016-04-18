// Description: A compile-time tag parametrized by a string

#ifndef PASTELSYS_TAG_H
#define PASTELSYS_TAG_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

#define PASTEL_TAG(name) Pastel::Tag<Pastel::tagHash(#name)>()

namespace Pastel
{

	using tag_integer = uint32;
	PASTEL_STATIC_ASSERT(std::is_unsigned<tag_integer>::value);

	template <tag_integer Hash>
	struct Tag {};

	//! Returns the hash for a tag-name.
	/*!
	Note that the N also contains the null-character at 
	the end of the string. That is, for "liquid",
	N = 6 + 1 = 7.
	*/
	template <integer N>
	constexpr tag_integer tagHash(
		const char (&tagName)[N],
		// Skip the null-character at the end.
		integer i = N - 1)
	{
		// This is the Fowler-Noll-Vo 1a hash-function (FNV-1a) for 32-bit integers.
		return i > 0
			? (tagHash(tagName, i - 1) ^ (tag_integer)tagName[i - 1]) * 16777619UL
			: 2166136261UL;
	}

	//! Returns the hash for a tag-name.
	/*!
	This should be named just tagHash(). However, a bug in Visual Studio 2015
	update 2 forces this function to have another name.
	*/
	constexpr tag_integer tagHashString(
		const char* tagName,
		std::size_t n)
	{
		// This is the Fowler-Noll-Vo 1a hash-function (FNV-1a) for 32-bit integers.
		return n > 0
			? (tagHashString(tagName, n - 1) ^ (tag_integer)tagName[n - 1]) * 16777619UL
			: 2166136261UL;
	}

	inline constexpr tag_integer operator "" _tag(const char* tagName, std::size_t n)
	{
		return tagHashString(tagName, n);
	}

}

#include "pastel/sys/generic/is_tag.h"
#include "pastel/sys/generic/tag_hash.h"

#endif
