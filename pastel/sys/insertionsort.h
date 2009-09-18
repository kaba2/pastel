// Description: Insertion sort
// Documentation: basic_algorithms.txt

#ifndef PASTEL_INSERTIONSORT_H
#define PASTEL_INSERTIONSORT_H

namespace Pastel
{

	template <typename BidiIterator>
	void insertionSort(BidiIterator begin, BidiIterator end);

	template <typename BidiIterator, typename Compare>
	void insertionSort(BidiIterator begin, BidiIterator end,
		Compare compare);

}

#include "pastel/sys/insertionsort.hpp"

#endif
