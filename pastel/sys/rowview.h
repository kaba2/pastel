// Description: RowView class
// Detail: An adapter view for a 1-dimensional subset
// Documentation: concrete_views.txt

#ifndef PASTEL_ROWVIEW_H
#define PASTEL_ROWVIEW_H

#include "pastel/sys/view.h"
#include "pastel/sys/rowviewcursor.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class ConstRowView
	{
	public:
		enum
		{
			Dimension = 1
		};

		typedef typename Contained_ConstView::Element Element;
		typedef typename Contained_ConstView::ConstReference ConstReference;
		typedef RowView_::ConstRowViewCursor<typename Contained_ConstView::ConstCursor>
			ConstCursor;

		ConstRowView(
			const Contained_ConstView& view,
			integer freeIndex,
			const Vector<integer, N>& pivot)
			: view_(view)
			, pivot_(pivot)
			, extent_(view.extent()[freeIndex])
			, freeIndex_(freeIndex)
		{
		}

		const Vector<integer, 1>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Vector<integer, 1>& position) const
		{
			Vector<integer, N> actualPosition = pivot_;
			actualPosition[freeIndex_] = position[0];

			return ConstCursor(view_.constCursor(
				actualPosition), freeIndex_);
		}

	protected:
		const Contained_ConstView view_;
		const Vector<integer, N> pivot_;
		const Vector<integer, 1> extent_;
		const integer freeIndex_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ConstView<1, Input_Element, ConstRowView<N, Input_ConstView> >
		constRowView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		integer freeIndex,
		const Vector<integer, N>& pivot)
	{
		return wrapConstView(
			ConstRowView<N, Input_ConstView>(view.contained(),
			freeIndex,
			pivot));
	}

	template <int N, typename Contained_View>
	class RowView
		: public ConstRowView<N, Contained_View>
	{
	private:
		typedef ConstRowView<N, Contained_View> Base;

		using Base::view_;
		using Base::pivot_;
		using Base::freeIndex_;

	public:
		//using Base::Dimension;
		typedef typename Base::ConstCursor ConstCursor;
		typedef typename Base::Element Element;
		using Base::extent;
		using Base::constCursor;

		typedef typename Contained_View::Reference Reference;
		typedef RowView_::RowViewCursor<typename Contained_View::Cursor>
			Cursor;

		RowView(
			const Contained_View& view,
			integer freeIndex,
			const Vector<integer, N>& pivot)
			: Base(view, freeIndex, pivot)
		{
		}

		Cursor cursor(const Vector<integer, 1>& position) const
		{
			Vector<integer, N> actualPosition = pivot_;
			actualPosition[freeIndex_] = position[0];

			return Cursor(view_.cursor(actualPosition), freeIndex_);
		}
	};

	template <int N, typename Input_Element, typename Input_View>
	View<1, Input_Element, RowView<N, Input_View> > rowView(
		const View<N, Input_Element, Input_View>& view,
		integer freeIndex,
		const Vector<integer, N>& pivot)
	{
		return wrapView(RowView<N, Input_View>(
			view.contained(), freeIndex, pivot));
	}

}

#endif
