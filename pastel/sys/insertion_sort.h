// Description: Insertion sort
// Documentation: sequence_algorithms.txt

#ifndef PASTELSYS_INSERTION_SORT_H
#define PASTELSYS_INSERTION_SORT_H

namespace Pastel
{

	template <typename BidiIterator>
	void insertionSort(BidiIterator begin, BidiIterator end);

	template <typename BidiIterator, typename Compare>
	void insertionSort(BidiIterator begin, BidiIterator end,
		Compare compare);

}

#include "pastel/sys/insertion_sort.hpp"

#endif
