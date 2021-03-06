// Description: SubArray class
// Documentation: array.txt

#ifndef PASTELSYS_SUBARRAY_H
#define PASTELSYS_SUBARRAY_H

#include "pastel/sys/iterator/sparse_iterator.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/subarray_iterator.h"

namespace Pastel
{

	template <typename Type, int N>
	class Array;

	template <typename Type, int N>
	class ConstSubArray;

	template <typename Type, int N = 2>
	class SubArray
	{
	private:
		static constexpr int Smaller = SubN<N>;

	public:
		friend class ConstSubArray<Type, N>;

		typedef SubArray_Iterator<Type, N> 
			Iterator;
		typedef SubArray_ConstIterator<Type, N> 
			ConstIterator;
		using Range = ranges::subrange<Iterator>
;
		using ConstRange = ranges::subrange<ConstIterator>
;

		typedef SparseIterator<Type*> 
			RowIterator;
		typedef ConstSparseIterator<const Type*> 
			ConstRowIterator;
		using RowRange = ranges::subrange<RowIterator>
;
		using ConstRowRange = ranges::subrange<ConstRowIterator>
;

		// Using default copy constructor.
		// Using default destructor.

		SubArray()
			: data_(0)
			, stride_(0)
			, extent_(0)
			, extentStride_(0)
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
			, extentStride_(extent)
			, size_(product(extent))
			, dataBegin_(0)
			, dataEnd_(0)

		{
			PENSURE(allGreaterEqual(extent, 0));
			PENSURE(!anyEqual(stride, 0));

			computeDataRange();
		}

		const SubArray& operator=(
			const SubArray<Type, N>& that) const
		{
			PENSURE(allEqual(extent_, that.extent()));

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

		const SubArray& operator=(
			const ConstSubArray<Type, N>& that) const
		{
			return (*this = that.subArray_);
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
			extentStride_.swap(that.extentStride_);
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

		integer n() const
		{
			return extent_.n();
		}

		integer size() const
		{
			return size_;
		}

		Type* data() const
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

			SubArray<Type, N> result(
				address(min),
				stride_,
				abs(max - min));
			
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

			SubArray<Type, N> result(
				address(min),

				stride_ * delta,
				numbers(abs(max - min), delta));
			
			return result;
		}

		// Slicing

		SubArray<Type, Smaller> slice(
			integer axis,
			integer index) const
		{
			PENSURE_OP(axis, >=, 0);
			PENSURE_OP(axis, <, n());
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, extent_[axis]);

			Vector<integer, Smaller> sliceExtent(
				shrink(extent_, axis));

			Vector<integer, Smaller> sliceStride(
				shrink(stride_, axis));

			SubArray<Type, Smaller> result(
				data_ + index * stride_[axis],
				sliceStride, sliceExtent);

			return result;
		}

		// Iterators

		Iterator begin(integer index) const
		{
			ENSURE_OP(index, >=, 0);
			ENSURE_OP(index, <=, size_);

			integer d = n();

			Vector<integer, N> position(
				ofDimension(d), 0);
			for (integer i = d - 1;i >= 0;--i)
			{
				position[i] = index / extentStride_[i];

				index -= position[i] * extentStride_[i];
			}

			return Iterator(
				data_,
				position,
				extent_,
				stride_);
		}

		Iterator begin() const
		{
			return Iterator(
				data_, 
				Vector<integer, N>(ofDimension(n()), 0),
				extent_,
				stride_);
		}

		Iterator end() const
		{
			integer d = n();
			return Iterator(
				data_,

				Vector<integer, N>(unitAxis<integer, N>(d - 1) * extent_[d - 1]),
				extent_,
				stride_);
		}

		Range range() const
		{
			return Range(begin(), end());
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

		Range rowRange(integer index, 
			const Vector<integer, N>& position) const
		{
			return Range(rowBegin(index, position), 
				rowEnd(index, position));
		}

		Vector<integer, N> position(
			integer index) const
		{
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, size());

			integer d = n();

			Vector<integer, N> result(
				ofDimension(d));

			for (integer i = d - 1;i > 0;--i)
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
			PENSURE(allLess(position, extent()));

			integer d = n();

			integer index = position[0];
			for (integer i = 1;i < d;++i)
			{

				index += extentStride_[i] * position[i];
			}
			
			return index;
		}

