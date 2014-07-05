// Description: CursorView class
// Detail: A view to a cursor range
// Documentation: concrete_views.txt

#ifndef PASTELSYS_CURSORVIEW_H
#define PASTELSYS_CURSORVIEW_H

#include "pastel/sys/view.h"

namespace Pastel
{

	template <int N, typename ViewConstCursor>
	class ConstCursorView
	{
	public:
		static PASTEL_CONSTEXPR int Dimension = N;

		using Element = typename ViewConstCursor::Element;
		using ConstReference = typename ViewConstCursor::ConstReference;
		using ConstCursor = ViewConstCursor;

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

	protected:
		const ViewConstCursor cursor_;
		const Vector<integer, N> extent_;
	};

	template <int N, typename ViewConstCursor>
	ConstView<N, typename ViewConstCursor::Element, ConstCursorView<N, ViewConstCursor> >
		constCursorView(
		const ViewConstCursor& cursor,
		const Vector<integer, N>& extent)
	{
		return wrapConstView(ConstCursorView<N, ViewConstCursor>(cursor, extent));
	}

	template <int N, typename ViewCursor>
	class CursorView
		: public ConstCursorView<N, ViewCursor>
	{
	private:
		using Base = ConstCursorView<N, ViewCursor>;

	public:
		using Element = typename Base::Element;
		using ConstReference = typename Base::ConstReference;
		using ConstCursor = typename Base::ConstCursor;
		using Base::extent;
		using Base::constCursor;
		//using Base::Dimension;

		using Reference = typename ViewCursor::Reference;

		using Cursor = ViewCursor;

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
			Cursor result = Base::cursor_;
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
