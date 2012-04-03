// Description: Array class
// Detail: An n-dimensional array

#ifndef PASTEL_ARRAY_H
#define PASTEL_ARRAY_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/subarray.h"
#include "pastel/sys/iterator_range.h"
#include "pastel/sys/alignedbox.h"

#include "pastel/sys/array_cursor.h"

namespace Pastel
{

	class StorageOrder
	{
	public:
		enum Enum
		{
			RowMajor,
			ColumnMajor
		};
	};

	//! A multi-dimensional array.
	template <typename Type, int N = 2>
	class Array
	{
	public:
		// Using default assignment.
		// Using default destructor.

		enum
		{
			Dimension = N
		};

		typedef SparseIterator<Type*> RowIterator;
		typedef ConstSparseIterator<const Type*> ConstRowIterator;
		typedef typename IteratorToRange<RowIterator>::type RowRange;
		typedef typename IteratorToRange<ConstRowIterator>::type ConstRowRange;

		typedef Type Element;
		typedef Type& Reference;
		typedef const Type& ConstReference;

		typedef Type* Iterator;
		typedef const Type* ConstIterator;

		typedef typename IteratorToRange<Iterator>::type Range;
		typedef typename IteratorToRange<ConstIterator>::type ConstRange;

		typedef Array_Private::Cursor<Type, N> Cursor;
		typedef Array_Private::ConstCursor<Type, N> ConstCursor;

		//! Constructs an empty array.
		Array();

		//! Construct an array of given extents using existing memory.
		Array(
			const Vector<integer, N>& extent,
			const Alias<Type*>& dataAlias,
			StorageOrder::Enum order = StorageOrder::RowMajor);

		//! Constructs an array of given extents filled with given data.
		explicit Array(
			const Vector<integer, N>& extent,
			const Type& defaultData = Type(),
			StorageOrder::Enum order = StorageOrder::RowMajor);

		//! Copy-constructs an array.
		/*!
		This is equivalent to
		Array(that, that.storageOrder()).
		*/
		Array(const Array& that);
		
		//! Copy-constructs an array.
		Array(const Array& that,
			StorageOrder::Enum order);

		//! Move-constructs an array.
		Array(Array&& that);

		//! Copy constructs an array with given extents.
		/*!
		This is equivalent to
		Array(that, extent, defaultData, that.storageOrder()).
		*/
		Array(
			const Array& that,
			const Vector<integer, N>& extent,
			const Type& defaultData = Type());

		//! Copy constructs an array with given extents.
		Array(
			const Array& that,
			const Vector<integer, N>& extent,
			const Type& defaultData,
			StorageOrder::Enum order);

		//! Destruct an array.
		~Array();

		//! Frees all memory and sets extents to zero.
		void clear();

		//! Swaps contents with another array.
		void swap(Array& that);

		//! Sets the extents of the array.
		/*!
		Preconditions:
		allGreaterEqual(extent, 0)

		If the new extents contain no elements, the
		effect is identical to calling "clear()".
		'defaultData' will be used to copy construct the newly
		created elements. If 'defaultData' is not given, it is 
		defaulted to an object constructed using the default 
		constructor.
		*/
		void setExtent(
			const Vector<integer, N>& extent,
			const Type& defaultData = Type());

		//! Returns the extent of the array.
		const Vector<integer, N>& extent() const;

		//! Sets the extents of the array using the existing data.
		/*!
		Preconditions:
		allGreaterEqual(extent, 0)
		product(extent) == product(extent())

		Time complexity:
		O(d) where d is the dimension.

		Because the data is stored sequentially in row-major
		order, it is possible without reallocation to change
		the extents of the array in such a way that the
		total number of elements stays constant. The data
		retains its row-major storage convention.
		*/
		void reshape(
			const Vector<integer, N>& extent);

		//! Returns (size() == 0).
		bool empty() const;

		//! Returns the number of elements in the array.
		integer size() const;

		//! Returns the dimension of the array.
		integer dimension() const;

		//! Copy-assigns from another array.
		/*!
		Preserves storage order.
		Preserves storage address if and only if
		the extents of the arrays are equal.
		*/
		Array<Type, N>& operator=(const Array& that);

		//! Move-assigns from another array.
		/*!
		Does not preserve storage order.
		Does not preserve storage address.
		*/
		Array<Type, N>& operator=(Array&& that);

