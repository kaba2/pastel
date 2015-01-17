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

	template <typename Range>
	Range_Output<typename boost::range_iterator<Range>::type> rangeOutput(
		const Range& range)
	{
		typedef typename boost::range_iterator<Range>::type
			Iterator;

		return Range_Output<Iterator>(
			range.begin(), range.end());
	}

}

#endif
