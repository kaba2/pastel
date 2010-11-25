// Description: BidirectionalRange class
// Detail: Bidirectional iterator range
// Documentation: iteratorrange.txt

#ifndef PASTEL_BIDIRECTIONALRANGE_H
#define PASTEL_BIDIRECTIONALRANGE_H

#include "pastel/sys/forwardrange.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Iterator>
	class BidirectionalRange
		: public ForwardRange<Iterator>
	{
	private:
		typedef ForwardRange<Iterator> Base;

	public:
		typedef std::reverse_iterator<Iterator> reverse_iterator;

		BidirectionalRange()
			: Base()
		{
		}

		BidirectionalRange(
			const Iterator& begin,
			const Iterator& end,
			integer size)
			: Base(begin, end, size)
		{
		}

		BidirectionalRange(
			const Iterator& begin,
			const Iterator& end)
			: Base(begin, end)
		{
		}

		BidirectionalRange(
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

	template <typename Iterator>
	BidirectionalRange<Iterator> bidirectionalRange(
		const Iterator& begin)
	{
		return BidirectionalRange<Iterator>(begin, 1);
	}

	template <typename Iterator>
	BidirectionalRange<Iterator> bidirectionalRange(
		const Iterator& begin, const Iterator& end)
	{
		return BidirectionalRange<Iterator>(begin, end);
	}

	template <typename Iterator>
	BidirectionalRange<Iterator> bidirectionalRange(
		const Iterator& begin, integer size)
	{
		return BidirectionalRange<Iterator>(begin, size);
	}

	template <typename Iterator>
	BidirectionalRange<Iterator> bidirectionalRange(
		const Iterator& begin, const Iterator& end, 
		integer size)
	{
		return BidirectionalRange<Iterator>(begin, end, size);
	}

	template <typename Type, int N>
	BidirectionalRange<Type*> bidirectionalRange(
		Type (&begin)[N])
	{
		return BidirectionalRange<Type*>(begin, N);
	}

}

#endif
