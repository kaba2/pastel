// Description: ConstantIterator class
// Detail: A constant value iterator 
// Documentation: iterators.txt

#ifndef PASTEL_CONSTANT_ITERATOR_H
#define PASTEL_CONSTANT_ITERATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/iterator_range.h"

#include <boost/operators.hpp>

#include <iterator>

namespace Pastel
{

	template <typename Type>
	class ConstantIterator
		: public boost::random_access_iterator_helper<
		ConstantIterator<Type>, Type, integer, const Type*, const Type>
	{
	public:
		// Note the reference is const Type and not const Type&.
		// You can't return a reference since a temporary 
		// iterator can get destructed before the data
		// is accessed.

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		ConstantIterator()
			: index_(0)
			, data_()
		{
		}

		explicit ConstantIterator(const Type& data, integer index = 0)
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

		//const Type& operator*() const
		Type operator*() const
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

	template <typename Type>
	ConstantIterator<Type> constantIterator(
		const Type& that, integer index = 0)
	{
		return ConstantIterator<Type>(that, index);
	}

	template <typename Type>
	RandomAccessIterator_Range<ConstantIterator<Type> > constantRange(
		const Type& that, integer size = 1)
	{
		return Pastel::range(constantIterator(that), size);
	}

}

#endif
