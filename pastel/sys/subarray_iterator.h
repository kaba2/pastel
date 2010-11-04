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

	template <typename Type, int N>
	class SubArray;

	template <typename Type, int N>
	class ConstSubArray;

	template <typename Type, int N = 2>
	class SubArray_Iterator
		: public boost::bidirectional_iterator_helper<
		SubArray_Iterator<Type, N>, Type, integer>
	{
	public:
		friend class SubArray<Type, N>;

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

		const Vector<integer, N>& position() const
		{
			return position_;
		}

	private:
		SubArray_Iterator(
			const SubArray<Type, N>* subArray,
			const Vector<integer, N>& position)
			: data_(subArray->address(position))
			, position_(position)
			, subArray_(subArray)
		{
		}

		Type* data_;
		Vector<integer, N> position_;
		const SubArray<Type, N>* subArray_;
	};

	template <typename Type, int N = 2>
	class SubArray_ConstIterator
		: public boost::bidirectional_iterator_helper<
		SubArray_ConstIterator<Type, N>, Type, integer>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		friend class SubArray<Type, N>;
		friend class ConstSubArray<Type, N>;

		SubArray_ConstIterator()
			: iter_()
		{
		}

		SubArray_ConstIterator(
			const SubArray_Iterator<Type, N>& that)
			: iter_(that)
		{
		}

		const Type& operator*() const
		{
			return *iter_;
		}

		SubArray_ConstIterator& operator++()
		{
			++iter_;
			return *this;
		}

		SubArray_ConstIterator& operator--()
		{
			--iter_;
			return *this;
		}

		const Vector<integer, N>& position() const
		{
			return iter_.position();
		}

		bool operator==(const SubArray_ConstIterator& that) const
		{
			return iter_ == that.iter_;
		}

	private:
		SubArray_Iterator<Type, N> iter_;
	};

}

#endif
