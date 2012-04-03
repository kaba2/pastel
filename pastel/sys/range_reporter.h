// Description: Range reporter

#ifndef PASTEL_RANGE_REPORTER_H
#define PASTEL_RANGE_REPORTER_H

#include "pastel/sys/reporter_concept.h"
#include "pastel/sys/range_concepts.h"

namespace Pastel
{

	template <typename Range>
	class Range_Reporter
	{
	public:
		explicit Range_Reporter(const Range& range)
			: range_(range)
		{
		}

		template <typename That>
		bool operator()(That&& that) const
		{
			if (!range_.empty())
			{
				range_.front() = std::forward<That>(that);
				range_.pop_front();
			}

			return !range_.empty();
		}

	private:
		mutable Range range_;
	};

	template <typename Range>
	Range_Reporter<Range> rangeReporter(
		const Range& range)
	{
		return Range_Reporter<Range>(range);
	}

}

#endif
