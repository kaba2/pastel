// Description: Visiting the rows of a view or a pair of views

#ifndef PASTELSYS_VIEW_VISIT_MORE2_H
#define PASTELSYS_VIEW_VISIT_MORE2_H

#include "pastel/sys/view/view_visit.h"

namespace Pastel
{

	//! Visits all rows of a non-mutable view.
	/*!
	Preconditions:
	0 <= freeIndex < N

	Effectively, this function calls
	rowVisitor(constRowView(view, freeIndex, position))
	once for each row.
	*/

	template <integer N, 
		typename Input_Element, typename Input_ConstView,
		typename RowVisitorFunctor>
		void visitRows(
		const ConstView<N, Input_Element, Input_ConstView>& view,
		integer freeIndex,
		const RowVisitorFunctor& rowVisitor);

	//! Visits all rows of a view.
	/*!
	Preconditions:
	0 <= freeIndex < N

	Effectively, this function calls
	rowVisitor(rowView(view, freeIndex, position))
	once for each row.
	*/
	template <integer N, 
		typename Input_Element, typename Input_View,
		typename RowVisitorFunctor>
		void visitRows(
		const View<N, Input_Element, Input_View>& view,
		integer freeIndex,
		const RowVisitorFunctor& rowVisitor);

	//! Visits all rows of two views in parallel.
	/*!
	Preconditions:
	0 <= freeIndex < N
	for all i != freeIndex: left.extent()[i] == right.extent()[i]
	
	Effectively, this function calls
	rowVisitor(rowView(left, freeIndex, position),
	rowView(right, freeIndex, position))
	once for each row.
	*/
	template <integer N, 
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename RowVisitorFunctor>
		void visitRows(
		const View<N, Left_Element, Left_View>& left,
		const View<N, Right_Element, Right_View>& right,
		integer freeIndex,
		const RowVisitorFunctor& rowVisitor);

	//! Visits all rows of two views in parallel.
	/*!
	Preconditions:
	0 <= freeIndex < N
	for all i != freeIndex: left.extent()[i] == right.extent()[i]
	
	Effectively, this function calls
	rowVisitor(constRowView(left, freeIndex, position),
	rowView(right, freeIndex, position))
	once for each row.
	*/
	template <integer N, 
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename RowVisitorFunctor>
		void visitRows(
		const ConstView<N, Left_Element, Left_View>& left,
		const View<N, Right_Element, Right_View>& right,
		integer freeIndex,
		const RowVisitorFunctor& rowVisitor);

	//! Visits all rows of two non-mutable views in parallel.
	/*!
	Preconditions:
	0 <= freeIndex < N
	for all i != freeIndex: left.extent()[i] == right.extent()[i]
	
	Effectively, this function calls
	rowVisitor(constRowView(left, freeIndex, position),
	constRowView(right, freeIndex, position))
	once for each row.
	*/
	template <integer N, 
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename RowVisitorFunctor>
		void visitRows(
		const ConstView<N, Left_Element, Left_View>& left,
		const ConstView<N, Right_Element, Right_View>& right,
		integer freeIndex,
		const RowVisitorFunctor& rowVisitor);

}

#endif
