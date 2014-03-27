// Description: Pairwise all-of

#ifndef PASTELSYS_PAIRWISE_ALL_OF
#define PASTELSYS_PAIRWISE_ALL_OF

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	template <
		typename ForwardIterator,
		typename Predicate>
	bool pairwiseAllOf(
		const boost::iterator_range<ForwardIterator>& range,
		const Predicate& predicate)
	{
		if (range.empty())
		{
			return true;
		}

		ForwardIterator iter = range.begin();
		ForwardIterator end = range.end();
		ForwardIterator next = std::next(iter);
		while(next != end)
		{
			if (!predicate(*iter, *next))
			{
				return false;
			}

			iter = next;
			++next;
		}

		return true;
	}

}

#endif
