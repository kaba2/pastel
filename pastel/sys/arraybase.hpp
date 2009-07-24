#ifndef PASTEL_ARRAYBASE_HPP
#define PASTEL_ARRAYBASE_HPP

#include "pastel/sys/arraybase.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/destruct.h"
#include "pastel/sys/vector_tools.h"

#include <algorithm>

namespace Pastel
{

	namespace Detail_Array
	{

		template <int N, typename Type>
		ArrayBase<N, Type>::ArrayBase()
			: extent_(0)
			, stride_(0)
			, size_(0)
			, data_(0)
			, deleteData_(true)
		{
		}

		template <int N, typename Type>
		ArrayBase<N, Type>::ArrayBase(
			const Vector<N, integer>& extent,
			const Alias<Type*>& dataAlias)
			: extent_(extent)
			, stride_(0)
			, size_(product(extent))
			, data_(dataAlias)
			, deleteData_(false)
		{
			ENSURE(allGreaterEqual(extent, 0));

			Vector<N, integer> newFactor;

			newFactor[0] = 1;

			for (integer i = 1;i < N;++i)
			{
				newFactor[i] = newFactor[i - 1] * extent[i - 1];
			}

			stride_ = newFactor;
		}

		template <int N, typename Type>
		ArrayBase<N, Type>::ArrayBase(
			const Vector<N, integer>& extent,
			const Type& defaultData)
			: extent_(0)
			, stride_(0)
			, size_(0)
			, data_(0)
			, deleteData_(true)
		{
			allocate(extent);

			try
			{
				std::uninitialized_fill_n(data_,
					size_, defaultData);
			}
			catch(...)
			{
				deallocate();
				throw;
			};
		}

		template <int N, typename Type>
		ArrayBase<N, Type>::ArrayBase(
			const ArrayBase& that)
			: extent_(0)
			, stride_(0)
			, size_(0)
			, data_(0)
			, deleteData_(true)
		{
			allocate(that.extent_);

			try
			{
				std::uninitialized_copy(
					that.data_, that.data_ + size_, data_);
			}
			catch(...)
			{
				deallocate();
				throw;
			};
		}

		template <int N, typename Type>
		ArrayBase<N, Type>::ArrayBase(
			const ArrayBase& that,
			const Vector<N, integer>& extent,
			const Type& defaultData)
			: extent_(0)
			, stride_(0)
			, size_(0)
			, data_(0)
			, deleteData_(true)
		{
			allocate(extent);

			try
			{
				if (that.size() == 0)
				{
					// Nothing to copy, pure
					// default initialization.
					std::uninitialized_fill_n(data_,
						size_, defaultData);
				}
				else if (extent_ == that.extent_)
				{
					// Nothing to default initialize,
					// pure copy construction.
					std::uninitialized_copy(
						that.data_, that.data_ + size_, data_);
				}
				else
				{
					// Both copy construction and
					// default initialization needed.
					copyConstruct(that, defaultData);
				}
			}
			catch(...)
			{
				deallocate();
				throw;
			};
		}

		template <int N, typename Type>
		ArrayBase<N, Type>::~ArrayBase()
		{
			clear();
		}

		template <int N, typename Type>
		void ArrayBase<N, Type>::clear()
		{
			if (deleteData_ && data_)
			{
				destruct(data_, data_ + size_);
				deallocate();
			}

			extent_.set(0);
			stride_.set(0);
			size_ = 0;
			deleteData_ = true;
		}

		template <int N, typename Type>
		void ArrayBase<N, Type>::swap(ArrayBase& that)
		{
			extent_.swap(that.extent_);
			stride_.swap(that.stride_);
			std::swap(size_, that.size_);
			std::swap(data_, that.data_);
			std::swap(deleteData_, that.deleteData_);
		}

		template <int N, typename Type>
		void ArrayBase<N, Type>::setExtent(
			const Vector<N, integer>& extent,
			const Type& defaultData)
		{
			for (integer i = 0;i < N;++i)
			{
				ENSURE_OP(extent[i], >=, 0);
			}

			const integer units = product(extent);
			if (units == 0)
			{
				clear();
			}
			else
			{
				if (extent != extent_)
				{
					ArrayBase<N, Type> copy(*this, extent,
						defaultData);
					swap(copy);
				}
			}
		}

		template <int N, typename Type>
		const Vector<N, integer>& ArrayBase<N, Type>::extent() const
		{
			return extent_;
		}

		template <int N, typename Type>
		void ArrayBase<N, Type>::reshape(
			const Vector<N, integer>& extent)
		{
			ENSURE(allGreaterEqual(extent, 0));

			const integer newSize = product(extent);
			
			ENSURE_OP(newSize, ==, size_);

			if (newSize > 0)
			{
				Vector<N, integer> newFactor;

				newFactor[0] = 1;

				for (integer i = 1;i < N;++i)
				{
					newFactor[i] = newFactor[i - 1] * extent[i - 1];
				}

				stride_ = newFactor;
				extent_ = extent;
				size_ = newSize;
			}
		}

