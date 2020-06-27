// Description: Range input
// Documentation: input.txt

#ifndef PASTELSYS_RANGE_INPUT_H
#define PASTELSYS_RANGE_INPUT_H

#include "pastel/sys/range.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <type_traits>
#include <iterator>

namespace Pastel
{

	//! Range input
	template <typename Range>
	class Range_Input
	{
	private:
		using Iterator = ranges::iterator_t<Range>;
		using Type = ranges::range_value_t<Range>;

		static constexpr bool IsRandomAccess =
			std::is_same<
				typename boost::iterator_traversal<Iterator>::type,
				boost::random_access_traversal_tag>::value;

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
			if (IsRandomAccess)
			{
				return std::distance(begin_, end_);
			}

			return 0;
		}

		template <
			typename Type = void,
			typename = EnableIfC<IsRandomAccess, Type>
		>
		integer n() const
		{
			return nHint();
		}

		bool empty() const
		{
			return begin_ == end_;
		}

		decltype(auto) get() const
		{
			PENSURE(!empty());
			return *begin_;
		}

		template <
			typename Type = void,
			typename = EnableIfC<IsRandomAccess, Type>
		>
		decltype(auto) operator[](integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, n());

			return begin_[i];
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
	decltype(auto) rangeInput(
		const Iterator& begin, const Iterator& end)
	{
		return Pastel::rangeInput(range(begin, end));
	}

}

#endif
