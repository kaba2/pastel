// Description: Range reporter

#ifndef PASTELSYS_RANGE_OUTPUT_H
#define PASTELSYS_RANGE_OUTPUT_H

#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	template <typename Iterator>
	class Range_Output
	{
	public:
		explicit Range_Output(
			Iterator begin,
			Iterator end)
			: begin_(begin)
			, end_(end)
		{
		}

		template <typename That>
		bool operator()(That&& that) const
		{
			if (begin_ != end_)
			{
				*begin_ = std::forward<That>(that);
				++begin_;
			}

			return begin_ != end_;
		}

	private:
		mutable Iterator begin_;
		Iterator end_;
	};

	template <ranges::forward_range Range>
	Range_Output<ranges::iterator_t<Range>> rangeOutput(
		const Range& range)
	{
		typedef ranges::iterator_t<Range>
			Iterator;

		return Range_Output<Iterator>(
			range.begin(), range.end());
	}

}

#endif
