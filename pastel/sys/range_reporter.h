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

		template <typename Type>
		void operator()(const Type& data) const
		{
			if (!range_.empty())
			{
				range_.front() = data;
				range_.pop_front();
			}
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
