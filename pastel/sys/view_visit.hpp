#ifndef PASTELSYS_VIEW_VISIT_HPP
#define PASTELSYS_VIEW_VISIT_HPP

#include "pastel/sys/view_visit.h"

namespace Pastel
{

	// Traversal without a view

	namespace VisitRectangle_
	{

		class NormalTag {};
		class TerminateTag {};

		template <int Index, int N, typename VisitRectangleFunctor>
		void visitRectangleDimension(
			const Vector<integer, N>& extent,
			const Vector<integer, N>& startPosition,
			const VisitRectangleFunctor& visitRectangle,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			integer width = extent[Index];

			Vector<integer, N> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitRectangleDimension<Index - 1>(
					extent, position, visitRectangle, Tag());

				++position[Index];
			}
		}

		template <int Index, int N, typename VisitRectangleFunctor>
		void visitRectangleDimension(

			const Vector<integer, N>& extent,
			const Vector<integer, N>& startPosition,
			const VisitRectangleFunctor& visitRectangle,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			integer width = extent[Index];

			Vector<integer, N> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				visitRectangle(position);

				++position[Index];
			}
		}

	}

	template <int N, typename VisitorFunctor>
	void visit(

		const AlignedBox<integer, N>& rectangle,
		const VisitorFunctor& visitor)
	{
		typedef typename boost::mpl::if_c<
			(N > 1),
			VisitRectangle_::NormalTag,
			VisitRectangle_::TerminateTag>::type Tag;

		VisitRectangle_::visitRectangleDimension<N - 1>(
			evaluate(rectangle.extent()),
			Vector<integer, N>(0),
			visitor,
			Tag());
	}

	// Single view traversal

	namespace Visit_
	{

		class NormalTag {};
		class TerminateTag {};

		template <int Index, int N, typename Cursor, typename VisitFunctor>
		void visitDimension(
			const Vector<integer, N>& extent,
			const Cursor& startCursor,
			const VisitFunctor& visitor,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			integer width = extent[Index];

			Cursor cursor = startCursor;

			for (integer i = 0;i < width;++i)
			{

				visitor(*cursor);

				cursor.increment(Index);
			}
		}

		template <int Index, int N, typename Cursor, typename VisitFunctor>
		void visitDimension(
			const Vector<integer, N>& extent,
			const Cursor& startCursor,
			const VisitFunctor& visitor,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			integer width = extent[Index];

			Cursor cursor = startCursor;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitDimension<Index - 1>(
					extent, cursor, visitor, Tag());

				cursor.increment(Index);
			}
		}

	}

	template <int N, typename Input_Element, typename Input_ConstView, typename VisitFunctor>
	void visit(

		const ConstView<N, Input_Element, Input_ConstView>& input,
		const VisitFunctor& visitor)
	{
		typedef typename boost::mpl::if_c<
			(N > 1),
			Visit_::NormalTag,
			Visit_::TerminateTag>::type Tag;

		Visit_::visitDimension<N - 1>(
			input.extent(),
			input.constCursor(Vector<integer, N>(0)),
			visitor,
			Tag());
	}

	template <int N, typename Input_Element, typename Input_View, typename VisitFunctor>
	void visit(
		const View<N, Input_Element, Input_View>& input,
		const VisitFunctor& visitor)
	{
		typedef typename boost::mpl::if_c<
			(N > 1),
			Visit_::NormalTag,
			Visit_::TerminateTag>::type Tag;

		Visit_::visitDimension<N - 1>(
			input.extent(),
			input.cursor(Vector<integer, N>(0)),
			visitor,
			Tag());
	}

	// Two-view parallel traversal
	
	namespace Visit_
	{

		template <int Index, int N,
			typename Left_Cursor,
			typename Right_Cursor,
			typename VisitFunctor>
		void visitDimension(
			const Vector<integer, N>& extent,
			const Left_Cursor& leftStartCursor,
			const Right_Cursor&  rightStartCursor,
			const VisitFunctor& visitor,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			integer width = extent[Index];

			Left_Cursor leftCursor = leftStartCursor;
			Right_Cursor rightCursor = rightStartCursor;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitDimension<Index - 1>(
					extent, leftCursor, rightCursor, visitor, Tag());

				leftCursor.increment(Index);
				rightCursor.increment(Index);
			}
		}

		template <int Index, int N,
			typename Left_Cursor,
			typename Right_Cursor,
			typename VisitFunctor>
		void visitDimension(

			const Vector<integer, N>& extent,
			const Left_Cursor& leftStartCursor,
			const Right_Cursor& rightStartCursor,
			const VisitFunctor& visitor,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			integer width = extent[Index];

			Left_Cursor leftCursor = leftStartCursor;
			Right_Cursor rightCursor = rightStartCursor;

			for (integer i = 0;i < width;++i)
			{

				visitor(*leftCursor, *rightCursor);

				leftCursor.increment(Index);
				rightCursor.increment(Index);
			}
		}

	}

	template <int N,
		typename Left_Element, typename Left_ConstView,
		typename Right_Element, typename Right_View,
		typename VisitFunctor>
	void visit(
		const ConstView<N, Left_Element, Left_ConstView>& left,
		const View<N, Right_Element, Right_View>& right,
		const VisitFunctor& visitor)
	{
		ENSURE(left.extent() == right.extent());

		typedef typename boost::mpl::if_c<
			(N > 1),
			Visit_::NormalTag,
			Visit_::TerminateTag>::type Tag;

		Visit_::visitDimension<N - 1>(
			left.extent(),
			left.constCursor(Vector<integer, N>(0)),
			right.cursor(Vector<integer, N>(0)),
			visitor,
			Tag());
	}

	template <int N,
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename VisitFunctor>
	void visit(
		const View<N, Left_Element, Left_View>& left,
		const View<N, Right_Element, Right_View>& right,
		const VisitFunctor& visitor)
	{
		ENSURE(left.extent() == right.extent());

		typedef typename boost::mpl::if_c<
			(N > 1),
			Visit_::NormalTag,
			Visit_::TerminateTag>::type Tag;

		Visit_::visitDimension<N - 1>(
			left.extent(),
			left.cursor(Vector<integer, N>(0)),
			right.cursor(Vector<integer, N>(0)),
			visitor,
			Tag());
	}

	template <int N,
		typename Left_Element, typename Left_ConstView,
		typename Right_Element, typename Right_ConstView,
		typename VisitFunctor>
	void visit(
		const ConstView<N, Left_Element, Left_ConstView>& left,
		const ConstView<N, Right_Element, Right_ConstView>& right,
		const VisitFunctor& visitor)
	{
		ENSURE(left.extent() == right.extent());

		typedef typename boost::mpl::if_c<
			(N > 1),
			Visit_::NormalTag,
			Visit_::TerminateTag>::type Tag;

		Visit_::visitDimension<N - 1>(
			left.extent(),
			left.constCursor(Vector<integer, N>(0)),
			right.constCursor(Vector<integer, N>(0)),
			visitor,
			Tag());
	}

}

#endif
