// Description: TransformedView class
// Detail: An adapter view to change the apparent type of the data.
// Documentation: view.txt

#ifndef PASTEL_TRANSFORMEDVIEW_H
#define PASTEL_TRANSFORMEDVIEW_H

#include "pastel/sys/view.h"
#include "pastel/sys/transformedviewcursor.h"

namespace Pastel
{

	//! An adapter view to change the apparent type of the data.
	/*!
	This view is only given as a non-mutable version. This is because
	the transformed data is constructed on the fly as it is accessed.
	The transformation is given by a user-supplied functor TransformFunctor.

	class TransformFunctor
	{
	public:
		typedef unspecified_type Input_;
		typedef unspecified_type Output_;

		Output_ operator()(const Input_& input) const;
	};
	*/

	template <int N, typename TransformFunctor, typename Contained_ConstView>
	class ConstTransformedView
	{
	public:
		enum
		{
			Dimension = N
		};

		typedef typename Contained_ConstView::Element Element;
		typedef typename Contained_ConstView::ConstReference ConstReference;

		typedef Detail_TransformedView::ConstTransformedViewCursor<N, TransformFunctor,
			typename Contained_ConstView::ConstCursor> ConstCursor;

		explicit ConstTransformedView(
			const Contained_ConstView& view,
			const TransformFunctor& transform)
			: view_(view)
			, transform_(transform)
		{
		}

		const Vector<integer, N>& extent() const
		{
			return view_.extent();
		}

		ConstCursor constCursor(
			const Vector<integer, N>& position) const
		{
			return ConstCursor(view_.constCursor(position), transform_);
		}

	protected:
		const Contained_ConstView view_;
		const TransformFunctor transform_;
	};

	template <typename TransformFunctor, int N, typename Input_Element, typename Input_ConstView>
	ConstView<N, Input_Element, ConstTransformedView<N, TransformFunctor, Input_ConstView> >
		constTransformedView(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		const TransformFunctor& transform)
	{
		return wrapConstView(
			ConstTransformedView<N, TransformFunctor, Input_ConstView>(
			view.contained(), transform));
	}

}

#endif
