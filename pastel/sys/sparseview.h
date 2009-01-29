#ifndef PASTELSYS_SPARSEVIEW_H
#define PASTELSYS_SPARSEVIEW_H

#include "pastel/sys/view.h"
#include "pastel/sys/sparseviewcursor.h"

namespace Pastel
{

	template <int N, typename ConstView>
	class ConstSparseView
	{
	public:
		enum
		{
			Dimension = N
		};

		typedef typename ConstView::Element Element;
		typedef typename ConstView::ConstReference ConstReference;
		typedef Detail_SparseView::ConstSparseViewCursor<N, typename ConstView::ConstCursor>
			ConstCursor;

		explicit ConstSparseView(
			const ConstView& view,
			const Vector<N, integer>& step)
			: view_(view)
			, step_(step)
			, extent_(view.extent() / step)
		{
			ENSURE(allGreater(step, 0));
		}

		const Vector<N, integer>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Point<N, integer>& position) const
		{
			return ConstCursor(view_.constCursor(
				Point<N, integer>(asVector(position) * step_)), step_);
		}

	protected:
		const ConstView view_;
		const Vector<N, integer> step_;
		const Vector<N, integer> extent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ConstView<N, Input_Element, ConstSparseView<N, Input_ConstView> >
		constSparseView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const Vector<N, integer>& step)
	{
		return wrapConstView(
			ConstSparseView<N, Input_ConstView>(view.contained(), step));
	}

	template <int N, typename Contained_View>
	class SparseView
		: public ConstSparseView<N, Contained_View>
	{
	private:
		typedef ConstSparseView<N, Contained_View> Base;
		using Base::view_;
		using Base::step_;

	public:
		//using Base::Dimension;
		using typename Base::Element;
		using typename Base::ConstReference;
		using typename Base::ConstCursor;
		using Base::extent;
		using Base::constCursor;

		typedef typename Contained_View::Reference Reference;
		typedef Detail_SparseView::SparseViewCursor<N, typename Contained_View::Cursor>
			Cursor;

		explicit SparseView(
			const Contained_View& view,
			const Vector<N, integer>& step)
			: Base(view, step)
		{
		}

		Cursor cursor(const Point<N, integer>& position) const
		{
			return Cursor(view_.cursor(
				Point<N, integer>(asVector(position) * step_)), step_);
		}
	};

	template <int N, typename Input_Element, typename Input_View>
	View<N, Input_Element, SparseView<N, Input_View> >
		sparseView(
		const View<N, Input_Element, Input_View>& view,
		const Vector<N, integer>& step)
	{
		return wrapView(SparseView<N, Input_View>(view.contained(), step));
	}

}


#endif
