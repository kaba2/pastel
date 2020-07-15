// Description: Transformed set

#ifndef PASTELSYS_TRANSFORMED_SET_H
#define PASTELSYS_TRANSFORMED_SET_H

#include "pastel/sys/range/range_concept.h"
#include "pastel/sys/function/function_concept.h"
#include <range/v3/view/transform.hpp>

namespace Pastel
{

	template <
		Range_Concept Range,
		typename Transform>
	decltype(auto) transformRange(
		Range&& set,
		Transform&& transform)
	{
		return ranges::views::transform(
			std::forward<Range>(set),
			std::forward<Transform>(transform)
		);
	}

}

#endif
