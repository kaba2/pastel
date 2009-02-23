#ifndef PASTELSYS_ARRAYBASE_H
#define PASTELSYS_ARRAYBASE_H

#include "pastel/sys/array.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"
#include "pastel/sys/arraybasecursor.h"

namespace Pastel
{

	namespace Detail_Array
	{

		template <int N, typename Type, typename Derived>
		class ArrayBase
		{
		public:
			enum
			{
				Dimension = N
			};

			typedef Type Element;
			typedef Type& Reference;
			typedef const Type& ConstReference;

			typedef Detail_Array::Cursor<N, Type> Cursor;
			typedef Detail_Array::ConstCursor<N, Type> ConstCursor;

			ArrayBase();
			ArrayBase(
				const Vector<N, integer>& extent,
				const Type& defaultData = Type());
			ArrayBase(const ArrayBase<N, Type, Derived>& that);
			ArrayBase(const ArrayBase<N, Type, Derived>& that,
				const Vector<N, integer>& extent,
				const Type& defaultData = Type());
			~ArrayBase();

			void clear();
			void swap(ArrayBase<N, Type, Derived>& that);

			//! Sets the extent and height of the array.
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

			//! Returns allEqual(extent(), 0).
			bool empty() const;

			//! Returns the number of elements in the array.
			integer size() const;

			//! Copies from another array.
			ArrayBase<N, Type, Derived>& operator=(const ArrayBase<N, Type, Derived>& that);

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

		private:
			void construct(
				const Vector<N, integer>& extent,
				const Type& defaultData);
			void copy(const ArrayBase<N, Type, Derived>& that,
				const Vector<N, integer>& extent,
				const Type& defaultData);

			Vector<N, integer> extent_;
			Vector<N, integer> factor_;
			integer size_;
			Type* data_;
		};

	}

}

#include "pastel/sys/arraybase.hpp"

#endif
