// Description: SubView class
// Detail: An adapter view for an n-dimensional subset
// Documentation: concrete_views.txt

#ifndef PASTELSYS_SUBVIEW_H
#define PASTELSYS_SUBVIEW_H

#include "pastel/sys/alignedbox.h"

#include "pastel/sys/view.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class ConstSubView
	{
	public:
		static PASTEL_CONSTEXPR int Dimension = N;

		typedef typename Contained_ConstView::Element Element;
		typedef typename Contained_ConstView::ConstReference ConstReference;
		typedef typename Contained_ConstView::ConstCursor ConstCursor;

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
		const Contained_ConstView view_;
		const AlignedBox<integer, N> window_;
		const Vector<integer, N> extent_;
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
		typedef ConstSubView<N, Contained_View> Base;
		using Base::view_;
		using Base::window_;

	public:
		//using Base::Dimension;
		typedef typename Base::Element Element;
		typedef typename Base::ConstReference ConstReference;
		typedef typename Base::ConstCursor ConstCursor;
		using Base::extent;
		using Base::constCursor;

		typedef typename Contained_View::Reference Reference;
		typedef typename Contained_View::Cursor Cursor;

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
