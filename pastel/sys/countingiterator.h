#ifndef PASTEL_COUNTINGITERATOR_H
#define PASTEL_COUNTINGITERATOR_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

#include <iterator>

namespace Pastel
{

	template <typename Type>
	class CountingIterator
		: public boost::random_access_iterator_helper<
		CountingIterator<Type>, Type, integer, const Type*, const Type&>
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

		const Type& operator*() const
		{
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

}

#endif
