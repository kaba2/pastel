#ifndef PASTEL_ARRAYBASE_HPP
#define PASTEL_ARRAYBASE_HPP

#include "pastel/sys/arraybase.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/stdext_destruct.h"
#include "pastel/sys/vector_tools.h"

#include <algorithm>

namespace Pastel
{

	namespace Detail_Array
	{

		template <typename Type, int N>
		ArrayBase<Type, N>::ArrayBase()
			: extent_(0)
			, stride_(0)
			, size_(0)
			, data_(0)
			, deleteData_(true)
		{
		}

		template <typename Type, int N>
		ArrayBase<Type, N>::ArrayBase(
			const Vector<integer, N>& extent,
			const Alias<Type*>& dataAlias)
			: extent_(extent)
			, stride_(0)
			, size_(product(extent))
			, data_(dataAlias)
			, deleteData_(false)
		{
			ENSURE(allGreaterEqual(extent, 0));

			Vector<integer, N> newFactor;

			newFactor[0] = 1;

			for (integer i = 1;i < N;++i)
			{
				newFactor[i] = newFactor[i - 1] * extent[i - 1];
			}

			stride_ = newFactor;
		}

		template <typename Type, int N>
		ArrayBase<Type, N>::ArrayBase(
			const Vector<integer, N>& extent,
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

		template <typename Type, int N>
		ArrayBase<Type, N>::ArrayBase(
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

		template <typename Type, int N>
		ArrayBase<Type, N>::ArrayBase(
			const ArrayBase& that,
			const Vector<integer, N>& extent,
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

		template <typename Type, int N>
		ArrayBase<Type, N>::~ArrayBase()
		{
			clear();
		}

		template <typename Type, int N>
		void ArrayBase<Type, N>::clear()
		{
			if (deleteData_ && data_)
			{
				StdExt::destruct(data_, data_ + size_);
				deallocate();
			}

			extent_.set(0);
			stride_.set(0);
			size_ = 0;
			deleteData_ = true;
		}

		template <typename Type, int N>
		void ArrayBase<Type, N>::swap(ArrayBase& that)
		{
			extent_.swap(that.extent_);
			stride_.swap(that.stride_);
			std::swap(size_, that.size_);
			std::swap(data_, that.data_);
			std::swap(deleteData_, that.deleteData_);
		}

		template <typename Type, int N>
		void ArrayBase<Type, N>::setExtent(
			const Vector<integer, N>& extent,
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
					ArrayBase<Type, N> copy(*this, extent,
						defaultData);
					swap(copy);
				}
			}
		}

		template <typename Type, int N>
		const Vector<integer, N>& ArrayBase<Type, N>::extent() const
		{
			return extent_;
		}

		template <typename Type, int N>
		void ArrayBase<Type, N>::reshape(
			const Vector<integer, N>& extent)
		{
			ENSURE(allGreaterEqual(extent, 0));

			const integer newSize = product(extent);
			
			ENSURE_OP(newSize, ==, size_);

			if (newSize > 0)
			{
				Vector<integer, N> newFactor;

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

		template <typename Type, int N>
		bool ArrayBase<Type, N>::empty() const
		{
			return size_ == 0;
		}

		template <typename Type, int N>
		integer ArrayBase<Type, N>::size() const
		{
			return size_;
		}

		template <typename Type, int N>
		integer ArrayBase<Type, N>::dimension() const
		{
			return extent_.dimension();
		}

		template <typename Type, int N>
		ArrayBase<Type, N>& ArrayBase<Type, N>::operator=(
			const ArrayBase& that)
		{
			if (extent() != that.extent())
			{
				ArrayBase<Type, N> copy(that);
				swap(copy);
			}
			else
			{
				std::copy(that.begin(), that.end(), 
					begin());
			}

			return *this;
		}

		template <typename Type, int N>
		Type& ArrayBase<Type, N>::operator()(
			integer index)
		{
			return (Type&)((const ArrayBase&)*this)(index);
		}

		template <typename Type, int N>
		const Type& ArrayBase<Type, N>::operator()(
			integer index) const
		{
			PENSURE2(index >= 0 && index < size_, index, size_);

			return *((const Type*)data_ + index);
		}

		template <typename Type, int N>
		Type& ArrayBase<Type, N>::operator()(
			const Vector<integer, N>& position)
		{
			return (Type&)((const ArrayBase&)*this)(position);
		}

		template <typename Type, int N>
		const Type& ArrayBase<Type, N>::operator()(
			const Vector<integer, N>& position) const
		{
			PENSURE(allGreaterEqual(position, 0));
			PENSURE(allLess(position, extent_));
			
			return *address(position);
		}

		template <typename Type, int N>
		SubArray<Type, N> ArrayBase<Type, N>::operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max)
		{
			PENSURE(allLess(min, extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(max, extent_));
			PENSURE(allGreaterEqual(max, -1));

			Vector<integer, N> newStride(stride_);
			for (integer i = 0;i < N;++i)
			{
				if (max[i] < min[i])
				{
					newStride[i] = -newStride[i];
				}
			}

			const SubArray<Type, N> result(
				address(min), newStride, mabs(max - min));

			return result;
		}

		template <typename Type, int N>
		ConstSubArray<Type, N> ArrayBase<Type, N>::operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max) const
		{
			return ((ArrayBase&)*this)(min, max);
		}

		template <typename Type, int N>
		SubArray<Type, N> ArrayBase<Type, N>::operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max,
			const Vector<integer, N>& delta)
		{
			PENSURE(allLess(min, extent_));
			PENSURE(allGreaterEqual(min, 0));
			PENSURE(allLessEqual(max, extent_));
			PENSURE(allGreaterEqual(max, -1));
			PENSURE(!anyEqual(delta, 0));

			for (integer i = 0;i < N;++i)
			{
				PENSURE((min[i] < max[i]) == (delta[i] > 0));
			}

			const SubArray<Type, N> result(
				address(min), stride_ * delta, 
				numbers(mabs(max - min), delta));

			return result;
		}

		template <typename Type, int N>
		ConstSubArray<Type, N> ArrayBase<Type, N>::operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max,
			const Vector<integer, N>& delta) const
		{
			return ((ArrayBase&)*this)(min, max, delta);
		}

