// Description: CursorView class
// Detail: A view to a cursor range
// Documentation: view.txt

#ifndef PASTEL_CURSORVIEW_H
#define PASTEL_CURSORVIEW_H

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
			const Vector<integer, N>& extent)
			: cursor_(cursor)
			, extent_(extent)
		{
		}

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Vector<integer, N>& position) const
		{
			ConstCursor result = cursor_;
			result.move(position);

			return result;
		}

	private:
		const ViewConstCursor cursor_;
		const Vector<integer, N> extent_;
	};

	template <int N, typename ViewConstCursor>
	ConstView<N, typename ViewConstCursor::Element, ConstCursorView<N, ViewConstCursor> >
		constCursorView(
		const ViewConstCursor& cursor,
		const Vector<integer, N>& extent)
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
		typedef typename Base::Element Element;
		typedef typename Base::ConstReference ConstReference;
		typedef typename Base::ConstCursor ConstCursor;
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
			const Vector<integer, N>& extent)
			: Base(cursor, extent)
		{
		}

		Cursor cursor(
			const Vector<integer, N>& position) const
		{
			Cursor result = cursor_;
			result.move(position);
			return result;
		}
	};

	template <int N, typename ViewCursor>
	View<N, typename ViewCursor::Element, CursorView<N, ViewCursor> > cursorView(
		const ViewCursor& cursor,
		const Vector<integer, N>& extent)
	{
		return wrapView(CursorView<N, ViewCursor>(cursor, extent));
	}

}

#endif
