#ifndef PASTELSYS_INSERTION_SORT_HPP
#define PASTELSYS_INSERTION_SORT_HPP

#include "pastel/sys/insertion_sort.h"

#include <algorithm>

namespace Pastel
{

	template <typename BidiIterator>
	void insertionSort(BidiIterator begin, BidiIterator end)
	{
		Pastel::insertionSort(begin, end, 
			std::less<typename BidiIterator::value_type>());
	}

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
				using std::swap;

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