		template <typename Type, int N>
		SubArray<Type, N> ArrayBase<Type, N>::operator()()
		{
			const SubArray<Type, N> result(
				data_, stride_, extent_);

			return result;
		}

		template <typename Type, int N>
		ConstSubArray<Type, N> ArrayBase<Type, N>::operator()() const
		{
			return ((ArrayBase&)*this)();
		}

		template <typename Type, int N>
		CommaFiller<Type, typename ArrayBase<Type, N>::Iterator> 
			ArrayBase<Type, N>::operator|=(const Type& that)
		{
			return commaFiller<Type>(begin(), end(), that);
		}

		template <typename Type, int N>
		ArrayBase<Type, N>& ArrayBase<Type, N>::operator=(const Type that)
		{
			// The parameter is deliberately taken by value,
			// because a reference could be from this array.
			std::fill(begin(), end(), that);
			return *this;
		}

		// Private

		template <typename Type, int N>
		void ArrayBase<Type, N>::allocate(
			const Vector<integer, N>& extent)
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

			Vector<integer, N> newFactor;

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

		template <typename Type, int N>
		void ArrayBase<Type, N>::deallocate()
		{
			deallocateRaw((void*)data_);
			data_ = 0;
			size_ = 0;
			stride_.set(0);
			extent_.set(0);
			deleteData_ = true;
		}

		template <typename Type, int N>
		void ArrayBase<Type, N>::copyConstruct(
			const ArrayBase& that,
			const Type& defaultData)
		{
			if (size_ == 0)
			{
				return;
			}

			const Vector<integer, N> minExtent = min(extent_, that.extent_);

			integer outOfCopyZone = 0;
			integer traversed = 0;
			Vector<integer, N> position(0);

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
				StdExt::destruct(data_, data_ + traversed);
				throw;
			};
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::Cursor
			ArrayBase<Type, N>::cursor(
			const Vector<integer, N>& position)
		{
			return Cursor(&(*this)(position), stride_);
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::ConstCursor
			ArrayBase<Type, N>::constCursor(
			const Vector<integer, N>& position) const
		{
			return ConstCursor(&(*this)(position), stride_);
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::Iterator
			ArrayBase<Type, N>::begin()
		{
			return data_;
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::ConstIterator
			ArrayBase<Type, N>::begin() const
		{
			return data_;
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::Iterator
			ArrayBase<Type, N>::end()
		{
			return data_ + size_;
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::ConstIterator
			ArrayBase<Type, N>::end() const
		{
			return data_ + size_;
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::RowIterator 
			ArrayBase<Type, N>::rowBegin(
			const Vector<integer, N>& position, 
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

		template <typename Type, int N>
		typename ArrayBase<Type, N>::ConstRowIterator 
			ArrayBase<Type, N>::rowBegin(
			const Vector<integer, N>& position, 
			integer axis) const
		{
			return ((ArrayBase&)*this).rowBegin(
				position, axis);
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::RowIterator 
			ArrayBase<Type, N>::rowEnd(
			const Vector<integer, N>& position, 
			integer axis)
		{
			return rowBegin(position, axis) + extent_[axis];
		}

		template <typename Type, int N>
		typename ArrayBase<Type, N>::ConstRowIterator 
			ArrayBase<Type, N>::rowEnd(
			const Vector<integer, N>& position, 
			integer axis) const
		{
			return ((ArrayBase&)*this).rowEnd(
				position, axis);
		}

		template <typename Type, int N>
		const Type* ArrayBase<Type, N>::address(const Vector<integer, N>& position) const
		{
			const Type* result = data_ + position[0];
			for (integer i = 1;i < N;++i)
			{
				result += position[i] * stride_[i];
			}

			return result;
		}

		template <typename Type, int N>
		Type* ArrayBase<Type, N>::address(const Vector<integer, N>& position)
		{
			return (Type*)((const ArrayBase&)*this).address(position);
		}

	}

}

#endif
