// Description: PermutedView class
// Detail: An adapter view to permute dimensions
// Documentation: concrete_views.txt

#ifndef PASTELSYS_PERMUTEDVIEW_H
#define PASTELSYS_PERMUTEDVIEW_H

#include "pastel/sys/view.h"
#include "pastel/sys/permutedviewcursor.h"

namespace Pastel
{

	//! Permutes the dimensions of the view.
	/*!
	More specifically:

	permutedView(x_1, ..., x_n) =
	view(x_p(1), ..., x_p(n))

	Where
	p is in [1, n]^n
	*/

	template <int N, typename Contained_ConstView>
	class ConstPermutedView
	{
	public:
		PASTEL_CONSTEXPR int Dimension = N;

		typedef typename Contained_ConstView::Element Element;
		typedef typename Contained_ConstView::ConstReference ConstReference;

		typedef PermutedView_::ConstPermutedViewCursor<N, typename Contained_ConstView::ConstCursor>
			ConstCursor;

		explicit ConstPermutedView(
			const Contained_ConstView& view,
			const Tuple<integer, N>& permutation)
			: view_(view)
			, permutation_(permutation)
			, extent_(permute(view.extent(), permutation))
		{
		}

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Vector<integer, N>& position) const
		{
			return ConstCursor(view_.constCursor(
				Vector<integer, N>(permute(position, permutation_))),
				permutation_);
		}

	protected:
		const Contained_ConstView view_;
		const Tuple<integer, N> permutation_;
		const Vector<integer, N> extent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ConstView<N, Input_Element, ConstPermutedView<N, Input_ConstView> >
		constPermutedView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const Tuple<integer, N>& permutation)
	{
		return wrapConstView(
			ConstPermutedView<N, Input_ConstView>(view.contained(), permutation));
	}

	//! Permutes the dimensions of the view.
	/*!
	More specifically:

	permutedView(x_1, ..., x_n) =
	view(x_p(1), ..., x_p(n))

	Where
	p is in [1, n]^n
	*/

	template <int N, typename Contained_View>
	class PermutedView
		: public ConstPermutedView<N, Contained_View>
	{
	private:
		typedef ConstPermutedView<N, Contained_View> Base;

		using Base::view_;
		using Base::permutation_;

	public:
		//using Base::Dimension;
		typedef typename Base::ConstCursor ConstCursor;
		typedef typename Base::Element Element;
		typedef typename Base::ConstReference ConstReference;
		using Base::extent;
		using Base::constCursor;

		typedef typename Contained_View::Reference Reference;
		typedef PermutedView_::PermutedViewCursor<N, typename Contained_View::Cursor>
			Cursor;

		explicit PermutedView(
			const Contained_View& view,
			const Tuple<integer, N>& permutation)
			: Base(view, permutation)
		{
		}

		Cursor cursor(const Vector<integer, N>& position) const
		{
			return Cursor(view_.cursor(
				asPoint(permute(position, permutation_))),
				permutation_);
		}
	};

	template <int N, typename Input_Element, typename Input_View>
	View<N, Input_Element, PermutedView<N, Input_View> > permutedView(
		const View<N, Input_Element, Input_View>& view,
		const Tuple<integer, N>& permutation)
	{
		return wrapView(PermutedView<N, Input_View>(view.contained(), permutation));
	}

}

#endif
