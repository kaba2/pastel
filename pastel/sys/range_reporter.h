// Description: Range reporter

#ifndef PASTEL_RANGE_REPORTER_H
#define PASTEL_RANGE_REPORTER_H

#include "pastel/sys/reporter_concept.h"

namespace Pastel
{

	template <typename Iterator>
	class Range_Reporter
	{
	public:
		explicit Range_Reporter(
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
	Range_Reporter<typename boost::range_iterator<Range>::type> rangeReporter(
		const Range& range)
	{
		typedef typename boost::range_iterator<Range>::type
			Iterator;

		return Range_Reporter<Iterator>(
			range.begin(), range.end());
	}

}

#endif
