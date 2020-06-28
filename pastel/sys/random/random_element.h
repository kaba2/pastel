// Description: Random element in a range

#ifndef PASTELSYS_RANDOM_ELEMENT_H
#define PASTELSYS_RANDOM_ELEMENT_H

#include "pastel/sys/random/random_integer.h"
#include "pastel/sys/range.h"

#include <initializer_list>

namespace Pastel
{

	template <ranges::random_access_range Range>
	decltype(auto) randomElement(Range&& elements)
	{
		PENSURE_OP(ranges::size(elements), >, 0);
		return elements[randomInteger(ranges::size(elements))];
	}

	template <typename Type>
	decltype(auto) randomElement(
		const std::initializer_list<Type>& elementSet)
	{
		using Pastel::randomElement;
		return randomElement(range(elementSet));
	}

}

#endif
