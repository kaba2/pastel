// Description: Visiting a view or a pair of views

#ifndef PASTEL_VIEW_VISIT_H
#define PASTEL_VIEW_VISIT_H

#include "pastel/sys/view.h"
#include "pastel/sys/alignedbox.h"

namespace Pastel
{

	//! Applies a functor to all positions: visitor(x).

	template <int N, typename VisitorFunctor>
	void visit(
		const AlignedBox<integer, N>& rectangle,
		const VisitorFunctor& visitor);

	//! Applies a functor to elements of view: visitor(input(x)).

	template <int N, typename Type, typename Input_ConstView, typename VisitorFunctor>
	void visit(
		const ConstView<N, Type, Input_ConstView>& input,
		const VisitorFunctor& visitor);

	//! Applies a functor to elements of view: visitor(input(x)).

	template <int N, typename Type, typename Input_View, typename VisitorFunctor>
	void visit(
		const View<N, Type, Input_View>& input,
		const VisitorFunctor& visitor);

	//! Applies a functor to elements of views: visitor(left(x), right(x)).

	/*!
	Preconditions:
	left.extent() == right.extent();
	*/

	template <int N,
		typename Left_Element, typename Left_ConstView,
		typename Right_Element, typename Right_View,
		typename VisitorFunctor>
	void visit(
		const ConstView<N, Left_Element, Left_ConstView>& left,
		const View<N, Right_Element, Right_View>& right,
		const VisitorFunctor& visitor);

	//! Applies a functor to elements of views: visitor(left(x), right(x)).

	/*!
	Preconditions:
	left.extent() == right.extent();
	*/

	template <int N,
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename VisitorFunctor>
	void visit(
		const View<N, Left_Element, Left_View>& left,
		const View<N, Right_Element, Right_View>& right,
		const VisitorFunctor& visitor);

	//! Applies a functor to elements of views: visitor(left(x), right(x)).

	/*!
	Preconditions:
	left.extent() == right.extent();
	*/

	template <int N,
		typename Left_Element, typename Left_ConstView,
		typename Right_Element, typename Right_ConstView,
		typename VisitorFunctor>
	void visit(
		const ConstView<N, Left_Element, Left_ConstView>& left,
		const ConstView<N, Right_Element, Right_ConstView>& right,
		const VisitorFunctor& visitor);

}

#include "pastel/sys/view_visit_more.h"
#include "pastel/sys/view_visit_more2.h"

#include "pastel/sys/view_visit.hpp"
#include "pastel/sys/view_visit_more.hpp"
#include "pastel/sys/view_visit_more2.hpp"

#endif
