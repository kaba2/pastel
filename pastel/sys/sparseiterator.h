#ifndef PASTEL_SPARSEITERATOR_H
#define PASTEL_SPARSEITERATOR_H

#include "pastel/sys/mytypes.h"

#include <iterator>

namespace Pastel
{

		template <typename Type>
		class ConstSparseIterator
			: public std::iterator<std::random_access_iterator_tag, Type> 
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			ConstSparseIterator()
				: data_(0)
				, delta_(0)
			{
			}

			ConstSparseIterator(
				const Type* data,
				integer delta)
				: data_(data)
				, delta_(delta)
			{
			}

			ConstSparseIterator& operator++()
			{
				data_ += delta_;
				return *this;
			}
			
			ConstSparseIterator operator++(int)
			{
				ConstSparseIterator result = *this;
				data_ += delta_;
				return result;
			}

			ConstSparseIterator& operator--()
			{
				data_ -= delta_;
				return *this;
			}
			
			ConstSparseIterator operator--(int)
			{
				ConstSparseIterator result = *this;
				data_ -= delta_;
				return result;
			}

			ConstSparseIterator& operator+=(
				integer that)
			{
				data_ += delta_ * that;
				return *this;
			}

			ConstSparseIterator operator+(
				integer that) const
			{
				ConstSparseIterator copy(*this);
				copy += that;
				return copy;
			}

			ConstSparseIterator& operator-=(
				integer that)
			{
				data_ -= delta_ * that;
				return *this;
			}

			ConstSparseIterator operator-(
				integer that) const
			{
				ConstSparseIterator copy(*this);
				copy -= that;
				return copy;
			}

			const Type& operator*() const
			{
				return *data_;
			}

			bool operator==(const ConstSparseIterator& that) const
			{
				return data_ == that.data_;
			}

			bool operator!=(const ConstSparseIterator& that) const
			{
				return data_ != that.data_;
			}

			// Need to be protected instead
		// of private for SparseIterator
		// to access data_.
		protected:
			const Type* data_;
			integer delta_;
		};

		template <typename Type>
		class SparseIterator
			: public ConstSparseIterator<Type>
		{
		private:
			typedef ConstSparseIterator<Type> Base;

		public:
			SparseIterator()
				: Base()
			{
			}

			SparseIterator(
				Type* data,
				integer delta)
				: Base(data, delta)
			{
			}

			SparseIterator& operator++()
			{
				Base::operator++();
				return *this;
			}
			
			SparseIterator operator++(int that)
			{
				SparseIterator copy(*this);
				Base::operator++(that);
				return copy;
			}

			SparseIterator& operator--()
			{
				Base::operator--();
				return *this;
			}
			
			SparseIterator operator--(int that)
			{
				SparseIterator copy(*this);
				Base::operator--(that);
				return copy;
			}

			SparseIterator& operator+=(
				integer that)
			{
				Base::operator+=(that);
				return *this;
			}

			SparseIterator operator+(integer that) const
			{
				return SparseIterator(Base::operator+(that));
			}

			SparseIterator& operator-=(
				integer that)
			{
				Base::operator-=(that);
				return *this;
			}

			SparseIterator operator-(integer that) const
			{
				return SparseIterator(Base::operator-(that));
			}

			Type& operator*()
			{
				return (Type&)Base::operator*();
			}
		
			bool operator==(const SparseIterator& that) const
			{
				return Base::operator==(that);
			}

			bool operator!=(const SparseIterator& that) const
			{
				return Base::operator!=(that);
			}

		private:
			SparseIterator(
				const ConstSparseIterator<Type>& that)
				: Base(that)
			{
			}
		};

}

#endif
