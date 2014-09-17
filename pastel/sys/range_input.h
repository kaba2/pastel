// Description: Range input
// Documentation: input.txt

#ifndef PASTELSYS_RANGE_INPUT_H
#define PASTELSYS_RANGE_INPUT_H

#include "pastel/sys/range.h"

#include <type_traits>
#include <iterator>

namespace Pastel
{

	//! Range input
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
			static PASTEL_CONSTEXPR bool IsRandomAccess =
				std::is_same<
					typename boost::iterator_traversal<Iterator>::type,
					boost::random_access_traversal_tag>::value;

			if (IsRandomAccess)
			{
				return std::distance(begin_, end_);
			}

			return 0;
		}

		bool empty() const
		{
			return begin_ == end_;
		}

		using Return = decltype(*std::declval<Iterator>());

		// FIX: Replace with decltype(auto) after
		// Visual Studio 2013 fixes its bugs.
		Return get() const
		{
			PENSURE(!empty());
			return *begin_;
		}

		void pop()
		{
			PENSURE(!empty());
			++begin_;
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
