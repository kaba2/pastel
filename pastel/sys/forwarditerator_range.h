// Description: ForwardIterator_Range class
// Detail: Forward iterator range
// Documentation: iterator_range.txt

#ifndef PASTEL_FORWARDITERATOR_RANGE_H
#define PASTEL_FORWARDITERATOR_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/forwardrange_concept.h"

#include <boost/operators.hpp>

#include <algorithm>
#include <iterator>

namespace Pastel
{

	template <typename Iterator>
	class ForwardIterator_Range
		: public boost::equality_comparable<ForwardIterator_Range<Iterator> >
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
		
		ForwardIterator_Range()
			: begin_()
			, end_()
			, size_(0)
		{
		}

		ForwardIterator_Range(
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

		ForwardIterator_Range(
			const Iterator& begin,
			const Iterator& end)
			: begin_(begin)
			, end_(end)
			, size_(Unknown)
		{
		}

		ForwardIterator_Range(
			const Iterator& begin,
			integer size)
			: begin_(begin)
			, end_(begin)
			, size_(size)
		{
		}

		void swap(ForwardIterator_Range& that)
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

		bool operator==(const ForwardIterator_Range& that) const
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

}

#endif
