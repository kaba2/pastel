#ifndef PASTELSYS_INTERVAL_SEQUENCE_HPP
#define PASTELSYS_INTERVAL_SEQUENCE_HPP

#include "pastel/sys/interval_sequence.h"
#include "pastel/sys/number_tests.h"

namespace Pastel
{

	template <typename Integer, typename Reporter>
	void difference(
		const std::vector<Integer>& aSet,
		const std::vector<Integer>& bSet,
		Reporter report)
	{
		PENSURE(isIntervalSequence(aSet));
		PENSURE(isIntervalSequence(bSet));

		if (aSet.empty())
		{
			// Since A is empty, there is nothing to report.
			return;
		}

		const integer n = aSet.size();
		const integer m = bSet.size();

		integer i = 0;
		integer j = 0;
		integer t = aSet[i];
		while(i < n)
		{
			// Skip over those intervals in A which
			// have already been reported.
			while(i < n && aSet[i + 1] <= t)
			{
				i += 2;
			}

			if (i == n)
			{
				// No more intervals in A left.
				// There is nothing more to report.
				break;
			}

			if (t < aSet[i])
			{
				t = aSet[i];
			}

			ASSERT_OP(aSet[i], <=, t)
			ASSERT_OP(t, <, aSet[i + 1]);

			// Skip over those intervals in B
			// which precede the current 
			// time position t.
			while(j < m && bSet[j + 1] <= t)
			{
				j += 2;
			}

			if (j == m)
			{
				// No more intervals in B left.
				// We may simply report all the
				// intervals left in A.
				break;
			}

			bool bActive = (bSet[j] <= t);
			ASSERT_OP(t, <, bSet[j + 1]);
			
			integer tNext = t;
			if (bActive)
			{
				// Both interval A and interval B are active.
				// Skip over; no reporting.
				if (aSet[i + 1] < bSet[j + 1])
				{
					// Interval A ends first.
					tNext = aSet[i + 1];
				}
				else
				{
					// Interval B ends first.
					tNext = bSet[j + 1];
				}
			}
			else
			{
				// Interval A is active, interval B is not active.
				// Report a fragment of interval A.
				tNext = std::min(aSet[i + 1], bSet[j]);

				report(std::make_pair(t, tNext));
			}

			t = tNext;
		}

		if (i < n && t < aSet[i + 1])
		{
			// Report the remaining fragment.
			integer tNext = aSet[i + 1];
			report(std::make_pair(t, tNext));
			i += 2;
		}

		while (i < n)
		{
			// Report every non-empty interval left in A.
			if (aSet[i] < aSet[i + 1])
			{
				report(std::make_pair(aSet[i], aSet[i + 1]));
			}
			i += 2;
		}
	}

	template <typename Integer>
	bool isIntervalSequence(
		const std::vector<Integer>& aSet)
	{
		if (!even(aSet.size()))
		{
			return false;
		}

		const integer n = aSet.size();
		for (integer i = 0;i < n - 1;++i)
		{
			if (aSet[i] > aSet[i + 1])
			{
				return false;
			}
		}

		return true;
	}

}

#endif
