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

		typename Base::reference 
			operator[](Base::size_type index) const
		{
			return *(Base::begin() + index);
		}
	};

	template <typename Iterator>
	RandomAccessRange<Iterator> randomAccessRange(
		const Iterator& begin)
	{
		return RandomAccessRange<Iterator>(begin, 1);
	}

	template <typename Iterator>
	RandomAccessRange<Iterator> randomAccessRange(
		const Iterator& begin, const Iterator& end)
	{
		return RandomAccessRange<Iterator>(begin, end);
	}

	template <typename Iterator>
	RandomAccessRange<Iterator> randomAccessRange(
		const Iterator& begin, integer size)
	{
		return RandomAccessRange<Iterator>(begin, size);
	}

	template <typename Iterator>
	RandomAccessRange<Iterator> randomAccessRange(
		const Iterator& begin, const Iterator& end, 
		integer size)
	{
		return RandomAccessRange<Iterator>(begin, end, size);
	}

	template <typename Type, int N>
	RandomAccessRange<Type*> randomAccessRange(
		Type (&begin)[N])
	{
		return RandomAccessRange<Type*>(begin, N);
	}

}

#endif
