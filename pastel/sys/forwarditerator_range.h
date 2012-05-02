// Description: ForwardIterator_Range class
// Detail: Forward iterator range
// Documentation: iterator_range.txt

#ifndef PASTEL_FORWARDITERATOR_RANGE_H
#define PASTEL_FORWARDITERATOR_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/forwardrange_concept.h"

#include <boost/operators.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/type_traits/is_same.hpp>

#include <algorithm>
#include <iterator>

namespace Pastel
{

	template <typename Iterator>
	class ForwardIterator_Range
		: public boost::equality_comparable<ForwardIterator_Range<Iterator> >
	{
	public:
		typedef typename std::iterator_traits<Iterator>::value_type value_type;
		typedef integer size_type;
		typedef typename std::iterator_traits<Iterator>::pointer pointer;
		typedef typename std::iterator_traits<Iterator>::reference reference;
		typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
		typedef Iterator iterator;
		enum
		{
			RandomAccess = boost::is_same<
			boost::iterator_category<Iterator>, 
			std::random_access_iterator_tag>::value
		};

		enum
		{
			Bidirectional = boost::is_same<
			boost::iterator_category<Iterator>, 
			std::bidirectional_iterator_tag>::value || RandomAccess
		};
	
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
			, size_(std::distance(begin, end))
		{
		}

		ForwardIterator_Range(
			const Iterator& begin,
			integer size)
			: begin_(begin)
			, end_(begin)
			, size_(size)
		{
			std::advance(end_, size);
		}

		void swap(ForwardIterator_Range& that)
		{
			using std::swap;
			swap(begin_, that.begin_);
			swap(end_, that.end_);
			swap(size_, that.size_);
		}

		void clear()
		{
			begin_ = end_;
			size_ = 0;
		}

		bool operator==(const ForwardIterator_Range& that) const
		{
			return begin_ == that.begin_ &&
				size_ == that.size_;
		}

		bool empty() const
		{
			return size_ == 0;
		}

		void pop_front()
		{
			PENSURE(!empty());
			++begin_;
			--size_;
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
			return end_;
		}

		integer size() const
		{
			return size_;
		}

	protected:
		Iterator begin_;
		Iterator end_;
		integer size_;
	};

}

#endif
