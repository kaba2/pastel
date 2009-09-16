// Description: ArrayBase class
// Detail: CRTP base class for Array

#ifndef PASTEL_ARRAYBASE_H
#define PASTEL_ARRAYBASE_H

#include "pastel/sys/array.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/arraybasecursor.h"
#include "pastel/sys/subarray.h"

namespace Pastel
{

	template <typename Type, int N>
	class Array;

	namespace Detail_Array
	{

		template <typename Type, int N>
		class ArrayBase
		{
		public:
			enum
			{
				Dimension = N
			};

			typedef SparseIterator<Type*> RowIterator;
			typedef ConstSparseIterator<const Type*> ConstRowIterator;

			typedef Type Element;
			typedef Type& Reference;
			typedef const Type& ConstReference;

			typedef Type* Iterator;
			typedef const Type* ConstIterator;

			typedef Detail_Array::Cursor<Type, N> Cursor;
			typedef Detail_Array::ConstCursor<Type, N> ConstCursor;

			ArrayBase();
			ArrayBase(
				const Vector<integer, N>& extent,
				const Alias<Type*>& dataAlias);
			ArrayBase(
				const Vector<integer, N>& extent,
				const Type& defaultData = Type());
			ArrayBase(const ArrayBase& that);
			ArrayBase(const ArrayBase& that,
				const Vector<integer, N>& extent,
				const Type& defaultData = Type());
			~ArrayBase();

			void clear();
			void swap(ArrayBase& that);

			//! Sets the extents of the array.
			/*!
			If extent or height is negative, a warning is generated.
			If extent or height is zero, the effect is identical to
			calling "clear()".
			'defaultData' will be used to copy construct the new
			objects. If 'defaultData' is not given, it is defaulted
			to an object constructed using the default constructor.
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

			//! Copies from another array.
			ArrayBase<Type, N>& operator=(const ArrayBase& that);

			//! Returns a reference to the element with the given index.
			Type& operator()(integer index);

			//! Returns a const reference to the element with the given index.
			const Type& operator()(integer index) const;

			//! Returns a reference to the element (x, y)
			Type& operator()(const Vector<integer, N>& position);

			//! Returns a const reference to the element (x, y)
			const Type& operator()(const Vector<integer, N>& position) const;

			//! Filling the array with a comma-delimited list of elements.
			CommaFiller<Type, Iterator> operator|=(const Type& that);

			//! Sets all the elements to the given value.
			/*!
			The parameter is deliberately taken by value,
			because a reference could be from this array.
			*/
			ArrayBase<Type, N>& operator=(const Type that);

			//! Returns the sub-array in the range [min, max].
			SubArray<N, Type> operator()(
				const Vector<integer, N>& min,
				const Vector<integer, N>& max);

			//! Returns the sub-array in the range [min, max].
			ConstSubArray<N, Type> operator()(
				const Vector<integer, N>& min,
				const Vector<integer, N>& max) const;

			//! Returns a sparse sub-array in the range [min, max].
			SubArray<N, Type> operator()(
				const Vector<integer, N>& min,
				const Vector<integer, N>& max,
				const Vector<integer, N>& delta);

			//! Returns a sparse sub-array in the range [min, max].
			ConstSubArray<N, Type> operator()(
				const Vector<integer, N>& min,
				const Vector<integer, N>& max,
				const Vector<integer, N>& delta) const;

			SubArray<N, Type> operator()();

			ConstSubArray<N, Type> operator()() const;

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

			Type* rawBegin()
			{
				return data_;
			}

			const Type* rawBegin() const
			{
				return data_;
			}

			Type* rawEnd()
			{
				return data_ + size_;
			}

			const Type* rawEnd() const
			{
				return data_ + size_;
			}

			//! Memory address of the given position.
			const Type* address(const Vector<integer, N>& position) const;

			//! Memory address of the given position.
			Type* address(const Vector<integer, N>& position);

		private:
			void allocate(
				const Vector<integer, N>& extent);
			void deallocate();
			void copyConstruct(
				const ArrayBase& that,
				const Type& defaultData);

			Vector<integer, N> extent_;
			Vector<integer, N> stride_;
			integer size_;
			Type* data_;
			bool deleteData_;
		};

	}

}

#include "pastel/sys/arraybase.hpp"

#endif
