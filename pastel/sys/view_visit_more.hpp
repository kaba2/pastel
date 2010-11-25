#ifndef PASTEL_VIEW_VISIT_MORE_HPP
#define PASTEL_VIEW_VISIT_MORE_HPP

#include "pastel/sys/view_visit_more.h"

namespace Pastel
{

	// Single view traversal

	namespace Detail_VisitPosition
	{

		class NormalTag {};
		class TerminateTag {};

		template <int Index, int N, typename Cursor, typename VisitPositionFunctor>
		void visitPositionDimension(
			const Vector<integer, N>& extent,
			const Vector<integer, N>& startPosition,
			const Cursor& startCursor,
			const VisitPositionFunctor& visitPosition,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Cursor cursor = startCursor;
			Vector<integer, N> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitPositionDimension<Index - 1>(
					extent, position, cursor, visitPosition, Tag());

				cursor.increment(Index);
				++position[Index];
			}
		}

		template <int Index, int N, typename Cursor, typename VisitPositionFunctor>
		void visitPositionDimension(
			const Vector<integer, N>& extent,
			const Vector<integer, N>& startPosition,
			const Cursor& startCursor,
			const VisitPositionFunctor& visitPosition,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Cursor cursor = startCursor;
			Vector<integer, N> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				visitPosition(position, *cursor);

				cursor.increment(Index);
				++position[Index];
			}
		}

	}

	template <int N, typename Input_Element, typename Input_ConstView, typename VisitPositionFunctor>
	void visitPosition(
		const ConstView<N, Input_Element, Input_ConstView>& input,
		const VisitPositionFunctor& visitPosition)
	{
		typedef typename boost::mpl::if_c<
			(N > 1),
			Detail_VisitPosition::NormalTag,
			Detail_VisitPosition::TerminateTag>::type Tag;

		Detail_VisitPosition::visitPositionDimension<N - 1>(
			input.extent(),
			Vector<integer, N>(0),
			input.constCursor(Vector<integer, N>(0)),
			visitPosition,
			Tag());
	}

	template <int N, typename Input_Element, typename Input_View, typename VisitPositionFunctor>
	void visitPosition(
		const View<N, Input_Element, Input_View>& input,
		const VisitPositionFunctor& visitPosition)
	{
		typedef typename boost::mpl::if_c<
			(N > 1),
			Detail_VisitPosition::NormalTag,
			Detail_VisitPosition::TerminateTag>::type Tag;

		Detail_VisitPosition::visitPositionDimension<N - 1>(
			input.extent(),
			Vector<integer, N>(0),
			input.cursor(Vector<integer, N>(0)),
			visitPosition,
			Tag());
	}

	// Two-view parallel traversal

	namespace Detail_VisitPosition
	{

		template <int Index, int N,
			typename Left_Cursor,
			typename Right_Cursor,
			typename VisitPositionFunctor>
		void visitPositionDimension(
			const Vector<integer, N>& extent,
			const Vector<integer, N>& startPosition,
			const Left_Cursor& leftStartCursor,
			const Right_Cursor&  rightStartCursor,
			const VisitPositionFunctor& visitPosition,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Left_Cursor leftCursor = leftStartCursor;
			Right_Cursor rightCursor = rightStartCursor;
			Vector<integer, N> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitPositionDimension<Index - 1>(
					extent, position, leftCursor, rightCursor, visitPosition, Tag());

				leftCursor.increment(Index);
				rightCursor.increment(Index);
				++position[Index];
			}
		}

		template <int Index, int N,
			typename Left_Cursor,
			typename Right_Cursor,
			typename VisitPositionFunctor>
		void visitPositionDimension(
			const Vector<integer, N>& extent,
			const Vector<integer, N>& startPosition,
			const Left_Cursor& leftStartCursor,
			const Right_Cursor& rightStartCursor,
			const VisitPositionFunctor& visitPosition,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Left_Cursor leftCursor = leftStartCursor;
			Right_Cursor rightCursor = rightStartCursor;
			Vector<integer, N> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				visitPosition(position, *leftCursor, *rightCursor);

				leftCursor.increment(Index);
				rightCursor.increment(Index);
				++position[Index];
			}
		}

	}

	template <int N,
		typename Left_Element, typename Left_ConstView,
		typename Right_Element, typename Right_View,
		typename VisitPositionFunctor>
	void visitPosition(
		const ConstView<N, Left_Element, Left_ConstView>& left,
		const View<N, Right_Element, Right_View>& right,
		const VisitPositionFunctor& visitPosition)
	{
		ENSURE(left.extent() == right.extent());

		typedef typename boost::mpl::if_c<
			(N > 1),
			Detail_VisitPosition::NormalTag,
			Detail_VisitPosition::TerminateTag>::type Tag;

		Detail_VisitPosition::visitPositionDimension<N - 1>(
			left.extent(),
			Vector<integer, N>(0),
			left.constCursor(Vector<integer, N>(0)),
			right.cursor(Vector<integer, N>(0)),
			visitPosition,
			Tag());
	}

	template <int N,
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename VisitPositionFunctor>
	void visitPosition(
		const View<N, Left_Element, Left_View>& left,
		const View<N, Right_Element, Right_View>& right,
		const VisitPositionFunctor& visitPosition)
	{
		ENSURE(left.extent() == right.extent());

		typedef typename boost::mpl::if_c<
			(N > 1),
			Detail_VisitPosition::NormalTag,
			Detail_VisitPosition::TerminateTag>::type Tag;

		Detail_VisitPosition::visitPositionDimension<N - 1>(
			left.extent(),
			Vector<integer, N>(0),
			left.cursor(Vector<integer, N>(0)),
			right.cursor(Vector<integer, N>(0)),
			visitPosition,
			Tag());
	}

	template <int N,
		typename Left_Element, typename Left_ConstView,
		typename Right_Element, typename Right_ConstView,
		typename VisitPositionFunctor>
	void visitPosition(
		const ConstView<N, Left_Element, Left_ConstView>& left,
		const ConstView<N, Right_Element, Right_ConstView>& right,
		const VisitPositionFunctor& visitPosition)
	{
		ENSURE(left.extent() == right.extent());

		typedef typename boost::mpl::if_c<
			(N > 1),
			Detail_VisitPosition::NormalTag,
			Detail_VisitPosition::TerminateTag>::type Tag;

		Detail_VisitPosition::visitPositionDimension<N - 1>(
			left.extent(),
			Vector<integer, N>(0),
			left.constCursor(Vector<integer, N>(0)),
			right.constCursor(Vector<integer, N>(0)),
			visitPosition,
			Tag());
	}

}

#endif
