#ifndef PASTEL_SUBARRAY_H
#define PASTEL_SUBARRAY_H

#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/range.h"
#include "pastel/sys/point_tools.h"

#include "pastel/math/integer_tools.h"

namespace Pastel
{

	template <int N, typename Extent_Expression, typename Stride_Expression>
	Vector<N, integer> numbers(
		const VectorExpression<N, integer, Extent_Expression>& extent,
		const VectorExpression<N, integer, Stride_Expression>& stride)
	{
		return (extent + mabs(stride) - 1) / mabs(stride);
	}

	template <int N, typename Type>
	class SubArray;

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
			const Vector<N, integer>& extent = subArray_->extent();
			const Vector<N, integer>& stride = subArray_->stride();
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
			const Vector<N, integer>& extent = subArray_->extent();
			const Vector<N, integer>& stride = subArray_->stride();
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
			const SubArray<N, Type>* subArray,
			const Point<N, integer>& position)
			: data_(subArray->address(position))
			, position_(position)
			, subArray_(subArray)
		{
		}

		const Type* data_;
		Point<N, integer> position_;
		const SubArray<N, Type>* subArray_;
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
			const Vector<N, integer>& extent = subArray_->extent();
			const Vector<N, integer>& stride = subArray_->stride();
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
			const Vector<N, integer>& extent = subArray_->extent();
			const Vector<N, integer>& stride = subArray_->stride();
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
			SubArray<N, Type>* subArray,
			const Point<N, integer>& position)
			: data_(subArray->address(position))
			, position_(position)
			, subArray_(subArray)
		{
		}

		Type* data_;
		Point<N, integer> position_;
		SubArray<N, Type>* subArray_;
	};

	template <int N, typename Type>
	class SubArray_Ref
	{
	public:
		friend class SubArray<N, Type>;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		SubArray_Ref()
			: that_(0)
		{
		}

		void swap(SubArray_Ref& that)
		{
			std::swap(that_, that.that_);
		}

	protected:
		SubArray_Ref(
			SubArray<N, Type>* that)
			: that_(that)
		{
		}
		
		SubArray<N, Type>* that_;
	};

	template <int N, typename Type>
	class SubArray
	{
	public:
		typedef SubArray_Iterator<N, Type> Iterator;
		typedef SubArray_ConstIterator<N, Type> ConstIterator;
		typedef typename SparseIterator<Type*> RowIterator;
		typedef typename ConstSparseIterator<Type*> ConstRowIterator;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		SubArray()
			: data_(0)
			, stride_(0)
			, extent_(0)
			, size_(0)
			, dataBegin_(0)
			, dataEnd_(0)
		{
		}

		SubArray(
			const SubArray_Ref<N, integer>& ref)
			: data_(ref.that_->data_)
			, stride_(ref.that_->stride_)
			, extent_(ref.that_->extent_)
			, size_(ref.that_->size_)
			, dataBegin_(ref.that_->dataBegin_)
			, dataEnd_(ref.that_->dataEnd_)
		{
		}

		SubArray(
			Type* data,
			const Vector<N, integer>& stride,
			const Vector<N, integer>& extent)
			: data_(data)
			, stride_(stride)
			, extent_(extent)
			, size_(product(extent))
			, dataBegin_(0)
			, dataEnd_(0)

		{
			PENSURE(allGreaterEqual(extent, 0));
			PENSURE(!anyEqual(stride, 0));

			computeDataRange();
		}

		SubArray& operator=(
			const SubArray& that)
		{
			PENSURE(allEqual(extent_, that.extent_));

			if (that.involves(dataBegin_, dataEnd_))
			{
				// The memory region of the assigned 
				// sub-array overlaps with the
				// memory region of this sub-array.
				// Solve by making a copy.
				
				Array<N, Type> copy(that.extent());
				std::copy(that.begin(), that.end(),
					copy.begin());
				
				std::copy(
					copy.begin(), copy.end(),
					begin());
			}
			else
			{
				std::copy(
					that.begin(), that.end(),
					begin());
			}

			return *this;
		}

		SubArray& operator=(const Type& that)
		{
			std::fill(begin(), end(), that);
			return *this;
		}

		operator SubArray_Ref<N, Type>()
		{
			SubArray_Ref<N, Type> result(this);
			return result;
		}

		void swap(SubArray& that)
		{
			std::swap(data_, that.data_);
			stride_.swap(that.stride_);
			extent_.swap(that.extent_);
			std::swap(size_, that.size_);
			std::swap(dataBegin_, that.dataBegin_);
			std::swap(dataEnd_, that.dataEnd_);
		}

		// Properties

		const Vector<N, integer>& stride() const
		{
			return stride_;
		}

		const Vector<N, integer>& extent() const
		{
			return extent_;
		}

		integer size() const
		{
			return size_;
		}

		Type* data()
		{
			return data_;
		}

		const Type* data() const
		{
			return data_;
		}

		bool involves(const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				dataBegin_, dataEnd_);
		}

		// Data access

		Type& operator()(const Point<N, integer>& position)
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLess(asVector(position), extent_));

			return *address(position);
		}

		const Type& operator()(const Point<N, integer>& position) const
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLess(asVector(position), extent_));

			return *address(position);
		}

		SubArray<N, Type> operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max)
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));

			SubArray<N, Type> result(
				address(min),
				stride_,
				mabs(max - min));
			
			return result;
		}

		const SubArray<N, Type> operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max) const
		{
			return ((SubArray&)*this)(min, max);
		}

		SubArray<N, Type> operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max,
			const Vector<N, integer>& delta)
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));
			PENSURE(!anyEqual(delta, 0));

			SubArray<N, Type> result(
				address(min),
				stride_ * delta,
				numbers(mabs(max - min), delta));
			
			return result;
		}

		const SubArray<N, Type> operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max,
			const Vector<N, integer>& delta) const
		{
			return ((SubArray&)*this)(min, max, delta);
		}

		// Iterators

		Iterator begin()
		{
			return Iterator(
				this,
				Point<N, integer>(ofDimension(extent_.dimension()), 0));
		}

		ConstIterator begin() const
		{
			return ConstIterator(
				this,
				Point<N, integer>(ofDimension(extent_.dimension()), 0));
		}

		Iterator end()
		{
			const integer n = extent_.dimension();
			return Iterator(
				this,
				Point<N, integer>(unitAxis<N, integer>(n - 1) * extent_[n - 1]));
		}

		ConstIterator end() const
		{
			const integer n = extent_.dimension();
			return ConstIterator(
				this,
				Point<N, integer>(unitAxis<N, integer>(n - 1) * extent_[n - 1]));
		}

		// Row iterators

		RowIterator rowBegin(integer index, 
			const Point<N, integer>& position)
		{
			return RowIterator(address(position), stride_[index]);
		}

		ConstRowIterator rowBegin(integer index, 
			const Point<N, integer>& position) const
		{
			return ConstRowIterator(address(position), stride_[index]);
		}

		RowIterator rowEnd(integer index, 
			const Point<N, integer>& position)
		{
			return RowIterator(address(position) + 
				(extent_[index] - position_[index]) * stride_[index], 
				stride_[index]);
		}

		ConstRowIterator rowEnd(integer index, 
			const Point<N, integer>& position) const
		{
			return ConstRowIterator(address(position) + 
				(extent_[index] - position_[index]) * stride_[index], 
				stride_[index]);
		}

		Type* address(const Point<N, integer>& position)
		{
			return data_ + dot(asVector(position), stride_);
		}

		const Type* address(const Point<N, integer>& position) const
		{
			return data_ + dot(asVector(position), stride_);
		}

	private:
		void computeDataRange()
		{
			dataBegin_ = data_;
			dataEnd_ = data_;

			const integer n = extent_.dimension();
			for (integer i = 0;i < n;++i)
			{
				if (stride_[i] < 0)
				{
					dataBegin_ += stride_[i] * (extent_[i] - 1);
				}
				else
				{
					dataEnd_ += stride_[i] * (extent_[i] - 1);
				}
			}
			++dataEnd_;
		}

		Type* data_;
		Vector<N, integer> stride_;
		Vector<N, integer> extent_;
		integer size_;
		Type* dataBegin_;
		Type* dataEnd_;
	};

}

#endif
