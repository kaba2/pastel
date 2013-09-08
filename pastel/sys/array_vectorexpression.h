// Description: A vector expression for a native array.
// Documentation: vectorexpression.txt

#ifndef PASTELSYS_ARRAY_VECTOREXPRESSION_H
#define PASTELSYS_ARRAY_VECTOREXPRESSION_H

#include "pastel/sys/vectorexpression.h"
#include "pastel/sys/sparse_iterator.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, int N>
	class ConstArray_VectorExpression
		: public VectorExpression<Real, N, ConstArray_VectorExpression<Real, N> >
	{
	public:
		typedef const ConstArray_VectorExpression StorageType;

		typedef ConstSparseIterator<const Real*> ConstIterator;

		ConstArray_VectorExpression()
			: data_(0)
			, size_(0)
			, stride_(0)
		{
		}

		ConstArray_VectorExpression(
			const Real* data,
			integer size,
			integer stride = 1)
			: data_(data)
			, size_(size)
			, stride_(stride)
		{
			PENSURE_OP(size, >=, 0);
			PENSURE_OP(stride, >=, 0);
			PENSURE2(N == Dynamic || size == N, N, size);
		}

		integer size() const
		{
			return size_;
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				data_, data_ + size_ * stride_);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return false;
		}

		integer n() const
		{
			return size_;
		}

		void swap(ConstArray_VectorExpression<Real, N>& that)
		{
			std::swap(data_, that.data_);
			std::swap(size_, that.size_);
			std::swap(stride_, that.stride_);
		}

		bool operator==(const ConstArray_VectorExpression& that) const
		{
			return std::equal(begin(), end(), that.begin());
		}

		const Real& operator[](integer index) const
		{
			PENSURE2(index >= 0 && index < size_, 
				index, size_);

			return data_[index * stride_];
		}

		ConstIterator begin() const
		{
			return ConstIterator(data_, stride_);
		}

		ConstIterator end() const
		{
			return ConstIterator(data_ + size_ * stride_, stride_);
		}

	protected:
		// Must be protected for
		// Array_VectorExpression to access

		const Real* data_;
		integer size_;
		integer stride_;
	};

	template <int N, typename Real>
	ConstArray_VectorExpression<Real, N> 
		constVectorExpression(const Real* data, integer size, integer stride = 1)
	{
		return ConstArray_VectorExpression<Real, N>(data, size, stride);
	}

	template <typename Real, int N>
	ConstArray_VectorExpression<Real, N> 
		constVectorExpression(const Real (&data)[N])
	{
		return ConstArray_VectorExpression<Real, N>(data, N);
	}

	template <typename Real, int N>
	class Array_VectorExpression
		: public ConstArray_VectorExpression<Real, N>
	{
	private:
		typedef ConstArray_VectorExpression<Real, N> Base;
		using Base::data_;
		using Base::size_;
		using Base::stride_;

	public:
		typedef const Array_VectorExpression StorageType;

		typedef SparseIterator<Real*> Iterator;

		using Base::size;

		Array_VectorExpression()
			: Base()
		{
		}

		Array_VectorExpression(
			const Real* data,
			integer size,
			integer stride = 1)
			: Base(data, size, stride)
		{
			PENSURE_OP(size, >=, 0);
			PENSURE_OP(stride, >=, 0);
			PENSURE2(N == Dynamic || size == N, N, size);
		}

		// The parameter to this function
		// is deliberately not a reference,
		// because the reference could point
		// to this vector.
		void set(Real that) const
		{
			std::fill(begin(), end(), that);
		}

		Iterator begin() const
		{
			return Iterator((Real*)data_, stride_);
		}

		Iterator end() const
		{
			return Iterator((Real*)(data_ + size_ * stride_), stride_);
		}

		const Array_VectorExpression& operator=(Real that) const
		{
			// We accept basic exception safety for performance.
			set(that);

			return *this;
		}

		const Array_VectorExpression& operator=(
			const Array_VectorExpression& that) const
		{
			ENSURE_OP(size(), ==, that.size());

			std::copy(that.begin(), that.end(), begin());

			return *this;
		}

		template <typename ThatReal, int ThatN, typename Expression>
		const Array_VectorExpression& operator=(
			const VectorExpression<ThatReal, ThatN, Expression>& that) const
		{
			ENSURE_OP(size(), ==, that.size());

			if (that.evaluateBeforeAssignment(
				data_, data_ + size_))
			{
				// In the case we must reallocate, we can
				// as well copy construct, so that there
				// is no redundant initialization.
				
				// Of course, if the expression involves
				// this vector as a non-trivial subexpression,
				// we must copy construct anyway.

				*this = evaluate(that);
			}
			else
			{				
				// We accept basic exception safety for performance.

				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter = that[i];
					++iter;
				}
			}

			return *this;
		}

		Real& operator[](integer index) const
		{
			PENSURE2(index >= 0 && index < size_, 
				index, size_);

			return (Real&)data_[index * stride_];
		}

		// The parameter to this function
		// is deliberately not a reference,
		// because the reference could point
		// to this vector.
		const Array_VectorExpression& operator+=(const Real that) const
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				*iter += that;
				++iter;
			}

			return *this;
		}

		// The parameter to this function
		// is deliberately not a reference,
		// because the reference could point
		// to this vector.
		const Array_VectorExpression& operator-=(const Real that) const
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				*iter -= that;
				++iter;
			}

			return *this;
		}

		// The parameter to this function
		// is deliberately not a reference,
		// because the reference could point
		// to this vector.
		const Array_VectorExpression& operator*=(const Real that) const
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				*iter *= that;
				++iter;
			}

			return *this;
		}

		// Here the reference is ok because we actually
		// use the parameter's inverse.
		const Array_VectorExpression& operator/=(const Real& that) const
		{
			return (*this *= Pastel::inverse(that));
		}

		template <typename ThatReal, int ThatN, typename Expression>
		const Array_VectorExpression& operator+=(
			const VectorExpression<ThatReal, ThatN, Expression>& that) const
		{
			PENSURE2(that.size() == size(), that.size(), size());

			if (that.evaluateBeforeAssignment(
				data_, data_ + size_))
			{
				*this += Vector<Real, N>(that);
			}
			else
			{
				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter += that[i];
					++iter;
				}
			}

			return *this;
		}

		template <typename ThatReal, int ThatN, typename Expression>
		const Array_VectorExpression& operator-=(
			const VectorExpression<ThatReal, ThatN, Expression>& that) const
		{
			PENSURE2(that.size() == size(), that.size(), size());

			if (that.evaluateBeforeAssignment(
				data_, data_ + size_))
			{
				*this -= Vector<Real, N>(that);
			}
			else
			{
				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter -= that[i];
					++iter;
				}
			}

			return *this;
		}

		template <typename ThatReal, int ThatN, typename Expression>
		const Array_VectorExpression& operator*=(
			const VectorExpression<ThatReal, ThatN, Expression>& that) const
		{
			PENSURE2(that.size() == size(), that.size(), size());

			if (that.evaluateBeforeAssignment(
				data_, data_ + size_))
			{
				*this *= Vector<Real, N>(that);
			}
			else
			{
				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter *= that[i];
					++iter;
				}
			}

			return *this;
		}

		template <typename ThatReal, int ThatN, typename Expression>
		const Array_VectorExpression& operator/=(
			const VectorExpression<ThatReal, ThatN, Expression>& that) const
		{
			PENSURE2(that.size() == size(), that.size(), size());

			if (that.evaluateBeforeAssignment(
				data_, data_ + size_))
			{
				*this /= Vector<Real, N>(that);
			}
			else
			{
				Iterator iter = begin();
				const integer n = size();

				for (integer i = 0;i < n;++i)
				{
					*iter /= that[i];
					++iter;
				}
			}

			return *this;
		}
	};

	template <int N, typename Real>
	Array_VectorExpression<Real, N> 
		vectorExpression(Real* data, integer size)
	{
		return Array_VectorExpression<Real, N>(data, size);
	}

	template <typename Real, int N>
	Array_VectorExpression<Real, N> 
		vectorExpression(const Real (&data)[N])
	{
		return Array_VectorExpression<Real, N>(data, N);
	}

}

#endif
