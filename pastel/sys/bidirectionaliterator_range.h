// Description: BidirectionalIterator_Range class
// Detail: Bidirectional iterator range
// Documentation: iteratorrange.txt

#ifndef PASTEL_BIDIRECTIONALITERATOR_RANGE_H
#define PASTEL_BIDIRECTIONALITERATOR_RANGE_H

#include "pastel/sys/forwarditerator_range.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Iterator>
	class BidirectionalIterator_Range
		: public ForwardIterator_Range<Iterator>
	{
	private:
		typedef ForwardIterator_Range<Iterator> Base;

	public:
		typedef std::reverse_iterator<Iterator> 
			reverse_iterator;

		BidirectionalIterator_Range()
			: Base()
		{
		}

		BidirectionalIterator_Range(
			const Iterator& begin,
			const Iterator& end,
			integer size)
			: Base(begin, end, size)
		{
		}

		BidirectionalIterator_Range(
			const Iterator& begin,
			const Iterator& end)
			: Base(begin, end)
		{
		}

		BidirectionalIterator_Range(
			const Iterator& begin,
			integer size)
			: Base(begin, size)
		{
		}

		typename Base::reference back() const
		{
			PENSURE(!Base::empty());
			Iterator last = Base::end();
			--last;
			return *last;
		}

		void pop_back()
		{
			PENSURE(!Base::empty());
			--Base::end_;
			if (Base::size_ != Base::Unknown)
			{
				--Base::size_;
			}
		}

		reverse_iterator rbegin() const
		{
			return reverse_iterator(Base::end());
		}
			
		reverse_iterator rend() const
		{
			return reverse_iterator(Base::begin());
		}
	};

}

#endif