		//! Copies data from another array.
		/*!
		Preconditions:
		extent() == that.extent()

		Preserves storage order.
		Preserves storage address.
		*/
		void assign(const Array& that);

		//! Returns a reference to the element with the given index.
		Type& operator()(integer index);

		//! Returns a const reference to the element with the given index.
		const Type& operator()(integer index) const;

		//! Returns a reference to the given element.
		Type& operator()(const Vector<integer, N>& position);

		//! Returns a const reference to the given element.
		const Type& operator()(const Vector<integer, N>& position) const;

		//! Allows to fill the array with a comma-delimited list of elements.
		CommaFiller<Type, Iterator> operator|=(const Type& that);

		//! Sets all the elements to the given value.
		/*!
		The parameter is deliberately taken by value,
		because a reference could be from this array.
		*/
		Array<Type, N>& operator=(const Type that);

		//! Returns the sub-array in the range [min, max].
		SubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max);

		//! Returns the sub-array in the range [min, max].
		ConstSubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max) const;

		//! Returns a sparse sub-array in the range [min, max].
		SubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max,
			const Vector<integer, N>& delta);

		//! Returns a sparse sub-array in the range [min, max].
		ConstSubArray<Type, N> operator()(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max,
			const Vector<integer, N>& delta) const;

		//! Returns a sub-array of the whole array.
		SubArray<Type, N> operator()();

		//! Returns a non-mutable sub-array of the whole array.
		ConstSubArray<Type, N> operator()() const;

		//! Returns a cursor to the given position.
		Cursor cursor(const Vector<integer, N>& position);

		//! Returns a cursor to the given position.
		ConstCursor constCursor(const Vector<integer, N>& position) const;

		// Iterators

		//! An iterator to the first element.
		Iterator begin();

		//! An iterator to the first element.
		ConstIterator begin() const;

		//! An iterator to one-past-end element.
		Iterator end();

		//! An iterator to one-past-end element.
		ConstIterator end() const;

		//! An iterator range to all elements.
		Range range();

		//! An iterator range to all elements.
		ConstRange range() const;

		//! An iterator to the first element of a row.
		RowIterator rowBegin(
			const Vector<integer, N>& position, 
			integer axis);

		//! An iterator to the first element of a row.
		ConstRowIterator rowBegin(
			const Vector<integer, N>& position, 
			integer axis) const;

		//! An iterator to the one-past end element of a row.
		RowIterator rowEnd(
			const Vector<integer, N>& position, 
			integer axis);

		//! An iterator to the one-past end element of a row.
		ConstRowIterator rowEnd(
			const Vector<integer, N>& position, 
			integer axis) const;

		//! An iterator range to the elements of a row.
		RowRange rowRange(
			const Vector<integer, N>& position, 
			integer axis);

		//! An iterator range to the elements of a row.
		ConstRowRange rowRange(
			const Vector<integer, N>& position, 
			integer axis) const;

		//! Returns a raw memory address of the first element.
		Type* rawBegin()
		{
			return data_;
		}

		//! Returns a raw memory address of the first element.
		const Type* rawBegin() const
		{
			return data_;
		}

		//! Returns a raw memory address of the one-past-last element.
		Type* rawEnd()
		{
			return data_ + size_;
		}

		//! Returns a raw memory address of the one-past-last element.
		const Type* rawEnd() const
		{
			return data_ + size_;
		}

		//! Returns the position at the given linear index.
		/*!
		Preconditions:
		index >= 0
		index < size()
		*/
		Vector<integer, N> position(integer index) const;

		//! Returns the linear index given a position.
		/*!
		Preconditions:
		allGreaterEqual(position, 0)
		allLess(position, extent())
		*/
		integer index(const Vector<integer, N>& position) const;

		//! Returns the memory address of the given position.
		const Type* address(const Vector<integer, N>& position) const;

		//! Returns the memory address of the given position.
		Type* address(const Vector<integer, N>& position);

		//! Returns the stride.
		const Vector<integer, N>& stride() const;

		//! Sets the storage order.
		void setStorageOrder(
			StorageOrder::Enum order);

		//! Returns the storage order.
		StorageOrder::Enum storageOrder() const;

		// 1-dimensional special functions.
		// If only we had static_if...

		integer width() const
		{
			return extent()[0];
		}

		Cursor cursor(integer x)
		{
			return cursor(Vector<integer, N>(x));
		}

		ConstCursor constCursor(integer x) const
		{
			return constCursor(Vector<integer, N>(x));
		}

		// 2-dimensional special functions

		integer height() const
		{
			return extent()[1];
		}

		Type& operator()(integer x, integer y)
		{
			return operator()(Vector<integer, N>(x, y));
		}

		const Type& operator()(integer x, integer y) const
		{
			return operator()(Vector<integer, N>(x, y));
		}

		Cursor cursor(integer x, integer y)
		{
			return cursor(Vector<integer, N>(x, y));
		}

		ConstCursor constCursor(integer x, integer y) const
		{
			return constCursor(Vector<integer, N>(x, y));
		}

		RowRange rowRange(integer y)
		{
			return rowRange(Vector<integer, N>(0, y), 0);
		}

		ConstRowRange rowRange(integer y) const
		{
			return rowRange(Vector<integer, N>(0, y), 0);
		}

		RowIterator rowBegin(integer y)
		{
			return rowBegin(Vector<integer, N>(0, y), 0);
		}

		ConstRowIterator rowBegin(integer y) const
		{
			return rowBegin(Vector<integer, N>(0, y), 0);
		}

		RowIterator rowEnd(integer y)
		{
			return rowEnd(Vector<integer, N>(0, y), 0);
		}

		ConstRowIterator rowEnd(integer y) const
		{
			return rowEnd(Vector<integer, N>(0, y), 0);
		}

		RowIterator columnBegin(integer x)
		{
			return rowBegin(Vector<integer, N>(x, 0), 1);
		}

		ConstRowIterator columnBegin(integer x) const
		{
			return rowBegin(Vector<integer, N>(x, 0), 1);
		}

		RowIterator columnEnd(integer x)
		{
			return rowEnd(Vector<integer, N>(x, 0), 1);
		}

		ConstRowIterator columnEnd(integer x) const
		{
			return rowEnd(Vector<integer, N>(x, 0), 1);
		}

		// 3-dimensional special functions

		integer depth() const
		{
			return extent()[2];
		}

		Type& operator()(integer x, integer y, integer z)
		{
			return operator()(Vector<integer, N>(x, y, z));
		}

		const Type& operator()(integer x, integer y, integer z) const
		{
			return operator()(Vector<integer, N>(x, y, z));
		}

		Cursor cursor(integer x, integer y, integer z)
		{
			return cursor(Vector<integer, N>(x, y, z));
		}

		ConstCursor constCursor(integer x, integer y, integer z) const
		{
			return constCursor(Vector<integer, N>(x, y, z));
		}

	private:
		//! Copy constructs an array.
		void copyConstruct(
			const Array& that,
			StorageOrder::Enum order);

		template <bool ThisInRowOrder>
		void copyInitialize(const Array& that);

		void computeStride();

		void allocate(
			const Vector<integer, N>& extent);

		void deallocate();

		void construct(
			const AlignedBox<integer, N>& region,
			const Type& defaultData);

		void copyConstructLarger(
			const Array& that,
			const Type& defaultData,
			StorageOrder::Enum order);

		/*
		extent_:
		The extents of the array.

		stride_:
		The multiplicative factors by
		which each coordinate is scaled before
		being summed to form a linear index.
		A linear index is computed by the dot
		product of 'stride_' and a position.

		order_:
		The storage order in which the data is
		linearized. A permutation of integers
		from 0 to dimension() - 1.			
		The order_[i] dimension changes 
		faster than order_[i + 1], when increasing
		a linear index.

		size_:
		The number of elements in the array.
		Equal to product(extent_).

		data_:
		A pointer to a memory region containing
		the elements of this array.

		deleteData_:
		If true, then the memory region in 'data_'
		is deallocated when it is no longer needed.
		Otherwise it is never deallocated.
		Used for aliasing.
		*/

		Vector<integer, N> extent_;
		Vector<integer, N> stride_;
		Vector<integer, N> order_;
		integer size_;
		Type* data_;
		bool deleteData_;
	};

	template <typename Type, int N>
	void swap(Array<Type, N>& left, Array<Type, N>& right);

}

#include "pastel/sys/array.hpp"

#endif
