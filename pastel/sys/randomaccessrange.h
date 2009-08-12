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
		using Base::reference;
		using Base::const_reference;
		using Base::size_type;
		
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
			return *(begin() + index);
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

}

#endif
