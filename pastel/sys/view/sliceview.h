// Description: SliceView class
// Detail: An adapter view for an (n-1)-dimensional subset
// Documentation: concrete_views.txt

#ifndef PASTELSYS_SLICEVIEW_H
#define PASTELSYS_SLICEVIEW_H

#include "pastel/sys/view/view.h"
#include "pastel/sys/view/sliceviewcursor.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class ConstSliceView
	{
	public:
		static constexpr int Dimension = N - 1;

		using Element = typename Contained_ConstView::Element;
		using ConstReference = typename Contained_ConstView::ConstReference;
		typedef SliceView_::ConstSliceViewCursor<N, typename Contained_ConstView::ConstCursor>
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

		const Vector<integer, SubN<N>>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Vector<integer, SubN<N>>& position) const
		{
			Vector<integer, N> actualPosition(
				extend(position, pivot_, fixedIndex_));

			return ConstCursor(view_.constCursor(
				actualPosition), fixedIndex_);
		}

	protected:
		Contained_ConstView view_;
		integer pivot_;
		Vector<integer, SubN<N>> extent_;
		integer fixedIndex_;
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
		using Base = ConstSliceView<N, Contained_View>;

		using Base::view_;
		using Base::pivot_;
		using Base::fixedIndex_;

	public:
		//using Base::Dimension;
		using ConstCursor = typename Base::ConstCursor;
		using Element = typename Base::Element;
		using Base::extent;
		using Base::constCursor;

		using Reference = typename Contained_View::Reference;
		typedef SliceView_::SliceViewCursor<N, typename Contained_View::Cursor>
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

		Cursor cursor(const Vector<integer, SubN<N>>& position) const
		{
			Vector<integer, N> actualPosition(
				extend(position, pivot_, fixedIndex_));

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
