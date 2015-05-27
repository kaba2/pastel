// Description: SparseView class
// Detail: An adapter view for a sparse grid subset
// Documentation: views.txt

#ifndef PASTELSYS_SPARSEVIEW_H
#define PASTELSYS_SPARSEVIEW_H

#include "pastel/sys/view/view.h"
#include "pastel/sys/view/sparseviewcursor.h"

namespace Pastel
{

	template <int N, typename ConstView>
	class ConstSparseView
	{
	public:
		static constexpr int Dimension = N;

		using Element = typename ConstView::Element;
		using ConstReference = typename ConstView::ConstReference;
		typedef SparseView_::ConstSparseViewCursor<N, typename ConstView::ConstCursor>
			ConstCursor;

		explicit ConstSparseView(
			const ConstView& view,
			const Vector<integer, N>& pivot,
			const Vector<integer, N>& step)
			: view_(view)
			, pivot_(pivot)
			, step_(step)
			, extent_((view.extent() - pivot + (step - 1)) / step)
		{
			ENSURE(allGreater(step, 0));
			ENSURE(allGreaterEqual(pivot, 0));
			ENSURE(allLess(pivot, view.extent()));
		}

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Vector<integer, N>& position) const
		{
			return ConstCursor(view_.constCursor(
				pivot_ + position * step_), step_);
		}

	protected:
		ConstView view_;
		Vector<integer, N> pivot_;
		Vector<integer, N> step_;
		Vector<integer, N> extent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ConstView<N, Input_Element, ConstSparseView<N, Input_ConstView> >
		constSparseView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const NoDeduction<Vector<integer, N>>& pivot,
		const NoDeduction<Vector<integer, N>>& step)
	{
		return wrapConstView(
			ConstSparseView<N, Input_ConstView>(view.contained(), pivot, step));
	}

	template <int N, typename Contained_View>
	class SparseView
		: public ConstSparseView<N, Contained_View>
	{
	private:
		using Base = ConstSparseView<N, Contained_View>;
		using Base::view_;
		using Base::step_;
		using Base::pivot_;

	public:
		//using Base::Dimension;
		using Element = typename Base::Element;
		using ConstReference = typename Base::ConstReference;
		using ConstCursor = typename Base::ConstCursor;
		using Base::extent;
		using Base::constCursor;

		using Reference = typename Contained_View::Reference;
		typedef SparseView_::SparseViewCursor<N, typename Contained_View::Cursor>
			Cursor;

		explicit SparseView(
			const Contained_View& view,
			const Vector<integer, N>& pivot,
			const Vector<integer, N>& step)
			: Base(view, pivot, step)
		{
		}

		Cursor cursor(const Vector<integer, N>& position) const
		{
			return Cursor(view_.cursor(pivot_ + position * step_), 
				step_);
		}
	};

	template <int N, typename Input_Element, typename Input_View>
	View<N, Input_Element, SparseView<N, Input_View> >
		sparseView(
		const View<N, Input_Element, Input_View>& view,
		const NoDeduction<Vector<integer, N>>& pivot,
		const NoDeduction<Vector<integer, N>>& step)
	{
		return wrapView(SparseView<N, Input_View>(view.contained(), pivot, step));
	}

}


#endif
