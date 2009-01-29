#ifndef PASTELSYS_INSERTIONSORT_HPP
#define PASTELSYS_INSERTIONSORT_HPP

#include "pastel/sys/insertionsort.h"

#include <algorithm>

namespace Pastel
{

	template <typename BidiIterator, typename Compare>
	void insertionSort(BidiIterator begin, BidiIterator end,
		Compare compare)
	{
		if (begin == end)
		{
			return;
		}

		BidiIterator previous = begin;
		BidiIterator iter = previous;
		++iter;

		while(iter != end)
		{
			BidiIterator next = iter;
			++next;

			// If the current element is in the
			// wrong place, it is carried
			// there by subsequent swaps.
			BidiIterator left = previous;
			BidiIterator right = iter;
			while (!compare(*left, *right))
			{
				using NameLookUpTrick::swap;

				swap(*left, *right);
				if (left == begin)
				{
					break;
				}

				right = left;
				--left;
			}

			previous = iter;
			iter = next;
		}

	}

}

#endif
