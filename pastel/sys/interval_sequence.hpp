#ifndef PASTELSYS_INTERVAL_SEQUENCE_HPP
#define PASTELSYS_INTERVAL_SEQUENCE_HPP

#include "pastel/sys/interval_sequence.h"
#include "pastel/sys/math/number_tests.h"

namespace Pastel
{

	template <
		typename A_Range, 
		typename B_Range,
		typename Output>
	void difference(
		const A_Range& aSet,
		const B_Range& bSet,
		Output report)
	{
		PENSURE(isIntervalSequence(aSet));
		PENSURE(isIntervalSequence(bSet));

		auto a = boost::begin(aSet);
		auto aEnd = boost::end(aSet);

		if (a == aEnd)
		{
			// Since A is empty, there is nothing to report.
			return;
		}

		auto b = boost::begin(bSet);
		auto bEnd = boost::end(bSet);

		integer t = *a;
		while(a != aEnd)
		{
			// Skip over those intervals in A which
			// have already been reported.
			while(a != aEnd && *std::next(a) <= t)
			{				
				std::advance(a, 2);
			}

			if (a == aEnd)
			{
				// No more intervals in A left.
				// There is nothing more to report.
				break;
			}

			auto aNext = std::next(a);

			if (t < *a)
			{
				t = *a;
			}

			ASSERT_OP(*a, <=, t)
			ASSERT_OP(t, <, *aNext);

			// Skip over those intervals in B
			// which precede the current 
			// time position t.
			while(b != bEnd && *std::next(b) <= t)
			{
				std::advance(b, 2);
			}

			if (b == bEnd)
			{
				// No more intervals in B left.
				// We may simply report all the
				// intervals left in A.
				break;
			}

			auto bNext = std::next(b);

			bool bActive = (*b <= t);
			ASSERT_OP(t, <, *bNext);
			
			integer tNext = t;
			if (bActive)
			{
				// Both interval A and interval B are active.
				// Skip over; no reporting.
				if (*aNext < *bNext)
				{
					// Interval A ends first.
					tNext = *aNext;
				}
				else
				{
					// Interval B ends first.
					tNext = *bNext;
				}
			}
			else
			{
				// Interval A is active, interval B is not active.
				// Report a fragment of interval A.
				tNext = std::min(*aNext, *b);

				report(std::make_pair(t, tNext));
			}

			t = tNext;
		}

		if (a != aEnd && t < *std::next(a))
		{
			// Report a possible remaining fragment.
			report(std::make_pair(t, *std::next(a)));

			std::advance(a, 2);
		}

		while (a != aEnd)
		{
			// Report every non-empty interval left in A.
			auto aNext = std::next(a);
			if (*a  < *aNext)
			{
				report(std::make_pair(*a, *aNext));
			}

			std::advance(a, 2);
		}
	}

	template <typename A_Range>
	bool isIntervalSequence(
		const A_Range& aSet)
	{
		auto a = boost::begin(aSet);
		auto aEnd = boost::end(aSet);

		while(a != aEnd)
		{
			auto aNext = std::next(a);
			if (aNext == aEnd)
			{
				// The sequence has an odd number of elements.
				return false;
			}

			if (*a > *aNext)
			{
				// The sequence is not non-decreasing.
				return false;
			}

			std::advance(a, 2);
		}

		return true;
	}

}

#endif
