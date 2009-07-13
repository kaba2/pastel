// Description: ArrayBase class
// Detail: CRTP base class for Array

#ifndef PASTEL_ARRAYBASE_H
#define PASTEL_ARRAYBASE_H

#include "pastel/sys/array.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"
#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/arraybasecursor.h"

namespace Pastel
{

	template <int N, typename Type>
	class Array;

	namespace Detail_Array
	{

		template <int N, typename Type>
		class ArrayBase
		{
		public:
			enum
			{
				Dimension = N
			};

			typedef SparseIterator<Type> RowIterator;
			typedef ConstSparseIterator<Type> ConstRowIterator;

			typedef Type Element;
			typedef Type& Reference;
			typedef const Type& ConstReference;

			typedef Type* Iterator;
			typedef const Type* ConstIterator;

			typedef Detail_Array::Cursor<N, Type> Cursor;
			typedef Detail_Array::ConstCursor<N, Type> ConstCursor;

			ArrayBase();
			ArrayBase(
				const Vector<N, integer>& extent,
				const Alias<Type*>& dataAlias);
			ArrayBase(
				const Vector<N, integer>& extent,
				const Type& defaultData = Type());
			ArrayBase(const ArrayBase& that);
			ArrayBase(const ArrayBase& that,
				const Vector<N, integer>& extent,
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
				const Vector<N, integer>& extent,
				const Type& defaultData = Type());

			//! Returns the extent of the array.
			const Vector<N, integer>& extent() const;

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
				const Vector<N, integer>& extent);

			//! Returns (size() == 0).
			bool empty() const;

			//! Returns the number of elements in the array.
			integer size() const;

			//! Sets all the elements to the given value.
			/*!
			The element is deliberately taken by value,
			because a reference could be from this array.
			*/
			void set(const Type that);

			//! Copies from another array.
			ArrayBase<N, Type>& operator=(const ArrayBase& that);

			//! Returns a reference to the element with the given index.
			Type& operator()(integer index);

			//! Returns a const reference to the element with the given index.
			const Type& operator()(integer index) const;

			//! Returns a reference to the element (x, y)
			Type& operator()(const Point<N, integer>& position);

			//! Returns a const reference to the element (x, y)
			const Type& operator()(const Point<N, integer>& position) const;

			//! Returns a cursor to the given position.
			Cursor cursor(const Point<N, integer>& position);

			//! Returns a cursor to the given position.
			ConstCursor constCursor(const Point<N, integer>& position) const;

			// Iterators

			//! Returns an iterator to the first element.
			Iterator begin();

			//! Returns an iterator to the first element.
			ConstIterator begin() const;

			//! Returns an iterator to one-past-end element.
			Iterator end();

			//! Returns an iterator to one-past-end element.
			ConstIterator end() const;

			RowIterator rowBegin(
				const Point<N, integer>& position, 
				integer axis);
			ConstRowIterator rowBegin(
				const Point<N, integer>& position, 
				integer axis) const;

			RowIterator rowEnd(
				const Point<N, integer>& position, 
				integer axis);
			ConstRowIterator rowEnd(
				const Point<N, integer>& position, 
				integer axis) const;

			Type* data()
			{
				return data_;
			}

			const Type* data() const
			{
				return data_;
			}

		private:
			void allocate(
				const Vector<N, integer>& extent);
			void deallocate();
			void copyConstruct(
				const ArrayBase& that,
				const Type& defaultData);

			Vector<N, integer> extent_;
			Vector<N, integer> factor_;
			integer size_;
			Type* data_;
			bool deleteData_;
		};

	}

}

#include "pastel/sys/arraybase.hpp"

#endif
