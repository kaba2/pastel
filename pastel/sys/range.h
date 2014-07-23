// Description: Iterator ranges
// Documentation: iterator.txt

#ifndef PASTELSYS_RANGE_H
#define PASTELSYS_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <boost/range/iterator_range.hpp>
#include <boost/range/metafunctions.hpp>

namespace Pastel
{

	template <typename Iterator>
	boost::iterator_range<Iterator> range(
		const Iterator& begin,
		integer size)
	{
		//ENSURE_OP(size, >=, 0);
		return boost::make_iterator_range(
			begin, std::next(begin, size));
	}

	template <typename Iterator>
	boost::iterator_range<Iterator> range(
		const Iterator& begin,
		const Iterator& end)
	{
		return boost::make_iterator_range(begin, end);
	}

	template <typename Type, int N>
	boost::iterator_range<Type*> range(
		Type (&that)[N])
	{
		return boost::make_iterator_range(
			std::begin(that), std::end(that));
	}
	
	template <typename Type>
	boost::iterator_range<const Type*> range(
		const std::initializer_list<Type>& that)
	{
		return boost::make_iterator_range(
			std::begin(that), std::end(that));
	}

}

#endif
