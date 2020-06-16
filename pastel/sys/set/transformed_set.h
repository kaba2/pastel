// Description: Transformed set

#ifndef PASTELSYS_TRANSFORMED_SET_H
#define PASTELSYS_TRANSFORMED_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/function/function_concept.h"
#include <range/v3/view/transform.hpp>

namespace Pastel
{

	template <
		typename Set,
		typename Transform>
	decltype(auto) transformedSet(
		Set&& set,
		Transform&& transform)
	{
		auto result = ranges::views::transform(
				std::forward<Set>(set),
				std::forward<Transform>(transform)
			);
		return result;
	}

}

#endif
