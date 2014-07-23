#ifndef PASTELSYS_ARRAY_HPP
#define PASTELSYS_ARRAY_HPP

#include "pastel/sys/array.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/destruct.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/alignedbox_tools.h"
#include "pastel/sys/difference_alignedbox_alignedbox.h"
#include "pastel/sys/rectangle_iterator.h"
#include "pastel/sys/range.h"

#include <algorithm>

namespace Pastel
{

	template <typename Type, int N>
	Array<Type, N>::Array()
		: extent_(0)
		, stride_(0)
		, order_(0)
		, size_(0)
		, data_(0)
		, deleteData_(true)
	{
		setStorageOrder(StorageOrder::RowMajor);
	}

	template <typename Type, int N>
	Array<Type, N>::Array(
		const Vector<integer, N>& extent,
		const Alias<Type*>& dataAlias,
		StorageOrder order)
		: extent_(extent)
		, stride_(ofDimension(extent.n()), 0)
		, order_(ofDimension(extent.n()), 0)
		, size_(product(extent))
		, data_(dataAlias)
		, deleteData_(false)
	{
		ENSURE(allGreaterEqual(extent, 0));

		setStorageOrder(order);
	}

	template <typename Type, int N>
	Array<Type, N>::Array(
		const Vector<integer, N>& extent,
		const Type& defaultData,
		StorageOrder order)
		: extent_(extent)
		, stride_(ofDimension(extent.n()), 0)
		, order_(ofDimension(extent.n()), 0)
		, size_(0)
		, data_(0)
		, deleteData_(true)
	{
		allocate(extent);
		setStorageOrder(order);

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
	Array<Type, N>::Array(
		const Array& that)
		: Array(that, that.storageOrder())
	{
	}

	template <typename Type, int N>
	Array<Type, N>::Array(
		const Array& that,
		StorageOrder order)
		: extent_(ofDimension(that.n()), 0)
		, stride_(ofDimension(that.n()), 0)
		, order_(ofDimension(that.n()), 0)
		, size_(0)
		, data_(0)
		, deleteData_(true)
	{
		copyConstruct(that, order);
	}

	template <typename Type, int N>
	Array<Type, N>::Array(Array&& that)
		: Array()
	{
		swap(that);
	}

	template <typename Type, int N>
	Array<Type, N>::Array(
		const Array& that,
		const Vector<integer, N>& extent,
		const Type& defaultData)
		: Array(that, extent, defaultData, that.storageOrder())
	{
	}

	template <typename Type, int N>
	Array<Type, N>::Array(
		const Array& that,
		const Vector<integer, N>& extent,
		const Type& defaultData,
		StorageOrder order)
		: extent_(extent)
		, stride_(ofDimension(extent.n()), 0)
		, order_(ofDimension(extent.n()), 0)
		, size_(0)
		, data_(0)
		, deleteData_(true)
	{
		copyConstructLarger(that, defaultData, order);
	}

	template <typename Type, int N>
	Array<Type, N>::~Array()
	{
		clear();
	}

	template <typename Type, int N>
	void Array<Type, N>::clear()
	{
		if (deleteData_ && data_)
		{
			destruct(data_, data_ + size_);
			deallocate();
		}

		extent_.set(0);
		stride_.set(0);
		// Note: we preserve the storage order.
		size_ = 0;
		deleteData_ = true;
	}

	template <typename Type, int N>
	void Array<Type, N>::swap(Array& that)
	{
		extent_.swap(that.extent_);
		stride_.swap(that.stride_);
		order_.swap(that.order_);
		std::swap(size_, that.size_);
		std::swap(data_, that.data_);
		std::swap(deleteData_, that.deleteData_);
	}

	template <typename Type, int N>
	void Array<Type, N>::setExtent(
		const Vector<integer, N>& extent,
		const Type& defaultData)
	{
		for (integer i = 0;i < n();++i)
		{
			ENSURE_OP(extent[i], >=, 0);
		}

		integer units = product(extent);
		if (units == 0)
		{
			clear();
		}
		else
		{
			if (extent != extent_)
			{

				Array<Type, N> copy(*this, extent,
					defaultData);
				swap(copy);
			}
		}
	}

	template <typename Type, int N>
	const Vector<integer, N>& Array<Type, N>::extent() const
	{
		return extent_;
	}

	template <typename Type, int N>
	void Array<Type, N>::reshape(
		const Vector<integer, N>& extent)
	{
		ENSURE(allGreaterEqual(extent, 0));

		integer size = product(extent);
			
		ENSURE_OP(size, ==, size_);

		if (size > 0)
		{
			extent_ = extent;
			computeStride();
			size_ = size;
		}
	}

	template <typename Type, int N>
	bool Array<Type, N>::empty() const
	{
		return size_ == 0;
	}

	template <typename Type, int N>
	integer Array<Type, N>::size() const
	{
		return size_;
	}

	template <typename Type, int N>
	integer Array<Type, N>::n() const
	{
		return extent_.n();
	}

	template <typename Type, int N>

	Array<Type, N>& Array<Type, N>::operator=(
		const Array& that)
	{
		if (this != &that)
		{
			if (extent() == that.extent())
			{
				assign(that);
			}
			else
			{
				Array copy(that, storageOrder());
				swap(copy);
			}
		}

		return *this;
	}

	template <typename Type, int N>
	Array<Type, N>& Array<Type, N>::operator=(
		Array&& that)
	{
		if (this != &that)
		{
			clear();
			swap(that);
		}

		return *this;
	}

	template <typename Type, int N>
	Array<Type, N>& Array<Type, N>::operator=(
		const std::initializer_list<Type>& that)
	{
		integer n = std::min(size(), (integer)that.size());
		std::copy_n(that.begin(), n, begin());
		return *this;
	}

	template <typename Type, int N>
	Array<Type, N>& Array<Type, N>::operator=(const Type that)
	{
		// The parameter is deliberately taken by value,
		// because a reference could be from this array.
		std::fill(begin(), end(), that);
		return *this;
	}

	template <typename Type, int N>
	void Array<Type, N>::assign(const Array& that)
	{
		ENSURE(extent() == that.extent());

		if (storageOrder() == that.storageOrder())
		{
			// Storage orders match, so simply
			// copy all stuff.
			std::copy(that.cbegin(), that.cend(), 
				begin());
		}
		else
		{
			// Storage orders do not match, so
			// do a point-by-point copy.
			RectangleIterator<N> iter(extent());
			while(!iter.done())
			{
				(*this)(iter.position()) = that(iter.position());
				++iter;
			}
		}
	}

	template <typename Type, int N>
	Type& Array<Type, N>::operator()(
		integer index)
	{
		return (Type&)((const Array&)*this)(index);
	}

	template <typename Type, int N>
	const Type& Array<Type, N>::operator()(
		integer index) const
	{
		PENSURE2(index >= 0 && index < size_, index, size_);

		return *((const Type*)data_ + index);
	}

	template <typename Type, int N>
	Type& Array<Type, N>::operator()(
		const Vector<integer, N>& position)
	{
		return (Type&)((const Array&)*this)(position);
	}

	template <typename Type, int N>
	const Type& Array<Type, N>::operator()(
		const Vector<integer, N>& position) const
	{
		PENSURE(allGreaterEqual(position, 0));
		PENSURE(allLess(position, extent_));
			
		return *address(position);
	}

	template <typename Type, int N>
	SubArray<Type, N> Array<Type, N>::operator()(
		const Vector<integer, N>& min,
		const Vector<integer, N>& max)
	{
		PENSURE(allLess(min, extent_));
		PENSURE(allGreaterEqual(min, 0));
		PENSURE(allLessEqual(max, extent_));
		PENSURE(allGreaterEqual(max, -1));

		integer d = n();

		Vector<integer, N> newStride(stride_);
		for (integer i = 0;i < d;++i)
		{
			if (max[i] < min[i])
			{
				newStride[i] = -newStride[i];
			}
		}

		SubArray<Type, N> result(
			address(min), newStride, mabs(max - min));

		return result;
	}

	template <typename Type, int N>
	ConstSubArray<Type, N> Array<Type, N>::operator()(

		const Vector<integer, N>& min,
		const Vector<integer, N>& max) const
	{
		return ((Array&)*this)(min, max);
	}

	template <typename Type, int N>
	SubArray<Type, N> Array<Type, N>::operator()(
		const Vector<integer, N>& min,
		const Vector<integer, N>& max,
		const Vector<integer, N>& delta)
	{
		PENSURE(allLess(min, extent_));
		PENSURE(allGreaterEqual(min, 0));
		PENSURE(allLessEqual(max, extent_));
		PENSURE(allGreaterEqual(max, -1));
		PENSURE(!anyEqual(delta, 0));

		integer d = n();

		for (integer i = 0;i < d;++i)
		{
			PENSURE((min[i] < max[i]) == (delta[i] > 0));
		}

		SubArray<Type, N> result(

			address(min), stride_ * delta, 
			numbers(mabs(max - min), delta));

		return result;
	}

	template <typename Type, int N>
	ConstSubArray<Type, N> Array<Type, N>::operator()(
		const Vector<integer, N>& min,
		const Vector<integer, N>& max,
		const Vector<integer, N>& delta) const
	{
		return ((Array&)*this)(min, max, delta);
	}

	template <typename Type, int N>
	SubArray<Type, N> Array<Type, N>::operator()()
	{
		SubArray<Type, N> result(
			data_, stride_, extent_);

		return result;
	}

	template <typename Type, int N>
	ConstSubArray<Type, N> Array<Type, N>::operator()() const
	{

		return ((Array&)*this)();
	}

	template <typename Type, int N>
	typename Array<Type, N>::Cursor
		Array<Type, N>::cursor(
		const Vector<integer, N>& position)
	{
		return Cursor(&(*this)(position), stride_);
	}

	template <typename Type, int N>
	typename Array<Type, N>::ConstCursor
		Array<Type, N>::constCursor(
		const Vector<integer, N>& position) const
	{
		return ConstCursor(&(*this)(position), stride_);
	}

	template <typename Type, int N>
	typename Array<Type, N>::Iterator
		Array<Type, N>::begin()
	{
		return data_;
	}

	template <typename Type, int N>
	typename Array<Type, N>::ConstIterator
		Array<Type, N>::cbegin() const
	{
		return data_;
	}

	template <typename Type, int N>
	typename Array<Type, N>::Iterator
		Array<Type, N>::end()
	{
		return data_ + size_;
	}

	template <typename Type, int N>
	typename Array<Type, N>::ConstIterator
		Array<Type, N>::cend() const
	{
		return data_ + size_;
	}

	template <typename Type, int N>
	typename Array<Type, N>::Range
		Array<Type, N>::range()
	{
		return Pastel::range(begin(), end());
	}

	template <typename Type, int N>
	typename Array<Type, N>::ConstRange
		Array<Type, N>::cRange() const
	{
		return Pastel::range(cbegin(), cend());
	}

	template <typename Type, int N>
	typename Array<Type, N>::RowIterator 
		Array<Type, N>::rowBegin(
		const Vector<integer, N>& position, 
		integer axis)
	{
		integer index = 
			dot(position, stride_) - 

			position[axis] * stride_[axis];

		return RowIterator(
			data_ + index, stride_[axis]);
	}

	template <typename Type, int N>
	typename Array<Type, N>::ConstRowIterator 
		Array<Type, N>::cRowBegin(
		const Vector<integer, N>& position, 
		integer axis) const
	{
		return ((Array&)*this).rowBegin(
			position, axis);
	}

	template <typename Type, int N>
	typename Array<Type, N>::RowIterator 
		Array<Type, N>::rowEnd(
		const Vector<integer, N>& position, 
		integer axis)
	{
		return rowBegin(position, axis) + 
			(std::ptrdiff_t)extent_[axis];
	}

	template <typename Type, int N>
	typename Array<Type, N>::ConstRowIterator 
		Array<Type, N>::cRowEnd(
		const Vector<integer, N>& position, 
		integer axis) const
	{
		return ((Array&)*this).rowEnd(
			position, axis);
	}

	template <typename Type, int N>
	typename Array<Type, N>::RowRange
		Array<Type, N>::rowRange(
		const Vector<integer, N>& position, 
		integer axis)
	{
		return Pastel::range(
			rowBegin(position, axis), 
			rowEnd(position, axis));
	}

	template <typename Type, int N>
	typename Array<Type, N>::ConstRowRange
		Array<Type, N>::cRowRange(
		const Vector<integer, N>& position, 
		integer axis) const
	{
		return Pastel::range(
			cRowBegin(position, axis), 
			cRowEnd(position, axis));
	}

	template <typename Type, int N>
	Vector<integer, N> Array<Type, N>::position(
		integer index) const
	{
		PENSURE_OP(index, >=, 0);
		PENSURE_OP(index, <, size());

		return Pastel::position(index, stride_, order_);
	}

	template <typename Type, int N>
	integer Array<Type, N>::index(
		const Vector<integer, N>& position) const
	{
		PENSURE(allGreaterEqual(position, 0));
		PENSURE(allLess(position, extent()));

		return linearIndex(position, stride_);
	}

	template <typename Type, int N>
	const Type* Array<Type, N>::address(
		const Vector<integer, N>& position) const
	{
		return data_ + index(position);
	}

	template <typename Type, int N>
	Type* Array<Type, N>::address(
		const Vector<integer, N>& position)
	{
		return (Type*)((const Array&)*this).address(position);
	}

	template <typename Type, int N>
	const Vector<integer, N>& Array<Type, N>::stride() const
	{
		return stride_;
	}

	template <typename Type, int N>
	void Array<Type, N>::setStorageOrder(
		StorageOrder order)
	{
		integer n = order_.n();

		if (order == StorageOrder::RowMajor)
		{
			for (integer i = 0;i < n;++i)
			{
				order_[i] = i;
			}
		}
		else
		{
			for (integer i = 0;i < n;++i)
			{
				order_[i] = n - i - 1;
			}
		}

		computeStride();
	}

	template <typename Type, int N>
	StorageOrder Array<Type, N>::storageOrder() const
	{
		if (order_[0] == 0)
		{
			return StorageOrder::RowMajor;
		}

		return StorageOrder::ColumnMajor;
	}

	// Private

	template <typename Type, int N>
	void Array<Type, N>::copyConstruct(

		const Array& that,
		StorageOrder order)
	{
		allocate(that.extent_);
		setStorageOrder(order);

		if (order == that.storageOrder())
		{
			// Storage orders match. Simply do copy-initialization.

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
		else
		{
			// Storage orders do not match, so
			// do a point-by-point copy-initialization.

			try
			{
				if (order == StorageOrder::RowMajor)
				{
					copyInitialize<true>(that);
				}
				else
				{
					copyInitialize<false>(that);
				}
			}
			catch(...)
			{
				deallocate();
				throw;
			}
		}
	}

	template <typename Type, int N>
	template <bool ThisInRowOrder>
	void Array<Type, N>::copyInitialize(const Array& that)
	{
		Iterator iter = begin();

		try
		{
			RectangleIterator<N, ThisInRowOrder> thatIter(extent());
			while(iter != end())
			{
				const Type& thatElement = that(thatIter.position());

				// Copy-initialize to 'thatElement'.
				new(&*iter) Type(thatElement);

				++iter;
			}
		}
		catch(...)
		{
			// Rollback the constructions.
			while(iter != begin())
			{
				--iter;
				iter->~Type();
			}

			throw;
		}
	}

	template <typename Type, int N>
	void Array<Type, N>::computeStride()
	{
		integer d = n();

		Vector<integer, N> stride(
			ofDimension(d));

		// Case i = 0.
		{
			integer k = order_[0];
			stride[k] = 1;
		}

		// Case i > 0.
		for (integer i = 1;i < d;++i)
		{
			integer j = order_[i - 1];
			const integer k = order_[i];


			stride[k] = stride[j] * extent_[j];
		}

		stride_ = stride;
	}

	template <typename Type, int N>
	void Array<Type, N>::allocate(
		const Vector<integer, N>& extent)
	{
		ASSERT(data_ == 0);
		ASSERT(size_ == 0);

		integer size = product(extent);
		if (size == 0)
		{
			return;
		}


		Type* data = (Type*)allocateRaw(size * sizeof(Type));

		extent_ = extent;
		// Stride computed elsewhere.
		size_ = size;
		data_ = data;
		deleteData_ = true;
	}

	template <typename Type, int N>
	void Array<Type, N>::deallocate()
	{
		deallocateRaw((void*)data_);
		// Note: order_ is preserved.
		extent_.set(0);
		stride_.set(0);
		size_ = 0;
		data_ = 0;
		deleteData_ = true;
	}

	template <typename Type, int N>
	void Array<Type, N>::construct(
		const AlignedBox<integer, N>& region,
		const Type& defaultData)
	{
		RectangleIterator<N> iter(region.min(), region.max());
		while(!iter.done())
		{
			Type& data = (*this)(iter.position());
				
			new(&data) Type(defaultData);

			++iter;
		}
	}

	template <typename Type, int N>
	void Array<Type, N>::copyConstructLarger(
		const Array& that,
		const Type& defaultData,
		StorageOrder order)
	{
		allocate(extent());
		setStorageOrder(order);

		if (size_ == 0)
		{
			return;
		}

		if (that.size() == 0)
		{
			// Nothing to copy: pure
			// default initialization.
			std::uninitialized_fill_n(data_,
				size_, defaultData);
			return;
		}
			
		if (extent_ == that.extent_ &&
			order_ == that.order_)
		{
			// Same extents. Use the regular copy construction.
			copyConstruct(that, order);
			return;
		}

		// Need a point-by-point copy construction.
		integer d = n();
		Vector<integer, N> minExtent = 
			min(extent_, that.extent_);

		RectangleIterator<N> iter(minExtent);
		while(!iter.done())
		{

			Type* data = &(*this)(iter.position());

			new(data) Type(that(iter.position()));
			++iter;
		}

		// Default construct the region which is not
		// copy constructed, i.e. the difference of the
		// whole region and the copy-region.
		AlignedBox<integer, N> wholeRegion(
			Vector<integer, N>(ofDimension(d), 0),
			extent_);
		AlignedBox<integer, N> copyRegion(
			Vector<integer, N>(ofDimension(d), 0),
			that.extent_);

		// Mentioning RectangleIterator<N> inside the
		// lambda triggers an internal compiler error
		// in Visual Studio 2010 SP1, which is related
		// to mentioning the N. Typedeffing the type
		// in advance here avoids the bug.
		using RectangleIterator_AvoidBug = RectangleIterator<N>;

		integer defaultConstructed = 0;
		try
		{
			difference(wholeRegion, copyRegion, 

				[&](const AlignedBox<integer, N>& region) -> bool
			{
				RectangleIterator_AvoidBug iter(region.min(), region.max());
				while(!iter.done())
				{
					Type& data = (*this)(iter.position());

					new(&data) Type(defaultData);
					++defaultConstructed;

					++iter;
				}
				return true;
			});
		}
		catch(...)
		{
			difference(wholeRegion, copyRegion, 
				[&](const AlignedBox<integer, N>& region) -> bool
			{
				RectangleIterator_AvoidBug iter(region.min(), region.max());
				while(!iter.done() && defaultConstructed > 0)
				{
					Type* data = &(*this)(iter.position());

					destruct(data);
					--defaultConstructed;

					++iter;
				}

				return defaultConstructed > 0;
			});

			throw;
		}
	}

	template <typename Type, int N>
	void swap(Array<Type, N>& left, Array<Type, N>& right)
	{
		left.swap(right);
	}

}

#endif
