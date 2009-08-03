#ifndef PASTEL_SUBARRAY_ITERATOR_H
#define PASTEL_SUBARRAY_ITERATOR_H

#include "pastel/sys/subarray.h"

namespace Pastel
{

	template <int N, typename Extent_Expression, typename Stride_Expression>
	Vector<integer, N> numbers(
		const VectorExpression<integer, N, Extent_Expression>& extent,
		const VectorExpression<integer, N, Stride_Expression>& stride)
	{
		return (extent + mabs(stride) - 1) / mabs(stride);
	}

	template <int N, typename Type>
	class SubArray;

	template <int N, typename Type>
	class ConstSubArray;

	template <int N, typename Type>
	class SubArray_ConstIterator
		: public boost::bidirectional_iterator_helper<
		SubArray_ConstIterator<N, Type>, Type, integer>
	{
	public:
		friend class SubArray<N, Type>;

		const Type& operator*() const
		{
			return *data_;
		}

		SubArray_ConstIterator& operator++()
		{
			const Vector<integer, N>& extent = subArray_->extent();
			const Vector<integer, N>& stride = subArray_->stride();
			const integer n = extent.dimension();

			for (integer i = 0;i < n;++i)
			{
				data_ += stride[i];
				++position_[i];
				if (position_[i] >= extent[i] &&
					i != n - 1)
				{
					data_ -= stride[i] * position_[i];
					position_[i] = 0;
				}
				else
				{
					break;
				}
			}

			return *this;
		}

		SubArray_ConstIterator& operator--()
		{
			const Vector<integer, N>& extent = subArray_->extent();
			const Vector<integer, N>& stride = subArray_->stride();
			const integer n = extent.dimension();

			for (integer i = 0;i < n;++i)
			{
				data_ -= stride[i];
				--position_[i];
				if (position_[i] < 0 &&
					i != n - 1)
				{
					data_ += stride[i] * position_[i];
					position_[i] = extent[i] - 1;
				}
				else
				{
					break;
				}
			}

			return *this;
		}

		bool operator==(const SubArray_ConstIterator& that) const
		{
			return data_ == that.data_;
		}

	private:
		SubArray_ConstIterator(
			const ConstSubArray<N, Type>* subArray,
			const Point<integer, N>& position)
			: data_(subArray->address(position))
			, position_(position)
			, subArray_(subArray)
		{
		}

		const Type* data_;
		Point<integer, N> position_;
		const ConstSubArray<N, Type>* subArray_;
	};

	template <int N, typename Type>
	class SubArray_Iterator
		: public boost::bidirectional_iterator_helper<
		SubArray_Iterator<N, Type>, Type, integer>
	{
	public:
		friend class SubArray<N, Type>;

		Type& operator*() const
		{
			return *data_;
		}

		SubArray_Iterator& operator++()
		{
			const Vector<integer, N>& extent = subArray_->extent();
			const Vector<integer, N>& stride = subArray_->stride();
			const integer n = extent.dimension();

			for (integer i = 0;i < n;++i)
			{
				data_ += stride[i];
				++position_[i];
				if (position_[i] >= extent[i] &&
					i != n - 1)
				{
					data_ -= stride[i] * position_[i];
					position_[i] = 0;
				}
				else
				{
					break;
				}
			}

			return *this;
		}

		SubArray_Iterator& operator--()
		{
			const Vector<integer, N>& extent = subArray_->extent();
			const Vector<integer, N>& stride = subArray_->stride();
			const integer n = extent.dimension();

			for (integer i = 0;i < n;++i)
			{
				data_ -= stride[i];
				--position_[i];
				if (position_[i] < 0 &&
					i != n - 1)
				{
					data_ += stride[i] * position_[i];
					position_[i] = extent[i] - 1;
				}
				else
				{
					break;
				}
			}

			return *this;
		}

		bool operator==(const SubArray_Iterator& that) const
		{
			return data_ == that.data_;
		}

	private:
		SubArray_Iterator(
			const SubArray<N, Type>* subArray,
			const Point<integer, N>& position)
			: data_(subArray->address(position))
			, position_(position)
			, subArray_(subArray)
		{
		}

		Type* data_;
		Point<integer, N> position_;
		const SubArray<N, Type>* subArray_;
	};

}

#endif
