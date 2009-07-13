// Description: Insertion sort algorithm
// Documentation: miscellaneous.txt

#ifndef PASTEL_INSERTIONSORT_H
#define PASTEL_INSERTIONSORT_H

namespace Pastel
{

	template <typename BidiIterator, typename Compare>
	void insertionSort(BidiIterator begin, BidiIterator end,
		Compare compare);

}

#include "pastel/sys/insertionsort.hpp"

#endif
