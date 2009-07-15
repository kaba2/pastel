#ifndef PASTEL_COUNTINGITERATOR_H
#define PASTEL_COUNTINGITERATOR_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

#include <iterator>

namespace Pastel
{

	template <typename Type>
	class ConstCountingIterator
		: public boost::random_access_iterator_helper<
		ConstCountingIterator<Type>, Type, integer>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		ConstCountingIterator()
			: data_()
		{
		}

		explicit ConstCountingIterator(const Type& data)
			: data_(data)
		{
		}

		ConstCountingIterator& operator++()
		{
			++data_;
			return *this;
		}
		
		ConstCountingIterator& operator--()
		{
			--data_;
			return *this;
		}

		ConstCountingIterator& operator+=(integer that)
		{
			data_ += that;
			return *this;
		}

		ConstCountingIterator& operator-=(integer that)
		{
			data_ -= that;
			return *this;
		}

		Type operator*() const
		{
			return data_;
		}

		bool operator==(const ConstCountingIterator& that) const
		{
			return data_ == that.data_;
		}

		bool operator<(const ConstCountingIterator& that) const
		{
			return data_ < that.data_;
		}

	private:
		Type data_;
	};

}

#endif
