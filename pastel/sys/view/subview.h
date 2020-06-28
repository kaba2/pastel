// Description: SubView class
// Detail: An adapter view for an n-dimensional subset
// Documentation: concrete_views.txt

#ifndef PASTELSYS_SUBVIEW_H
#define PASTELSYS_SUBVIEW_H

#include "pastel/sys/alignedbox.h"

#include "pastel/sys/view/view.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class ConstSubView
	{
	public:
		static constexpr int Dimension = N;

		using Element = typename Contained_ConstView::Element;
		using ConstReference = typename Contained_ConstView::ConstReference;
		using ConstCursor = typename Contained_ConstView::ConstCursor;

		ConstSubView()
			: view_()
			, window_()
			, extent_()
		{
		}

		ConstSubView(
			const Contained_ConstView& view,
			const AlignedBox<integer, N>& window)
			: view_(view)
			, window_(window)
			, extent_(window.extent())
		{
			ENSURE(allGreaterEqual(window.min(), 0) && allLess(window.min(), view.extent()));
			ENSURE(allGreaterEqual(window.max(), 0) && allLessEqual(window.max(), view.extent()));
			ENSURE(allLessEqual(window.min(), window.max()));
		}

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Vector<integer, N>& position) const
		{
			return view_.constCursor(
				position + window_.min());
		}

	protected:
		Contained_ConstView view_;
		AlignedBox<integer, N> window_;
		Vector<integer, N> extent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ConstView<N, Input_Element, ConstSubView<N, Input_ConstView> >
		constSubView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const AlignedBox<integer, N>& window)
	{
		return wrapConstView(
			ConstSubView<N, Input_ConstView>(view.contained(), window));
	}

	template <int N, typename Contained_View>
	class SubView
		: public ConstSubView<N, Contained_View>
	{
	private:
		using Base = ConstSubView<N, Contained_View>;
		using Base::view_;
		using Base::window_;

	public:
		//using Base::Dimension;
		using Element = typename Base::Element;
		using ConstReference = typename Base::ConstReference;
		using ConstCursor = typename Base::ConstCursor;
		using Base::extent;
		using Base::constCursor;

		using Reference = typename Contained_View::Reference;
		using Cursor = typename Contained_View::Cursor;

		SubView()
			: Base()
		{
		}

		SubView(
			const Contained_View& view,
			const AlignedBox<integer, N>& window)
			: Base(view, window)
		{
		}

		Cursor cursor(
			const Vector<integer, N>& position) const
		{
			return view_.cursor(
				position + window_.min());
		}
	};

	template <int N, typename Input_Element, typename Input_View>
	View<N, Input_Element, SubView<N, Input_View> >
		subView(const View<N, Input_Element, Input_View>& view,
			const AlignedBox<integer, N>& window)
	{
		return wrapView(SubView<N, Input_View>(view.contained(), window));
	}

}

#endif
