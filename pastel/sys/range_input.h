// Description: Range input
// Documentation: inputs.txt

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
	private:
		using Iterator = typename boost::range_iterator<const Range>::type;
		using Type = typename boost::range_value<Range>::type;

	public:
		Range_Input()
			: begin_()
			, end_()
		{
		}

		Range_Input(const Range& range)
			: begin_(std::begin(range))
			, end_(std::end(range))
		{
		}

		integer nHint() const
		{
			if (boost::is_same<
				typename boost::iterator_traversal<Iterator>::type,
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
	Range_Input<Range> rangeInput(
		const Range& range)
	{
		return range;
	}

	template <typename Iterator>
	Range_Input<boost::iterator_range<Iterator>> rangeInput(
		const Iterator& begin, const Iterator& end)
	{
		return Pastel::rangeInput(range(begin, end));
	}

}

#endif
