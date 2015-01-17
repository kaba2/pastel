// Description: Insertion sort
// Documentation: sequence_algorithms.txt

#ifndef PASTELSYS_INSERTION_SORT_H
#define PASTELSYS_INSERTION_SORT_H

namespace Pastel
{

	template <typename BidiIterator>
	void insertionSort(BidiIterator begin, BidiIterator end);

	template <typename BidiIterator, typename Less>
	void insertionSort(BidiIterator begin, BidiIterator end,
		Less compare);

}

#include "pastel/sys/sequence/insertion_sort.hpp"

#endif
