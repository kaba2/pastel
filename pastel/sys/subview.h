#ifndef PASTEL_SUBVIEW_H
#define PASTEL_SUBVIEW_H

#include "pastel/sys/rectangle.h"

#include "pastel/sys/view.h"

namespace Pastel
{

	template <int N, typename Contained_ConstView>
	class ConstSubView
	{
	public:
		enum
		{
			Dimension = N
		};

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
			const Rectangle<N>& window)
			: view_(view)
			, window_(window)
			, extent_(window.extent())
		{
			ENSURE(allGreaterEqual(window.min(), 0) && allLess(asVector(window.min()), view.extent()));
			ENSURE(allGreaterEqual(window.max(), 0) && allLessEqual(asVector(window.max()), view.extent()));
			ENSURE(allLessEqual(window.min(), window.max()));
		}

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Point<integer, N>& position) const
		{
			return view_.constCursor(
				position + asVector(window_.min()));
		}

	protected:
		const Contained_ConstView view_;
		const Rectangle<N> window_;
		const Vector<integer, N> extent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ConstView<N, Input_Element, ConstSubView<N, Input_ConstView> >
		constSubView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const Rectangle<N>& window)
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
		using typename Base::Element;
		using typename Base::ConstReference;
		using typename Base::ConstCursor;
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
			const Rectangle<N>& window)
			: Base(view, window)
		{
		}

		Cursor cursor(
			const Point<integer, N>& position) const
		{
			return view_.cursor(
				position + asVector(window_.min()));
		}
	};

	template <int N, typename Input_Element, typename Input_View>
	View<N, Input_Element, SubView<N, Input_View> >
		subView(const View<N, Input_Element, Input_View>& view,
			const Rectangle<N>& window)
	{
		return wrapView(SubView<N, Input_View>(view.contained(), window));
	}

}

#endif