		Type* address(const Vector<integer, N>& position) const
		{
			return data_ + dot(position, stride_);
		}

		Type* dataBegin() const
		{
			return dataBegin_;
		}

		Type* dataEnd() const
		{
			return dataEnd_;
		}

	private:
		void computeDataRange()
		{
			dataBegin_ = data_;
			dataEnd_ = data_;

			integer d = n();
			for (integer i = 0;i < d;++i)
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

			extentStride_[0] = 1;
			for (integer i = 1;i < d;++i)
			{
				extentStride_[i] = extentStride_[i - 1] * extent_[i - 1];
			}
		}

		// Iteration region
		Type* data_;
		Vector<integer, N> stride_;
		Vector<integer, N> extent_;

		// Precomputed stuff
		Vector<integer, N> extentStride_;
		integer size_;

		// Memory range
		Type* dataBegin_;
		Type* dataEnd_;
	};

	template <typename Type, int N = 2>
	class ConstSubArray
	{
	private:
		static constexpr int Smaller = SubN<N>;

	public:
		friend class SubArray<Type, N>;

		using ConstIterator = SubArray_ConstIterator<Type, N>;
		using ConstRowIterator = ConstSparseIterator<const Type*>;

		using ConstRange = ranges::subrange<ConstIterator>
;
		using ConstRowRange = ranges::subrange<ConstRowIterator>
;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		ConstSubArray()
			: subArray_()
		{
		}

		ConstSubArray(
			const SubArray<Type, N>& that)
			: subArray_(that)
		{
		}

		void swap(ConstSubArray& that)
		{
			subArray_.swap(that.subArray_);
		}

		// Properties

		const Vector<integer, N>& stride() const
		{
			return subArray_.stride();
		}

		const Vector<integer, N>& extent() const
		{
			return subArray_.extent();
		}

		integer n() const
		{
			return subArray_.n();
		}

		integer size() const
		{
			return subArray_.size();
		}

		const Type* data() const
		{
			return subArray_.data();
		}

		bool involves(const void* memoryBegin, const void* memoryEnd) const
		{
			return subArray_.involves(memoryBegin, memoryEnd);
		}

		// Data access

		const Type& operator()(const Vector<integer, N>& position) const
		{
			return subArray_(position);
		}

		ConstSubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max) const
		{
			return subArray_(min, max);
		}

		ConstSubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max,
			const Vector<integer, N>& delta) const
		{
			return subArray_(min, max, delta);
		}

		// Slicing

		ConstSubArray<Type, Smaller> slice(
			integer axis,
			integer index) const
		{
			return subArray_.slice(axis, index);
		}

		// Iterators

		ConstIterator begin(integer index) const
		{
			return subArray_.begin(index);
		}

		ConstIterator begin() const
		{
			return subArray_.begin();
		}

		ConstIterator end() const
		{
			return subArray_.end();
		}

		ConstRange range() const
		{
			return ConstRange(begin(), end());
		}

		const Type* dataBegin() const
		{
			return subArray_.dataBegin();
		}

		const Type* dataEnd() const
		{
			return subArray_.dataEnd();
		}

		// Row iterators

		ConstRowIterator rowBegin(integer index, 
			const Vector<integer, N>& position) const
		{
			return subArray_.rowBegin(index, position);
		}

		ConstRowIterator rowEnd(integer index, 
			const Vector<integer, N>& position) const
		{
			return subArray_.rowEnd(index, position);
		}

		ConstRowRange rowRange(integer index, 
			const Vector<integer, N>& position) const
		{
			return ConstRowRange(
				rowBegin(index, position), 
				rowEnd(index, position));
		}

		const Type* address(const Vector<integer, N>& position) const
		{
			return subArray_.address(position);
		}

		Vector<integer, N> position(integer index) const
		{
			return subArray_.position(index);
		}

		integer index(const Vector<integer, N>& position) const
		{
			return subArray_.index(position);
		}

	private:
		SubArray<Type, N> subArray_;

		ConstSubArray& operator=(
			const ConstSubArray& that) = delete;
	};

}

#endif
