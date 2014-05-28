// Description: Adapted view
// Documentation: concrete_views.txt

#ifndef PASTELSYS_ADAPTEDVIEW_H
#define PASTELSYS_ADAPTEDVIEW_H

#include "pastel/sys/view.h"
#include "pastel/sys/adaptedviewcursor.h"

namespace Pastel
{

	template <int N, typename Adapter, typename Contained_ConstView>
	class ConstAdaptedView
	{
	public:
		static PASTEL_CONSTEXPR int Dimension = N;

		typedef AdaptedView_::ConstAdaptedViewCursor<N, typename Contained_ConstView::ConstCursor, Adapter>
			ConstCursor;
		typedef typename ConstCursor::Element Element;
		typedef typename ConstCursor::ConstReference ConstReference;

		explicit ConstAdaptedView(
			const Contained_ConstView& view,
			const Adapter& adapter)
			: view_(view)
			, adapter_(adapter)
			, extent_(view.extent())
		{
		}

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Vector<integer, N>& position) const
		{
			return ConstCursor(view_.constCursor(position), adapter_);
		}

	protected:
		const Contained_ConstView view_;
		const Adapter adapter_;
		const Vector<integer, N> extent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView, typename Adapter>
	ConstView<N, typename ConstAdaptedView<N, Adapter, Input_ConstView>::Element,
		ConstAdaptedView<N, Adapter, Input_ConstView> >
		constAdaptedView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const Adapter& adapter)
	{
		return wrapConstView(
			ConstAdaptedView<N, Adapter, Input_ConstView>(view.contained(), adapter));
	}

	template <int N, typename Adapter, typename Contained_View>
	class AdaptedView
		: public ConstAdaptedView<N, Adapter, Contained_View>
	{
	private:
		typedef ConstAdaptedView<N, Adapter, Contained_View> Base;

		using Base::view_;
		using Base::adapter_;

	public:
		//using Base::Dimension;
		typedef typename Base::ConstCursor ConstCursor;
		using Base::extent;
		using Base::constCursor;

		typedef AdaptedView_::AdaptedViewCursor<N, typename Contained_View::Cursor, Adapter>
			Cursor;
		typedef typename Base::Element Element;
		typedef typename Cursor::Reference Reference;

		explicit AdaptedView(
			const Contained_View& view,
			const Adapter& adapter)
			: Base(view, adapter)
		{
		}

		Cursor cursor(const Vector<integer, N>& position) const
		{
			return Cursor(view_.cursor(position), adapter_);
		}
	};

	template <int N, typename Input_Element, typename Input_View, typename Adapter>
	View<N, typename AdaptedView<N, Adapter, Input_View>::Element,
		AdaptedView<N, Adapter, Input_View> > adaptedView(
		const View<N, Input_Element, Input_View>& view,
		const Adapter& adapter)
	{
		return wrapView(AdaptedView<N, Adapter, Input_View>(view.contained(), adapter));
	}

}

#endif
