#ifndef PASTEL_SUBARRAY_H
#define PASTEL_SUBARRAY_H

#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/range.h"
#include "pastel/sys/point_tools.h"

#include "pastel/math/integer_tools.h"

#include "pastel/sys/subarray_iterator.h"

namespace Pastel
{

	template <int N, typename Type>
	class ConstSubArray;

	template <int N, typename Type>
	class SubArray
	{
	private:
		enum
		{
			Smaller = PASTEL_ADD_N(N, -1)
		};

	public:
		friend class ConstSubArray<N, Type>;

		typedef SubArray_Iterator<N, Type> Iterator;
		typedef SubArray_ConstIterator<N, Type> ConstIterator;
		typedef typename SparseIterator<Type*> RowIterator;
		typedef typename ConstSparseIterator<const Type*> ConstRowIterator;

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

		operator ConstSubArray<N, Type>() const
		{
			const ConstSubArray<N, Type> result(
				data_, stride_, extent_);

			return result;
		}

		const SubArray& operator=(
			const SubArray& that) const
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

		const SubArray& operator=(const Type& that) const
		{
			std::fill(begin(), end(), that);
			return *this;
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

		integer dimension() const
		{
			return extent_.dimension();
		}

		integer size() const
		{
			return size_;
		}

		Type* data() const
		{
			return data_;
		}

		Type* dataBegin() const
		{
			return dataBegin_;
		}

		Type* dataEnd() const
		{
			return dataEnd_;
		}

		bool involves(const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				dataBegin_, dataEnd_);
		}

		// Data access

		Type& operator()(const Point<N, integer>& position) const
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLess(asVector(position), extent_));

			return *address(position);
		}

		SubArray<N, Type> operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max) const
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));

			const SubArray<N, Type> result(
				address(min),
				stride_,
				mabs(max - min));
			
			return result;
		}

		SubArray<N, Type> operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max,
			const Vector<N, integer>& delta) const
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));
			PENSURE(!anyEqual(delta, 0));

			const SubArray<N, Type> result(
				address(min),
				stride_ * delta,
				numbers(mabs(max - min), delta));
			
			return result;
		}

		// Slicing

		SubArray<Smaller, Type> slice(
			integer axis,
			integer index) const
		{
			PENSURE_OP(axis, >=, 0);
			PENSURE_OP(axis, <, dimension());
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, extent_[axis]);

			const Vector<Smaller, integer> sliceExtent(
				shrink(extent_, axis));

			const Vector<Smaller, integer> sliceStride(
				shrink(stride_, axis));

			const SubArray<Smaller, Type> result(
				data_ + index * stride_[axis],
				sliceStride, sliceExtent);

			return result;
		}

		// Iterators

		Iterator begin() const
		{
			return Iterator(
				this,
				Point<N, integer>(ofDimension(extent_.dimension()), 0));
		}

		Iterator end() const
		{
			const integer n = extent_.dimension();
			return Iterator(
				this,
				Point<N, integer>(unitAxis<N, integer>(n - 1) * extent_[n - 1]));
		}

		// Row iterators

		RowIterator rowBegin(integer index, 
			const Point<N, integer>& position) const
		{
			return RowIterator(address(position), stride_[index]);
		}

		RowIterator rowEnd(integer index, 
			const Point<N, integer>& position) const
		{
			return RowIterator(address(position) + 
				(extent_[index] - position[index]) * stride_[index], 
				stride_[index]);
		}

		Type* address(const Point<N, integer>& position) const
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

	template <int N, typename Type>
	class ConstSubArray
	{
	private:
		enum
		{
			Smaller = PASTEL_ADD_N(N, -1)
		};

	public:
		typedef SubArray_ConstIterator<N, Type> ConstIterator;
		typedef typename ConstSparseIterator<const Type*> ConstRowIterator;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		ConstSubArray()
			: data_(0)
			, stride_(0)
			, extent_(0)
			, size_(0)
			, dataBegin_(0)
			, dataEnd_(0)
		{
		}

		/*
		ConstSubArray(
			const SubArray<N, Type>& that)
			: data_(that.data_)
			, stride_(that.stride_)
			, extent_(that.extent_)
			, size_(that.size_)
			, dataBegin_(that.dataBegin_)
			, dataEnd_(that.dataEnd_)
		{
		}
		*/

		ConstSubArray(
			const Type* data,
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

		void swap(ConstSubArray& that)
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

		integer dimension() const
		{
			return extent_.dimension();
		}

		integer size() const
		{
			return size_;
		}

		const Type* data() const
		{
			return data_;
		}

		const Type* dataBegin() const
		{
			return dataBegin_;
		}

		const Type* dataEnd() const
		{
			return dataEnd_;
		}

		bool involves(const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				dataBegin_, dataEnd_);
		}

		// Data access

		const Type& operator()(const Point<N, integer>& position) const
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLess(asVector(position), extent_));

			return *address(position);
		}

		ConstSubArray<N, Type> operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max) const
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));

			const ConstSubArray<N, Type> result(
				address(min),
				stride_,
				mabs(max - min));
			
			return result;
		}

		ConstSubArray<N, Type> operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max,
			const Vector<N, integer>& delta) const
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));
			PENSURE(!anyEqual(delta, 0));

			const ConstSubArray<N, Type> result(
				address(min),
				stride_ * delta,
				numbers(mabs(max - min), delta));
			
			return result;
		}

		// Slicing

		ConstSubArray<Smaller, Type> slice(
			integer axis,
			integer index) const
		{
			PENSURE_OP(axis, >=, 0);
			PENSURE_OP(axis, <, dimension());
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, extent_[axis]);

			const Vector<Smaller, integer> sliceExtent(
				shrink(extent_, axis));

			const Vector<Smaller, integer> sliceStride(
				shrink(stride_, axis));

			const ConstSubArray<Smaller, Type> result(
				data_ + index * stride_[axis],
				sliceStride, sliceExtent);

			return result;
		}

		// Iterators

		ConstIterator begin() const
		{
			return ConstIterator(
				this,
				Point<N, integer>(ofDimension(extent_.dimension()), 0));
		}

		ConstIterator end() const
		{
			const integer n = extent_.dimension();
			return ConstIterator(
				this,
				Point<N, integer>(unitAxis<N, integer>(n - 1) * extent_[n - 1]));
		}

		// Row iterators

		ConstRowIterator rowBegin(integer index, 
			const Point<N, integer>& position) const
		{
			return ConstRowIterator(address(position), stride_[index]);
		}

		ConstRowIterator rowEnd(integer index, 
			const Point<N, integer>& position) const
		{
			return ConstRowIterator(address(position) + 
				(extent_[index] - position[index]) * stride_[index], 
				stride_[index]);
		}

		const Type* address(const Point<N, integer>& position) const
		{
			return data_ + dot(asVector(position), stride_);
		}

	private:
		// Prohibited
		ConstSubArray& operator=(
			const ConstSubArray& that);

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

		const Type* data_;
		Vector<N, integer> stride_;
		Vector<N, integer> extent_;
		integer size_;
		const Type* dataBegin_;
		const Type* dataEnd_;
	};

}

#endif
