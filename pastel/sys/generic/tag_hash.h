// Description: Retrieves the hash from a tag.

#ifndef PASTELSYS_TAG_HASH_H
#define PASTELSYS_TAG_HASH_H

#include "pastel/sys/generic/tag.h"

#include <type_traits>

namespace Pastel
{

	namespace Tag_Hash_
	{

		template <typename Type>
		struct Tag_Hash__
		{
			enum : int
			{
				value = 0
			};
		};

		template <tag_integer Hash>
		struct Tag_Hash__<Tag<Hash>>
		{
			enum : tag_integer
			{
				value = Hash
			};
		};

	}

	template <typename Type>
	using Tag_Hash = 
		Tag_Hash_::Tag_Hash__<
			std::remove_cv_t<
				std::remove_reference_t<Type>
			>
		>;

}

#endif
