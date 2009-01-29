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
		enum
		{
			Dimension = N
		};

		typedef typename Contained_ConstView::Element Element;
		typedef typename Contained_ConstView::ConstReference ConstReference;

		typedef Detail_PermutedView::ConstPermutedViewCursor<N, typename Contained_ConstView::ConstCursor>
			ConstCursor;

		explicit ConstPermutedView(
			const Contained_ConstView& view,
			const Tuple<N, integer>& permutation)
			: view_(view)
			, permutation_(permutation)
			, extent_(permute(view.extent(), permutation))
		{
		}

		const Vector<N, integer>& extent() const
		{
			return extent_;
		}

		ConstCursor constCursor(
			const Point<N, integer>& position) const
		{
			return ConstCursor(view_.constCursor(
				Point<N, integer>(permute(asVector(position), permutation_))),
				permutation_);
		}

	protected:
		const Contained_ConstView view_;
		const Tuple<N, integer> permutation_;
		const Vector<N, integer> extent_;
	};

	template <int N, typename Input_Element, typename Input_ConstView>
	ConstView<N, Input_Element, ConstPermutedView<N, Input_ConstView> >
		constPermutedView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const Tuple<N, integer>& permutation)
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
		using typename Base::ConstCursor;
		using typename Base::Element;
		using typename Base::ConstReference;
		using Base::extent;
		using Base::constCursor;

		typedef typename Contained_View::Reference Reference;
		typedef Detail_PermutedView::PermutedViewCursor<N, typename Contained_View::Cursor>
			Cursor;

		explicit PermutedView(
			const Contained_View& view,
			const Tuple<N, integer>& permutation)
			: Base(view, permutation)
		{
		}

		Cursor cursor(const Point<N, integer>& position) const
		{
			return Cursor(view_.cursor(
				asPoint(permute(asVector(position), permutation_))),
				permutation_);
		}
	};

	template <int N, typename Input_Element, typename Input_View>
	View<N, Input_Element, PermutedView<N, Input_View> > permutedView(
		const View<N, Input_Element, Input_View>& view,
		const Tuple<N, integer>& permutation)
	{
		return wrapView(PermutedView<N, Input_View>(view.contained(), permutation));
	}

}

#endif
