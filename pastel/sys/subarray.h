// Description: SubArray class
// Documentation: array.txt

#ifndef PASTEL_SUBARRAY_H
#define PASTEL_SUBARRAY_H

#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/range.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/subarray_iterator.h"

#include "pastel/sys/integer_tools.h"

namespace Pastel
{

	template <typename Type, int N>
	class ConstSubArray;

	template <typename Type, int N = 2>
	class SubArray
	{
	private:
		enum
		{
			Smaller = ModifyN<N, N - 1>::Result
		};

	public:
		friend class ConstSubArray<Type, N>;

		typedef SubArray_Iterator<Type, N> Iterator;
		typedef SubArray_ConstIterator<Type, N> ConstIterator;
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
			const Vector<integer, N>& stride,
			const Vector<integer, N>& extent)
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

		operator ConstSubArray<Type, N>() const
		{
			const ConstSubArray<Type, N> result(
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
				
				Array<Type, N> copy(that.extent());
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

		const Vector<integer, N>& stride() const
		{
			return stride_;
		}

		const Vector<integer, N>& extent() const
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

		Type* rawBegin() const
		{
			return dataBegin_;
		}

		Type* rawEnd() const
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

		Type& operator()(const Vector<integer, N>& position) const
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLess(position, extent_));

			return *address(position);
		}

		SubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max) const
		{
			PENSURE(allLess(min, extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(max, extent_));
			PENSURE(allGreaterEqual(max, -1));

			const SubArray<Type, N> result(
				address(min),
				stride_,
				mabs(max - min));
			
			return result;
		}

		SubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max,
			const Vector<integer, N>& delta) const
		{
			PENSURE(allLess(min, extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(max, extent_));
			PENSURE(allGreaterEqual(max, -1));
			PENSURE(!anyEqual(delta, 0));

			const SubArray<Type, N> result(
				address(min),
				stride_ * delta,
				numbers(mabs(max - min), delta));
			
			return result;
		}

		// Slicing

		SubArray<Type, Smaller> slice(
			integer axis,
			integer index) const
		{
			PENSURE_OP(axis, >=, 0);
			PENSURE_OP(axis, <, dimension());
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, extent_[axis]);

			const Vector<integer, Smaller> sliceExtent(
				shrink(extent_, axis));

			const Vector<integer, Smaller> sliceStride(
				shrink(stride_, axis));

			const SubArray<Type, Smaller> result(
				data_ + index * stride_[axis],
				sliceStride, sliceExtent);

			return result;
		}

		// Iterators

		Iterator begin(integer index) const
		{
			ENSURE_OP(index, >=, 0);
			ENSURE_OP(index, <=, size_);

			const integer n = extent_.dimension();
			
			Vector<integer, N> position(
				ofDimension(n), 0);
			for (integer i = n - 1;i >= 0;--i)
			{
				position[i] = index / stride_[i];
				index -= position[i] * stride_[i];
			}

			return Iterator(
				this, position);
		}

		Iterator begin() const
		{
			return Iterator(
				this,
				Vector<integer, N>(ofDimension(extent_.dimension()), 0));
		}

		Iterator end() const
		{
			const integer n = extent_.dimension();
			return Iterator(
				this,
				Vector<integer, N>(unitAxis<integer, N>(n - 1) * extent_[n - 1]));
		}

		// Row iterators

		RowIterator rowBegin(integer index, 
			const Vector<integer, N>& position) const
		{
			return RowIterator(address(position), stride_[index]);
		}

		RowIterator rowEnd(integer index, 
			const Vector<integer, N>& position) const
		{
			return RowIterator(address(position) + 
				(extent_[index] - position[index]) * stride_[index], 
				stride_[index]);
		}

		Type* address(const Vector<integer, N>& position) const
		{
			return data_ + dot(position, stride_);
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
		Vector<integer, N> stride_;
		Vector<integer, N> extent_;
		integer size_;
		Type* dataBegin_;
		Type* dataEnd_;
	};

	template <typename Type, int N = 2>
	class ConstSubArray
	{
	private:
		enum
		{
			Smaller = ModifyN<N, N - 1>::Result
		};

	public:
		typedef SubArray_ConstIterator<Type, N> ConstIterator;
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
			const SubArray<Type, N>& that)
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
			const Vector<integer, N>& stride,
			const Vector<integer, N>& extent)
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

		const Vector<integer, N>& stride() const
		{
			return stride_;
		}

		const Vector<integer, N>& extent() const
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

		const Type* rawBegin() const
		{
			return dataBegin_;
		}

		const Type* rawEnd() const
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

		const Type& operator()(const Vector<integer, N>& position) const
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLess(position, extent_));

			return *address(position);
		}

		ConstSubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max) const
		{
			PENSURE(allLess(min, extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(max, extent_));
			PENSURE(allGreaterEqual(max, -1));

			const ConstSubArray<Type, N> result(
				address(min),
				stride_,
				mabs(max - min));
			
			return result;
		}

		ConstSubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max,
			const Vector<integer, N>& delta) const
		{
			PENSURE(allLess(min, extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(max, extent_));
			PENSURE(allGreaterEqual(max, -1));
			PENSURE(!anyEqual(delta, 0));

			const ConstSubArray<Type, N> result(
				address(min),
				stride_ * delta,
				numbers(mabs(max - min), delta));
			
			return result;
		}

		// Slicing

		ConstSubArray<Type, Smaller> slice(
			integer axis,
			integer index) const
		{
			PENSURE_OP(axis, >=, 0);
			PENSURE_OP(axis, <, dimension());
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, extent_[axis]);

			const Vector<integer, Smaller> sliceExtent(
				shrink(extent_, axis));

			const Vector<integer, Smaller> sliceStride(
				shrink(stride_, axis));

			const ConstSubArray<Type, Smaller> result(
				data_ + index * stride_[axis],
				sliceStride, sliceExtent);

			return result;
		}

		// Iterators

		ConstIterator begin(integer index) const
		{
			ENSURE_OP(index, >=, 0);
			ENSURE_OP(index, <=, size_);

			const integer n = extent_.dimension();
			
			Vector<integer, N> position(
				ofDimension(n), 0);
			for (integer i = n - 1;i >= 0;--i)
			{
				position[i] = index / stride_[i];
				index -= position[i] * stride_[i];
			}

			return ConstIterator(
				this, position);
		}

		ConstIterator begin() const
		{
			return ConstIterator(
				this,
				Vector<integer, N>(ofDimension(extent_.dimension()), 0));
		}

		ConstIterator end() const
		{
			const integer n = extent_.dimension();
			return ConstIterator(
				this,
				Vector<integer, N>(unitAxis<integer, N>(n - 1) * extent_[n - 1]));
		}

		// Row iterators

		ConstRowIterator rowBegin(integer index, 
			const Vector<integer, N>& position) const
		{
			return ConstRowIterator(address(position), stride_[index]);
		}

		ConstRowIterator rowEnd(integer index, 
			const Vector<integer, N>& position) const
		{
			return ConstRowIterator(address(position) + 
				(extent_[index] - position[index]) * stride_[index], 
				stride_[index]);
		}

		const Type* address(const Vector<integer, N>& position) const
		{
			return data_ + dot(position, stride_);
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
		Vector<integer, N> stride_;
		Vector<integer, N> extent_;
		integer size_;
		const Type* dataBegin_;
		const Type* dataEnd_;
	};

}

#endif
