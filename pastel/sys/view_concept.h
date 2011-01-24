#ifndef PASTEL_VIEW_CONCEPT_H
#define PASTEL_VIEW_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	namespace View_Concept
	{

		class ConstView_ConstCursor
		{
		private:
			typedef ConstView_ConstCursor ConstCursor;

		public:
			//! The type of the dereferenced value.
			typedef UserDefinedType Element;

			//! Increment the position on the 'index':th axis.
			ConstCursor& increment(integer index);
			//! Decrement the position on the 'index':th axis.
			ConstCursor& decrement(integer index);
			//! Move the position on the 'index':th axis.
			ConstCursor& move(integer index, integer amount);
			//! Move the position on all axes.
			ConstCursor& move(const Vector<integer, N>& amount);

			//! Returns a pointer to current value.
			const Element* operator->() const;
			//! Returns a reference to current value.
			const Element& operator*() const;
		};

		class ConstView
		{
		public:
			enum 
			{
				//! The dimension of the array.
				Dimension = UserDefinedInteger,
				//! An alias for the dimension
				N = Dimension
			};

			//! The cursor type, modeling ConstView_ConstCursor.
			typedef UserDefinedType ConstCursor;
			//! The type of the contained data.
			typedef UserDefinedType Element;

			//! Returns the extent of the array.
			const Vector<integer, N>& extent() const;

			//! Returns a cursor to the given position.
			ConstCursor constCursor(
				const Vector<integer, N>& position) const;
		};

		class View
			: public ConstView
		{
		public:
			typedef UserDefinedType Cursor;

			//! Returns a cursor to the given position.
			Cursor cursor(
				const Vector<integer, N>& position) const;
		};

	}

}

#endif
