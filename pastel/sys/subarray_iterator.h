#ifndef PASTELSYS_SUBARRAY_ITERATOR_H
#define PASTELSYS_SUBARRAY_ITERATOR_H

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
		: public boost::random_access_iterator_helper<
		SubArray_Iterator<Type, N>, Type, integer>
	{
	public:
		friend class SubArray<Type, N>;

		Type& operator*() const
		{
			return *data_;
		}

		const integer dimension() const
		{
			return extent_.dimension();
		}

		SubArray_Iterator& operator+=(integer amount)
		{
			PENSURE_OP(index_ + amount, <=, size_);
			PENSURE_OP(index_ + amount, >=, 0);

			position_ = position(index_ + amount);
			data_ = begin_ + dot(stride_, position_);
			index_ += amount;

			return *this;
		}

		SubArray_Iterator& operator-=(integer amount)
		{
			return *this += -amount;
		}

		integer operator-(const SubArray_Iterator& that) const
		{
			return index_ - that.index_;
		}

		SubArray_Iterator& operator++()
		{
			const integer n = dimension();
			for (integer i = 0;i < n;++i)
			{
				data_ += stride_[i];
				index_ += extentStride_[i];
				++position_[i];
				if (position_[i] >= extent_[i] &&
					i != n - 1)
				{
					data_ -= stride_[i] * position_[i];
					index_ -= extentStride_[i] * position_[i];
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
			const integer n = dimension();
			for (integer i = 0;i < n;++i)
			{
				data_ -= stride_[i];
				index_ -= extentStride_[i];
				--position_[i];
				if (position_[i] < 0 &&
					i != n - 1)
				{
					data_ += stride_[i] * position_[i];
					index_ += extentStride_[i] * position_[i];
					position_[i] = extent_[i] - 1;
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

		Vector<integer, N> position(
			integer index) const
		{
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <=, size());

			const integer n = dimension();

			Vector<integer, N> result(
				ofDimension(n));

			for (integer i = n - 1;i > 0;--i)
			{
				result[i] = index / extentStride_[i];
				index -= result[i] * extentStride_[i];
			}
			result[0] = index;

			return result;
		}

		integer index(
			const Vector<integer, N>& position) const
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLessEqual(position, extent_));

			const integer n = dimension();

			integer index = position[0];
			for (integer i = 1;i < n;++i)
			{
				index += extentStride_[i] * position[i];
			}
			
			return index;
		}

		const Vector<integer, N>& position() const
		{
			return position_;
		}

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		const Vector<integer, N>& stride() const
		{
			return stride_;
		}

		integer size() const
		{
			return size_;
		}

	private:
		SubArray_Iterator(
			Type* begin,
			const Vector<integer, N>& position,
			const Vector<integer, N>& extent,
			const Vector<integer, N>& stride)
			: begin_(begin)
			, extent_(extent)
			, stride_(stride)
			, size_(product(extent))
			, extentStride_(extent)
			, data_(begin)
			, index_(0)
			, position_(position)
		{
			if (size_ > 0)
			{
				const integer n = dimension();

				extentStride_[0] = 1;
				for (integer i = 1;i < n;++i)
				{
					extentStride_[i] = 
						extentStride_[i - 1] * extent_[i - 1];
				}

				data_ = begin_ + dot(position, stride);
				index_ = index(position);
			}
		}

		// Iteration region
		Type* begin_;
		Vector<integer, N> extent_;
		Vector<integer, N> stride_;

		// Precomputed stuff
		integer size_;
		Vector<integer, N> extentStride_;

		// Current position
		Type* data_;
		integer index_;
		Vector<integer, N> position_;
	};

	template <typename Type, int N = 2>
	class SubArray_ConstIterator
		: public boost::random_access_iterator_helper<
		SubArray_ConstIterator<Type, N>, const Type, integer>
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

		SubArray_ConstIterator& operator+=(integer amount)
		{
			iter_ += amount;

			return *this;
		}

		SubArray_ConstIterator& operator-=(integer amount)
		{
			iter_ -= amount;

			return *this;
		}

		integer operator-(const SubArray_ConstIterator& that) const
		{
			return iter_ - that.iter_;
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

		const Vector<integer, N>& extent() const
		{
			return iter_.extent();
		}

		const Vector<integer, N>& stride() const
		{
			return iter_.stride();
		}

		integer size() const
		{
			return iter_.size();
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
