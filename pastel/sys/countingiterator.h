// Description: CountingIterator class
// Detail: An iterator adapter for dereferencing the adapted iterator
// Documentation: iterators.txt

#ifndef PASTEL_COUNTINGITERATOR_H
#define PASTEL_COUNTINGITERATOR_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <iterator>

namespace Pastel
{
	template <typename Type>
	class CountingIterator;

	namespace Detail_CountingIterator
	{

		template <typename Type_, bool IsArithmetic>
		class Base
		{
		public:
			typedef boost::random_access_iterator_helper<
				CountingIterator<Type_>, Type_, integer, const Type_, const Type_>
				Type;
		};

		template <typename Type_>
		class Base<Type_, false>
		{
		public:
			typedef boost::random_access_iterator_helper<
				CountingIterator<Type_>, Type_, integer, const Type_, const Type_> 
				RandomAccess_Base;

			typedef boost::bidirectional_iterator_helper<
				CountingIterator<Type_>, Type_, integer, const Type_, const Type_>
				Bidirectional_Base;

			typedef boost::forward_iterator_helper<
				CountingIterator<Type_>, Type_, integer, const Type_, const Type_>
				Forward_Base;

			typedef boost::is_same<typename boost::iterator_category<Type_>::type,
				std::random_access_iterator_tag> IsRandomAccess;

			typedef boost::is_same<typename boost::iterator_category<Type_>::type,
				std::bidirectional_iterator_tag> IsBidirectional;

			typedef typename
				boost::mpl::if_<
				IsRandomAccess,
				RandomAccess_Base,
				typename boost::mpl::if_<
				IsBidirectional,
				Bidirectional_Base, Forward_Base>::type>::type Type;
		};

	}

	template <typename Type>
	class CountingIterator
		: public Detail_CountingIterator::Base<Type, boost::is_arithmetic<Type>::value>::Type
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		CountingIterator()
			: data_()
		{
		}

		explicit CountingIterator(const Type& data)
			: data_(data)
		{
		}

		CountingIterator& operator++()
		{
			++data_;
			return *this;
		}
		
		CountingIterator& operator--()
		{
			--data_;
			return *this;
		}

		CountingIterator& operator+=(integer that)
		{
			data_ += that;
			return *this;
		}

		CountingIterator& operator-=(integer that)
		{
			data_ -= that;
			return *this;
		}

		integer operator-(const CountingIterator& that) const
		{
			return data_ - that.data_;
		}

		Type operator*() const
		{
			// Note: this function must return 'data_' by value,
			// for otherwise *(iter + n) would refer to an already
			// destructed temporary.
			return data_;
		}

		bool operator==(const CountingIterator& that) const
		{
			return data_ == that.data_;
		}

		bool operator<(const CountingIterator& that) const
		{
			return data_ < that.data_;
		}

	private:
		Type data_;
	};

	template <typename Type>
	CountingIterator<Type> countingIterator(const Type& that)
	{
		return CountingIterator<Type>(that);
	}

}

#endif
