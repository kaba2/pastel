// Description: ForwardRange class
// Detail: Forward iterator range
// Documentation: iteratorrange.txt

#ifndef PASTEL_FORWARDRANGE_H
#define PASTEL_FORWARDRANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <boost/operators.hpp>

#include <algorithm>
#include <iterator>

namespace Pastel
{

	template <typename Iterator>
	class ForwardRange
		: boost::equality_comparable<ForwardRange<Iterator> >
	{
	protected:
		enum
		{
			Unknown = -1
		};

	public:
		typedef typename std::iterator_traits<Iterator>::value_type value_type;
		typedef integer size_type;
		typedef typename std::iterator_traits<Iterator>::pointer pointer;
		typedef typename std::iterator_traits<Iterator>::reference reference;
		typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
		typedef Iterator iterator;
		
		ForwardRange()
			: begin_()
			, end_()
			, size_(0)
		{
		}

		ForwardRange(
			const Iterator& begin,
			const Iterator& end,
			integer size)
			: begin_(begin)
			, end_(end)
			, size_(size)
		{
			PENSURE_OP(size, >=, 0);
			PENSURE((size == 0) == (begin == end));
		}

		ForwardRange(
			const Iterator& begin,
			const Iterator& end)
			: begin_(begin)
			, end_(end)
			, size_(Unknown)
		{
		}

		ForwardRange(
			const Iterator& begin,
			integer size)
			: begin_(begin)
			, end_(begin)
			, size_(size)
		{
		}

		void swap(ForwardRange& that)
		{
			using std::swap;
			swap(begin_, that.begin_);
			swap(end_, that.end_);
			swap(size_, that.size_);
		}

		void updateCache() const
		{
			end();
			size();
		}

		void clear()
		{
			begin_ = Iterator();
			end_ = Iterator();
			size_ = 0;
		}

		bool operator==(const ForwardRange& that) const
		{
			if (begin_ == that.begin_)
			{
				if (size_ == Unknown && that.size_ == Unknown)
				{
					return end_ == that.end_;
				}
				
				// Accessing via function to trigger evaluation.
				return size() == that.size();
			}

			return false;
		}

		bool empty() const
		{
			return !(size_ > 0 || begin_ != end_);
		}

		void pop_front()
		{
			PENSURE(!empty());
			++begin_;
			if (size_ != Unknown)
			{
				--size_;
			}
		}

		reference front() const
		{
			return *begin_;
		}

		const Iterator& begin() const
		{
			return begin_;
		}

		const Iterator& end() const
		{
			if (end_ == begin_ && size_ > 0)
			{
				end_ = begin_;
				// Update cache.
				std::advance(end_, size_);
			}

			return end_;
		}

		integer size() const
		{
			if (size_ == Unknown)
			{
				// Update cache.
				size_ = std::distance(begin_, end_);
			}

			return size_;
		}

	protected:
		Iterator begin_;
		mutable Iterator end_;
		mutable integer size_;
	};

	template <typename Iterator>
	ForwardRange<Iterator> forwardRange(
		const Iterator& begin)
	{
		return ForwardRange<Iterator>(begin, 1);
	}

	template <typename Iterator>
	ForwardRange<Iterator> forwardRange(
		const Iterator& begin, const Iterator& end)
	{
		return ForwardRange<Iterator>(begin, end);
	}

	template <typename Iterator>
	ForwardRange<Iterator> forwardRange(
		const Iterator& begin, integer size)
	{
		return ForwardRange<Iterator>(begin, size);
	}

	template <typename Iterator>
	ForwardRange<Iterator> forwardRange(
		const Iterator& begin, const Iterator& end, 
		integer size)
	{
		return ForwardRange<Iterator>(begin, end, size);
	}

	template <typename Type, int N>
	ForwardRange<Type*> forwardRange(
		Type (&begin)[N])
	{
		return ForwardRange<Type*>(begin, N);
	}

}

#endif
