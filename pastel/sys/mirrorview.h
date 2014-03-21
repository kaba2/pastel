// Description: MirrorView class
// Detail: An adapter view to mirror a specific dimension
// Documentation: concrete_views.txt

#ifndef PASTELSYS_MIRRORVIEW_H
#define PASTELSYS_MIRRORVIEW_H

#include "pastel/sys/mirrorviewcursor.h"

namespace Pastel
{

	//! Mirrors the specified dimension.
	/*!
	More specifically:

	mirrorView(x_1, ..., x_k, ..., x_n) =
	view(x_1, ..., (extent(k) - 1) - x_k, ..., x_n)
	*/

	template <int N, int MirrorIndex, typename Contained_ConstView>
	class ConstMirrorView
	{
	public:
		PASTEL_CONSTEXPR int Dimension = N;

		typedef typename Contained_ConstView::Element Element;
		typedef typename Contained_ConstView::ConstReference ConstReference;
		typedef MirrorView_::ConstMirrorViewCursor<N, MirrorIndex,
			typename Contained_ConstView::ConstCursor> ConstCursor;

		explicit ConstMirrorView(
			const Contained_ConstView& view)
			: view_(view)
		{
		}

		const Vector<integer, N>& extent() const
		{
			return view_.extent();
		}

		ConstCursor constCursor(
			Vector<integer, N> position) const
		{
			position[MirrorIndex] = view_.extent()[MirrorIndex] - 1 - position[MirrorIndex];

			return ConstCursor(view_.constCursor(position));
		}

	protected:
		const Contained_ConstView view_;
	};

	template <int MirrorIndex, int N, typename Input_Element, typename Input_ConstView>
	ConstView<N, Input_Element, ConstMirrorView<N, MirrorIndex, Input_ConstView> >
		constMirrorView(
		const ConstView<N, Input_Element, Input_ConstView>& view)
	{
		return wrapConstView(ConstMirrorView<N, MirrorIndex, Input_ConstView>(view.contained()));
	}

	//! Mirrors the specified dimension.
	/*!
	More specifically:

	mirrorView(x_1, ..., x_k, ..., x_n) =
	view(x_1, ..., (extent(k) - 1) - x_k, ..., x_n)
	*/

	template <int N, int MirrorIndex, typename Contained_View>
	class MirrorView
		: public ConstMirrorView<N, MirrorIndex, Contained_View>
	{
	private:
		typedef ConstMirrorView<N, MirrorIndex, Contained_View> Base;
		using Base::view_;

	public:
		//using Base::Dimension;
		typedef typename Base::Element Element;
		typedef typename Base::ConstReference ConstReference;
		typedef typename Base::ConstCursor ConstCursor;
		using Base::width;
		using Base::height;
		using Base::constCursor;

		typedef typename Contained_View::Reference Reference;
		typedef MirrorView_::MirrorViewCursor<N, MirrorIndex, typename Contained_View::Cursor>
			Cursor;

		explicit MirrorView(const Contained_View& view)
			: ConstMirrorView<N, MirrorIndex, Contained_View>(view)
		{
		}

		Cursor cursor(
			Vector<integer, N> position) const
		{
			position[MirrorIndex] = view_.extent()[MirrorIndex] - 1 - position[MirrorIndex];

			return Cursor(view_.cursor(position));
		}
	};

	template <int MirrorIndex, int N, typename Input_Element, typename Input_View>
	View<N, Input_Element, MirrorView<N, MirrorIndex, Input_View> >
		mirrorView(
		const View<N, Input_Element, Input_View>& view)
	{
		return wrapView(MirrorView<N, MirrorIndex, Input_View>(view.contained()));
	}

}


#endif
