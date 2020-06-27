// Description: Iterator ranges
// Documentation: iterator.txt

#ifndef PASTELSYS_RANGE_H
#define PASTELSYS_RANGE_H

#include "pastel/sys/mytypes.h"

#include <range/v3/view/subrange.hpp>
#include <range/v3/algorithm.hpp>
#include <boost/iterator_adaptors.hpp>

namespace Pastel
{

	template <typename Iterator>
	decltype(auto) range(
		const Iterator& begin,
		integer size)
	{
		//ENSURE_OP(size, >=, 0);
		return ranges::subrange(begin, std::next(begin, size));
	}

	template <typename Iterator>
	decltype(auto) range(
		const Iterator& begin,
		const Iterator& end)
	{
		return ranges::subrange(begin, end);
	}

	template <typename Type, integer N>
	decltype(auto) range(
		Type (&that)[N])
	{
		return ranges::subrange(
			std::begin(that), std::end(that));
	}
	
	template <typename Type>
	decltype(auto) range(
		const std::initializer_list<Type>& that)
	{
		return ranges::subrange(
			std::begin(that), std::end(that));
	}

}

#endif
