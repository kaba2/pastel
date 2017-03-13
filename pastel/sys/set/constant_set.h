// Description: Constant multi-set

#ifndef PASTELSYS_CONSTANT_SET_H
#define PASTELSYS_CONSTANT_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/ensure.h"
#include <range/v3/view/repeat_n.hpp>

namespace Pastel
{

	template <typename Element>
	decltype(auto) constantSet(integer n, Element&& element)
	{
		return ranges::view::repeat_n(std::forward<Element>(element), n);
	}

}

#endif
