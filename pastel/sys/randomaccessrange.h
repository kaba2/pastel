// Description: RandomAccessRange class
// Detail: Random-access iterator range
// Documentation: iteratorrange.txt

#ifndef PASTEL_RANDOMACCESSRANGE_H
#define PASTEL_RANDOMACCESSRANGE_H

#include "pastel/sys/bidirectionalrange.h"

namespace Pastel
{

	template <typename Iterator>
	class RandomAccessRange
		: public BidirectionalRange<Iterator>
	{
	private:
		typedef BidirectionalRange<Iterator> Base;

	public:
		typedef typename Base::reference 
			reference;
		typedef typename Base::size_type 
			size_type;

		RandomAccessRange()
			: Base()
		{
		}

		RandomAccessRange(
			const Iterator& begin,
			const Iterator& end,
			integer size)
			: Base(begin, end, size)
		{
		}

		RandomAccessRange(
			const Iterator& begin,
			const Iterator& end)
			: Base(begin, end)
		{
		}

		RandomAccessRange(
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
