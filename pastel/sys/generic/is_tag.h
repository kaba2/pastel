// Description: Returns whether a type is tag.

#ifndef PASTELSYS_IS_TAG_H
#define PASTELSYS_IS_TAG_H

#include "pastel/sys/generic/tag.h"

#include <type_traits>

namespace Pastel
{

	namespace IsTag_
	{

		template <typename Type>
		struct IsTag_F_
			: std::false_type
		{};

		template <tag_integer Hash>
		struct IsTag_F_<Tag<Hash>>
			: std::true_type
		{};

	}

	template <typename Type>
	using IsTag_F = 
		IsTag_::IsTag_F_<
			std::remove_cv_t<
				std::remove_reference_t<Type>
			>
		>;

	template <typename Type>
	using IsTag = 
		typename IsTag_F<Type>::type;

}

#endif
