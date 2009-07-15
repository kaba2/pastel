// Description: SparseIterator class
// Detail: A random-access iterator adapter for a larger step size.
// Documentation: miscellaneous.txt

#ifndef PASTEL_SPARSEITERATOR_H
#define PASTEL_SPARSEITERATOR_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

#include <iterator>

namespace Pastel
{

	template <typename Iterator>
	class SparseIterator
		: public boost::random_access_iterator_helper<
		SparseIterator<Iterator>, 
		typename std::iterator_traits<Iterator>::value_type,
		typename std::iterator_traits<Iterator>::difference_type,
		typename std::iterator_traits<Iterator>::pointer,
		typename std::iterator_traits<Iterator>::reference>
	{
	public:
		template <typename ConstIterator>
		friend class ConstSparseIterator;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		SparseIterator()
			: iter_()
			, delta_(0)
		{
		}

		SparseIterator(
			const Iterator& iter,
			integer delta = 1)
			: iter_(iter)
			, delta_(delta)
		{
		}

		SparseIterator& operator++()
		{
			iter_ += delta_;
			return *this;
		}
		
		SparseIterator& operator--()
		{
			iter_ -= delta_;
			return *this;
		}

		SparseIterator& operator+=(
			integer that)
		{
			iter_ += delta_ * that;
			return *this;
		}

		SparseIterator& operator-=(
			integer that)
		{
			iter_ -= delta_ * that;
			return *this;
		}

		typename std::iterator_traits<Iterator>::value_type& operator*() const
		{
			return *iter_;
		}

		bool operator==(const SparseIterator& that) const
		{
			return iter_ == that.iter_;
		}

		bool operator<(const SparseIterator& that) const
		{
			return iter_ < that.iter_;
		}

	private:
		Iterator iter_;
		integer delta_;
	};

	template <typename ConstIterator>
	class ConstSparseIterator
		: public boost::random_access_iterator_helper<
		ConstSparseIterator<ConstIterator>, 
		typename std::iterator_traits<ConstIterator>::value_type,
		typename std::iterator_traits<ConstIterator>::difference_type,
		typename std::iterator_traits<ConstIterator>::pointer,
		typename std::iterator_traits<ConstIterator>::reference>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		ConstSparseIterator()
			: iter_()
			, delta_(0)
		{
		}

		explicit ConstSparseIterator(
			const ConstIterator& iter,
			integer delta = 1)
			: iter_(iter)
			, delta_(delta)
		{
		}

		template <typename Iterator>
		ConstSparseIterator(
			const SparseIterator<Iterator>& that,
			integer delta = 1)
			: iter_(that.iter_)
			, delta_(delta)
		{
		}

		ConstSparseIterator& operator++()
		{
			iter_ += delta_;
			return *this;
		}
		
		ConstSparseIterator& operator--()
		{
			iter_ -= delta_;
			return *this;
		}
		
		ConstSparseIterator& operator+=(
			integer that)
		{
			iter_ += delta_ * that;
			return *this;
		}

		ConstSparseIterator& operator-=(
			integer that)
		{
			iter_ -= delta_ * that;
			return *this;
		}

		const typename std::iterator_traits<ConstIterator>::value_type& operator*() const
		{
			return *iter_;
		}

		bool operator==(const ConstSparseIterator& that) const
		{
			return iter_ == that.iter_;
		}

		bool operator<(const ConstSparseIterator& that) const
		{
			return iter_ < that.iter_;
		}

	private:
		ConstIterator iter_;
		integer delta_;
	};

}

#endif
