// Description: RandomAccessIterator_Range class
// Detail: Random-access iterator range
// Documentation: iterator_range.txt

#ifndef PASTEL_RANDOMACCESSITERATOR_RANGE_H
#define PASTEL_RANDOMACCESSITERATOR_RANGE_H

#include "pastel/sys/bidirectionaliterator_range.h"
#include "pastel/sys/randomaccessrange_concept.h"

namespace Pastel
{

	template <typename Iterator>
	class RandomAccessIterator_Range
		: public BidirectionalIterator_Range<Iterator>
	{
	private:
		typedef BidirectionalIterator_Range<Iterator>
			Base;

	public:
		typedef typename Base::reference 
			reference;
		typedef typename Base::size_type 
			size_type;

		RandomAccessIterator_Range()
			: Base()
		{
		}

		RandomAccessIterator_Range(
			const Iterator& begin,
			const Iterator& end,
			integer size)
			: Base(begin, end, size)
		{
		}

		RandomAccessIterator_Range(
			const Iterator& begin,
			const Iterator& end)
			: Base(begin, end)
		{
		}

		RandomAccessIterator_Range(
			const Iterator& begin,
			integer size)
			: Base(begin, size)
		{
		}

		reference operator[](size_type index) const
		{
			return *(Base::begin() + index);
		}
	};

}

#endif
