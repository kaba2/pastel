// Description: Range input

#ifndef PASTELSYS_RANGE_INPUT_H
#define PASTELSYS_RANGE_INPUT_H

#include "pastel/sys/range.h"

#include <boost/type_traits/is_same.hpp>

#include <iterator>

namespace Pastel
{

	template <typename Range>
	class Range_Input
	{
	public:
		typedef typename boost::range_iterator<Range>::type Iterator;
		typedef typename boost::range_value<Range>::type Type;

		Range_Input(Range range)
			: begin_(std::begin(range))
			, end_(std::end(range))
		{
		}

		integer nHint() const
		{
			if (boost::is_same<
				boost::iterator_traversal<Iterator>::type,
				boost::random_access_traversal_tag>::value)
			{
				return std::distance(begin_, end_);
			}

			return -1;
		}

		bool empty() const
		{
			return begin_ == end_;
		}

		Type operator()()
		{
			return *(begin_++);
		}

	private:
		Iterator begin_;
		Iterator end_;
	};

	template <typename Range>
	Range_Input<Range> rangeInput(Range range)
	{
		return Range_Input<Range>(range);
	}

}

#endif
