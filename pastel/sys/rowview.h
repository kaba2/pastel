#ifndef PASTELSYS_ROWVIEW_H
#define PASTELSYS_ROWVIEW_H

#include "pastel/sys/view.h"
#include "pastel/sys/rowviewcursor.h"

namespace Pastel
{

	template <int N, int FreeIndex, typename Contained_ConstView>
	class ConstRowView
	{
	public:
		enum
		{
			Dimension = 1
		};

		typedef typename Contained_ConstView::Element Element;
		typedef typename Contained_ConstView::ConstReference ConstReference;
		typedef Detail_RowView::ConstRowViewCursor<FreeIndex, typename Contained_ConstView::ConstCursor>
			ConstCursor;

		ConstRowView(
			const Contained_ConstView& view,
			const Point<N, integer>& pivot)
			: view_(view)
			, pivot_(pivot)
			, extent_(view.extent()[FreeIndex])
		{
		}

		const Vector<1, integer>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Point<1, integer>& position) const
		{
			Point<N, integer> actualPosition = pivot_;
			actualPosition[FreeIndex] = position[0];

			return ConstCursor(view_.constCursor(
				actualPosition));
		}

	protected:
		const Contained_ConstView view_;
		const Point<N, integer> pivot_;
		const Vector<1, integer> extent_;
	};

	template <int FreeIndex, int N, typename Input_Element, typename Input_ConstView>
	ConstView<1, Input_Element, ConstRowView<N, FreeIndex, Input_ConstView> > 
		constRowView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const Point<N, integer>& pivot)
	{
		return wrapConstView(
			ConstRowView<N, FreeIndex, Input_ConstView>(view.contained(), pivot));
	}

	template <int N, int FreeIndex, typename Contained_View>
	class RowView
		: public ConstRowView<N, FreeIndex, Contained_View>
	{
	private:
		typedef ConstRowView<N, FreeIndex, Contained_View> Base;

		using Base::view_;
		using Base::pivot_;
	
	public:
		//using Base::Dimension;
		using typename Base::ConstCursor;
		using typename Base::Element;
		using Base::extent;
		using Base::constCursor;

		typedef typename Contained_View::Reference Reference;
		typedef Detail_RowView::RowViewCursor<FreeIndex, typename Contained_View::Cursor>
			Cursor;

		RowView(
			const Contained_View& view,
			const Point<N, integer>& pivot)
			: Base(view, pivot)
		{
		}

		Cursor cursor(const Point<1, integer>& position) const
		{
			Point<N, integer> actualPosition = pivot_;
			actualPosition[FreeIndex] = position[0];

			return Cursor(view_.cursor(actualPosition));
		}
	};

	template <int FreeIndex, int N, typename Input_Element, typename Input_View>
	View<1, Input_Element, RowView<N, FreeIndex, Input_View> > rowView(
		const View<N, Input_Element, Input_View>& view,
		const Point<N, integer>& pivot)
	{
		return wrapView(RowView<N, FreeIndex, Input_View>(
			view.contained(), pivot));
	}

}

#endif