		template <int N, typename Type>
		bool ArrayBase<N, Type>::empty() const
		{
			return size_ == 0;
		}

		template <int N, typename Type>
		integer ArrayBase<N, Type>::size() const
		{
			return size_;
		}

		template <int N, typename Type>
		ArrayBase<N, Type>& ArrayBase<N, Type>::operator=(
			const ArrayBase& that)
		{
			if (extent() != that.extent())
			{
				ArrayBase<N, Type> copy(that);
				swap(copy);
			}
			else
			{
				std::copy(that.begin(), that.end(), 
					begin());
			}

			return *this;
		}

		template <int N, typename Type>
		Type& ArrayBase<N, Type>::operator()(
			integer index)
		{
			return (Type&)((const ArrayBase&)*this)(index);
		}

		template <int N, typename Type>
		const Type& ArrayBase<N, Type>::operator()(
			integer index) const
		{
			PENSURE2(index >= 0 && index < size_, index, size_);

			return *((const Type*)data_ + index);
		}

		template <int N, typename Type>
		Type& ArrayBase<N, Type>::operator()(
			const Point<N, integer>& position)
		{
			return (Type&)((const ArrayBase&)*this)(position);
		}

		template <int N, typename Type>
		const Type& ArrayBase<N, Type>::operator()(
			const Point<N, integer>& position) const
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLess(asVector(position), extent_));
			
