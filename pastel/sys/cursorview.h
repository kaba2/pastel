#ifndef PASTELSYS_CURSORVIEW_H
#define PASTELSYS_CURSORVIEW_H

#include "pastel/sys/view.h"

namespace Pastel
{

	template <int N, typename ViewConstCursor>
	class ConstCursorView
	{
	public:
		enum
		{
			Dimension = N
		};

		typedef typename ViewConstCursor::Element Element;
		typedef typename ViewConstCursor::ConstReference ConstReference;
		typedef ViewConstCursor ConstCursor;

		ConstCursorView()
			: cursor_()
			, extent_(0)
		{
		}

		ConstCursorView(
			const ViewConstCursor& cursor,
			const Vector<N, integer>& extent)
			: cursor_(cursor)
			, extent_(extent)
		{
		}

		const Vector<N, integer>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Point<N, integer>& position) const
		{
			ConstCursor result = cursor_;
			result.move(asVector(position));

			return result;
		}

	private:
		const ViewConstCursor cursor_;
		const Vector<N, integer> extent_;
	};

	template <int N, typename ViewConstCursor>
	ConstView<N, typename ViewConstCursor::Element, ConstCursorView<N, ViewConstCursor> > 
		constCursorView(
		const ViewConstCursor& cursor, 
		const Vector<N, integer>& extent)
	{
		return wrapConstView(ConstCursorView<ViewConstCursor>(cursor, extent));
	}

	template <int N, typename ViewCursor>
	class CursorView
		: public ConstCursorView<N, ViewCursor>
	{
	private:
		typedef ConstCursorView<N, ViewCursor> Base;

	public:
		using typename Base::Element;
		using typename Base::ConstReference;
		using typename Base::ConstCursor;
		using Base::extent;
		using Base::constCursor;
		//using Base::Dimension;

		typedef typename ViewCursor::Reference Reference;

		typedef ViewCursor Cursor;

		CursorView()
			: Base()
		{
		}

		CursorView(
			const ViewCursor& cursor,
			const Vector<N, integer>& extent)
			: Base(cursor, extent)
		{
		}

		Cursor cursor(
			const Point<N, integer>& position) const
		{
			Cursor result = cursor_;
			result.move(asVector(position));
			return result;
		}
	};

	template <int N, typename ViewCursor>
	View<N, typename ViewCursor::Element, CursorView<N, ViewCursor> > cursorView(
		const ViewCursor& cursor, 
		const Vector<N, integer>& extent)
	{
		return wrapView(CursorView<N, ViewCursor>(cursor, extent));
	}

}

#endif
