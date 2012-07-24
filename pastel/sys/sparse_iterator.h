// Description: SparseIterator class
// Detail: An iterator adapter for a larger step size.
// Documentation: iterators.txt

#ifndef PASTEL_SPARSE_ITERATOR_H
#define PASTEL_SPARSE_ITERATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/iterator_range.h"

#include <boost/operators.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <iterator>

namespace Pastel
{

	namespace Detail_SparseIterator
	{

		template <typename Derived, typename Iterator>
		class SparseIteratorHelperBase
			: public boost::random_access_iterator_helper<
			Derived, 
			typename std::iterator_traits<Iterator>::value_type,
			typename std::iterator_traits<Iterator>::difference_type,
			typename std::iterator_traits<Iterator>::pointer,
			typename std::iterator_traits<Iterator>::reference>
		{
		};

		template <typename Derived, typename Iterator>
		class SparseIteratorBase
			: public Detail_SparseIterator::SparseIteratorHelperBase<Derived, Iterator>
		{
		private:
			typedef Detail_SparseIterator::SparseIteratorHelperBase<Derived, Iterator>
				Base;

		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			SparseIteratorBase()
				: iter_()
				, delta_(0)
			{
			}

			explicit SparseIteratorBase(
				const Iterator& iter,
				typename Base::difference_type delta = 1)
				: iter_(iter)
				, delta_(delta)
			{
			}

			Derived& operator++()
			{
				iter_ += delta_;
				return (Derived&)*this;
			}
			
			Derived& operator--()
			{
				iter_ -= delta_;
				return (Derived&)*this;
			}

			Derived& operator+=(
				typename Base::difference_type that)
			{
				iter_ += delta_ * that;
				return (Derived&)*this;
			}

			Derived& operator-=(
				typename Base::difference_type that)
			{
				iter_ -= delta_ * that;
				return (Derived&)*this;
			}

			friend typename Base::difference_type operator-(
				const Derived& left, const Derived& right)
			{
				PENSURE_OP(left.delta_, ==, right.delta_);

				return (left.iter_ - right.iter_) / left.delta_;
			}

			typename Base::reference operator*() const
			{
				return *iter_;
			}

			friend bool operator==(
				const Derived& left, const Derived& right)
			{
				return left.iter_ == right.iter_;
			}

			friend bool operator<(
				const Derived& left, const Derived& right)
			{
				return left.iter_ < right.iter_;
			}

		protected:
			Iterator iter_;
			typename Base::difference_type delta_;
		};

	}

	template <typename ConstIterator>
	class ConstSparseIterator;

	template <typename Iterator>
	class SparseIterator
		: public Detail_SparseIterator::SparseIteratorBase<SparseIterator<Iterator>, Iterator>
	{
	private:
		typedef Detail_SparseIterator::SparseIteratorBase<SparseIterator<Iterator>, Iterator>
			Base;

		template <typename ConstIterator>
		friend class ConstSparseIterator;

	public:
		SparseIterator()
			: Base()
		{
		}

		explicit SparseIterator(
			const Iterator& iter,
			typename Base::difference_type delta = 1)
			: Base(iter, delta)
		{
		}
	};

	template <typename ConstIterator>
	class ConstSparseIterator
		: public Detail_SparseIterator::SparseIteratorBase<ConstSparseIterator<ConstIterator>, ConstIterator>
	{
	private:
		typedef Detail_SparseIterator::SparseIteratorBase<ConstSparseIterator<ConstIterator>, ConstIterator>
			Base;

	public:
		ConstSparseIterator()
			: Base()
		{
		}

		template <typename Iterator>
		ConstSparseIterator(
			const SparseIterator<Iterator>& that)
			: Base(that.iter_, that.delta_)
		{
		}

		explicit ConstSparseIterator(
			const ConstIterator& iter,
			integer delta = 1)
			: Base(iter, delta)
		{
		}
	};

	template <typename Iterator>
	SparseIterator<Iterator> sparseIterator(
		const Iterator& that, 
		typename std::iterator_traits<Iterator>::difference_type delta)
	{
		PENSURE_OP(delta, >=, 1);

		return SparseIterator<Iterator>(that, delta);
	}

	template <typename ConstIterator>
	ConstSparseIterator<ConstIterator> constSparseIterator(
		const ConstIterator& that, 
		typename std::iterator_traits<ConstIterator>::difference_type delta)
	{
		PENSURE_OP(delta, >=, 1);

		return ConstSparseIterator<ConstIterator>(that, delta);
	}

	template <typename Iterator>
	SparseIterator<Iterator> sparseEnd(
		const Iterator& begin, const Iterator& end,
		typename std::iterator_traits<Iterator>::difference_type delta)
	{
		PENSURE_OP(delta, >= , 1);

		const integer elements = end - begin;
		const integer remainder = elements % delta;

		Iterator result = end;
		if (remainder > 0)
		{
			result += delta - remainder;
		}
		
		return SparseIterator<Iterator>(result, delta);
	}

	template <typename ConstIterator>
	ConstSparseIterator<ConstIterator> constSparseEnd(
		const ConstIterator& begin, const ConstIterator& end,
		typename std::iterator_traits<ConstIterator>::difference_type delta)
	{
		PENSURE_OP(delta, >=, 1);

		return sparseEnd(begin, end, delta);
	}

	template <typename Iterator>
	RandomAccessIterator_Range<SparseIterator<Iterator> > sparseRange(
		const Iterator& begin, const Iterator& end,
		typename std::iterator_traits<Iterator>::difference_type delta)
	{
		PENSURE_OP(delta, >=, 1);

		return Pastel::range(
			sparseIterator(begin, delta),
			sparseEnd(begin, end, delta));
	}

	template <typename ConstIterator>
	RandomAccessIterator_Range<ConstSparseIterator<ConstIterator> > constSparseRange(
		const ConstIterator& begin, const ConstIterator& end,
		typename std::iterator_traits<ConstIterator>::difference_type delta)
	{
		PENSURE_OP(delta, >=, 1);

		return Pastel::range(
			constSparseIterator(begin, delta),
			constSparseEnd(begin, end, delta));
	}

}

#endif
