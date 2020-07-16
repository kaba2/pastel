// Description: Constant multi-set

#ifndef PASTELSYS_CONSTANT_RANGE_H
#define PASTELSYS_CONSTANT_RANGE_H

#include "pastel/sys/range/range_concept.h"
#include "pastel/sys/ensure.h"
#include <range/v3/view/repeat_n.hpp>

namespace Pastel
{

	template <typename Element>
	decltype(auto) constantRange(Element&& element, integer n)
	{
		return ranges::views::repeat_n(std::forward<Element>(element), n);
	}

}

#endif
