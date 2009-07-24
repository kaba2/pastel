#ifndef PASTEL_CONSTANTITERATOR_H
#define PASTEL_CONSTANTITERATOR_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

#include <iterator>

namespace Pastel
{

	template <typename Type>
	class ConstantIterator
		: public boost::random_access_iterator_helper<
		ConstantIterator<Type>, Type, integer, const Type*, const Type&>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		ConstantIterator()
			: index_(0)
			, data_()
		{
		}

		explicit ConstantIterator(integer index, const Type& data)
			: index_(index)
			, data_(data)
		{
		}

		ConstantIterator& operator++()
		{
			++index_;
			return *this;
		}
		
		ConstantIterator& operator--()
		{
			--index_;
			return *this;
		}

		ConstantIterator& operator+=(integer that)
		{
			index_ += that;
			return *this;
		}

		ConstantIterator& operator-=(integer that)
		{
			index_ -= that;
			return *this;
		}

		integer operator-(const ConstantIterator& that) const
		{
			return index_ - that.index_;
		}

		const Type& operator*() const
		{
			return data_;
		}

		bool operator==(const ConstantIterator& that) const
		{
			return index_ == that.index_;
		}

		bool operator<(const ConstantIterator& that) const
		{
			return index_ < that.index_;
		}

	private:
		integer index_;
		Type data_;
	};

}

#endif