			return *address(position);
		}

		template <int N, typename Type>
		SubArray<N, Type> ArrayBase<N, Type>::operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max)
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));

			Vector<N, integer> newStride(stride_);
			for (integer i = 0;i < N;++i)
			{
				if (max[i] < min[i])
				{
					newStride[i] = -newStride[i];
				}
			}

			const SubArray<N, Type> result(
				address(min), newStride, mabs(max - min));

			return result;
		}

		template <int N, typename Type>
		ConstSubArray<N, Type> ArrayBase<N, Type>::operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max) const
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));

			Vector<N, integer> newStride(stride_);
			for (integer i = 0;i < N;++i)
			{
				if (max[i] < min[i])
				{
					newStride[i] = -newStride[i];
				}
			}

			const ConstSubArray<N, Type> result(
				address(min), newStride, mabs(max - min));

			return result;
		}

		template <int N, typename Type>
		SubArray<N, Type> ArrayBase<N, Type>::operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max,
			const Vector<N, integer>& delta)
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));
			PENSURE(!anyEqual(delta, 0));

			for (integer i = 0;i < N;++i)
			{
				PENSURE((min[i] < max[i]) == (delta[i] > 0));
			}

			const SubArray<N, Type> result(
				address(min), stride_ * delta, 
				numbers(mabs(max - min), delta));

			return result;
		}

		template <int N, typename Type>
		ConstSubArray<N, Type> ArrayBase<N, Type>::operator()(
			const Point<N, integer>& min,
			const Point<N, integer>& max,
			const Vector<N, integer>& delta) const
		{
			PENSURE(allLess(asVector(min), extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(asVector(max), extent_));
			PENSURE(allGreaterEqual(max, -1));
			PENSURE(!anyEqual(delta, 0));

			for (integer i = 0;i < N;++i)
			{
				PENSURE((min[i] < max[i]) == (delta[i] > 0));
			}

			const ConstSubArray<N, Type> result(
				address(min), stride_ * delta, 
				numbers(mabs(max - min), delta));

			return result;
		}

		template <int N, typename Type>
		SubArray<N, Type> ArrayBase<N, Type>::operator()()
		{
			const SubArray<N, Type> result(
				data_, stride_, extent_);

			return result;
		}

		template <int N, typename Type>
		ConstSubArray<N, Type> ArrayBase<N, Type>::operator()() const
		{
			const ConstSubArray<N, Type> result(
				data_, stride_, extent_);

			return result;
		}

		template <int N, typename Type>
		CommaFiller<Type, typename ArrayBase<N, Type>::Iterator> 
			ArrayBase<N, Type>::operator|=(const Type& that)
		{
			return commaFiller<Type>(begin(), end(), that);
		}

		template <int N, typename Type>
		ArrayBase<N, Type>& ArrayBase<N, Type>::operator=(const Type that)
		{
			// The parameter is deliberately taken by value,
			// because a reference could be from this array.
			std::fill(begin(), end(), that);
			return *this;
		}

		// Private

		template <int N, typename Type>
		void ArrayBase<N, Type>::allocate(
			const Vector<N, integer>& extent)
		{
			ASSERT(data_ == 0);
			ASSERT(size_ == 0);

			const integer units = product(extent);
			if (units == 0)
			{
				return;
			}

			Type* newData = (Type*)allocateRaw(units * sizeof(Type));

			// Compute steps between consecutive
			// elements on each axis.

			Vector<N, integer> newFactor;

			newFactor[0] = 1;

			for (integer i = 1;i < N;++i)
			{
				newFactor[i] = newFactor[i - 1] * extent[i - 1];
			}

			stride_ = newFactor;
			extent_ = extent;
			size_ = units;
			data_ = newData;
			deleteData_ = true;
		}

		template <int N, typename Type>
		void ArrayBase<N, Type>::deallocate()
		{
			deallocateRaw((void*)data_);
			data_ = 0;
			size_ = 0;
			stride_.set(0);
			extent_.set(0);
			deleteData_ = true;
		}

		template <int N, typename Type>
		void ArrayBase<N, Type>::copyConstruct(
			const ArrayBase& that,
			const Type& defaultData)
		{
			if (size_ == 0)
			{
				return;
			}

			const Vector<N, integer> minExtent = min(extent_, that.extent_);

			integer outOfCopyZone = 0;
			integer traversed = 0;
			Point<N, integer> position(0);

			try
			{
				while(traversed < size_)
				{
					if (outOfCopyZone == 0)
					{
						std::uninitialized_copy(
							&that(position),
							&that(position) + minExtent[0],
							&(*this)(position));
						
						traversed += minExtent[0];

						if (extent_[0] > minExtent[0])
						{
							std::uninitialized_fill_n(
								&(*this)(position) + minExtent[0],
								extent_[0] - minExtent[0],
								defaultData);
							
							traversed -= minExtent[0];
							traversed += extent_[0];
						}
					}
					else
					{
						std::uninitialized_fill_n(
							&(*this)(position),
							extent_[0],
							defaultData);

						traversed += extent_[0];
					}

					for (integer i = 1;i < N;++i)
					{
						++position[i];
						if (position[i] == minExtent[i])
						{
							++outOfCopyZone;
						}
						if (position[i] == extent_[i])
						{
							position[i] = 0;
							--outOfCopyZone;
						}
						else
						{
							break;
						}
					}
				}
			}
			catch(...)
			{
				destruct(data_, data_ + traversed);
				throw;
			};
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::Cursor
			ArrayBase<N, Type>::cursor(
			const Point<N, integer>& position)
		{
			return Cursor(&(*this)(position), stride_);
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::ConstCursor
			ArrayBase<N, Type>::constCursor(
			const Point<N, integer>& position) const
		{
			return ConstCursor(&(*this)(position), stride_);
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::Iterator
			ArrayBase<N, Type>::begin()
		{
			return data_;
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::ConstIterator
			ArrayBase<N, Type>::begin() const
		{
			return data_;
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::Iterator
			ArrayBase<N, Type>::end()
		{
			return data_ + size_;
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::ConstIterator
			ArrayBase<N, Type>::end() const
		{
			return data_ + size_;
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::RowIterator 
			ArrayBase<N, Type>::rowBegin(
			const Point<N, integer>& position, 
			integer axis)
		{
			integer index = position[0];
			for (integer i = 1;i < axis;++i)
			{
				index += position[i] * stride_[i];
			}
			for (integer i = axis + 1;i < N;++i)
			{
				index += position[i] * stride_[i];
			}

			return RowIterator(
				data_ + index, stride_[axis]);
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::ConstRowIterator 
			ArrayBase<N, Type>::rowBegin(
			const Point<N, integer>& position, 
			integer axis) const
		{
			return ((ArrayBase&)*this).rowBegin(
				position, axis);
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::RowIterator 
			ArrayBase<N, Type>::rowEnd(
			const Point<N, integer>& position, 
			integer axis)
		{
			return rowBegin(position, axis) + extent_[axis];
		}

		template <int N, typename Type>
		typename ArrayBase<N, Type>::ConstRowIterator 
			ArrayBase<N, Type>::rowEnd(
			const Point<N, integer>& position, 
			integer axis) const
		{
			return ((ArrayBase&)*this).rowEnd(
				position, axis);
		}

		template <int N, typename Type>
		const Type* ArrayBase<N, Type>::address(const Point<N, integer>& position) const
		{
			const Type* result = data_ + position[0];
			for (integer i = 1;i < N;++i)
			{
				result += position[i] * stride_[i];
			}

			return result;
		}

		template <int N, typename Type>
		Type* ArrayBase<N, Type>::address(const Point<N, integer>& position)
		{
			return (Type*)((const ArrayBase&)*this).address(position);
		}

	}

}

#endif
