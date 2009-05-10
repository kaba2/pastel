#ifndef PASTELSYS_SLICEVIEW_H
#define PASTELSYS_SLICEVIEW_H

#include "pastel/sys/view.h"
#include "pastel/sys/sliceviewcursor.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class ConstSliceView
	{
	public:
		enum
		{
			Dimension = N - 1
		};

		typedef typename Contained_ConstView::Element Element;
		typedef typename Contained_ConstView::ConstReference ConstReference;
		typedef Detail_SliceView::ConstSliceViewCursor<N, typename Contained_ConstView::ConstCursor>
			ConstCursor;

		ConstSliceView(
			const Contained_ConstView& view,
			integer fixedIndex,
			integer pivot)
			: view_(view)
			, pivot_(pivot)
			, extent_(shrink(view.extent(), fixedIndex))
			, fixedIndex_(fixedIndex)
		{
			ENSURE2(fixedIndex >= 0 && fixedIndex < N, fixedIndex, N);
			ENSURE2(pivot >= 0 && pivot < view.extent()[fixedIndex], pivot, view.extent()[fixedIndex]);
		}

		const Vector<PASTEL_ADD_N(N, -1), integer>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Point<PASTEL_ADD_N(N, -1), integer>& position) const
		{
			const Point<N, integer> actualPosition(
				extend(asVector(position), pivot_, fixedIndex_));

			return ConstCursor(view_.constCursor(
				actualPosition), fixedIndex_);
		}

	protected:
		const Contained_ConstView view_;
		const integer pivot_;
		const Vector<PASTEL_ADD_N(N, -1), integer> extent_;
		const integer fixedIndex_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ConstView<N - 1, Input_Element, ConstSliceView<N, Input_ConstView> >
		constSliceView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		integer fixedIndex,
		integer pivot)
	{
		return wrapConstView(
			ConstSliceView<N, Input_ConstView>(view.contained(),
			fixedIndex,
			pivot));
	}

	template <int N, typename Contained_View>
	class SliceView
		: public ConstSliceView<N, Contained_View>
	{
	private:
		typedef ConstSliceView<N, Contained_View> Base;

		using Base::view_;
		using Base::pivot_;
		using Base::fixedIndex_;

	public:
		//using Base::Dimension;
		using typename Base::ConstCursor;
		using typename Base::Element;
		using Base::extent;
		using Base::constCursor;

		typedef typename Contained_View::Reference Reference;
		typedef Detail_SliceView::SliceViewCursor<N, typename Contained_View::Cursor>
			Cursor;

		SliceView(
			const Contained_View& view,
			integer fixedIndex,
			integer pivot)
			: Base(view, fixedIndex, pivot)
		{
			ENSURE2(fixedIndex >= 0 && fixedIndex < N, fixedIndex, N);
			ENSURE2(pivot >= 0 && pivot < view.extent()[fixedIndex], pivot, view.extent()[fixedIndex]);
		}

		Cursor cursor(const Point<PASTEL_ADD_N(N, -1), integer>& position) const
		{
			const Point<N, integer> actualPosition(
				extend(asVector(position), pivot_, fixedIndex_));

			return Cursor(view_.cursor(actualPosition), fixedIndex_);
		}
	};

	template <int N, typename Input_Element, typename Input_View>
	View<N - 1, Input_Element, SliceView<N, Input_View> > sliceView(
		const View<N, Input_Element, Input_View>& view,
		integer fixedIndex,
		integer pivot)
	{
		return wrapView(SliceView<N, Input_View>(
			view.contained(), fixedIndex, pivot));
	}

}

#endif
