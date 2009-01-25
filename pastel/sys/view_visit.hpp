#ifndef PASTELSYS_VIEW_VISIT_HPP
#define PASTELSYS_VIEW_VISIT_HPP

#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace Detail_Visit
	{

		class NormalTag {};
		class TerminateTag {};

		// Single view traversal

		template <int Index, int N, typename Cursor, typename VisitFunctor>
		void visitDimension(
			const Vector<N, integer>& extent,
			const Cursor& startCursor,
			const VisitFunctor& visitor,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Cursor cursor = startCursor;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitDimension<Index - 1>(
					extent, cursor, visitor, Tag());
				
				cursor.increment<Index>();
			}
		}

		template <int Index, int N, typename Cursor, typename VisitFunctor>
		void visitDimension(
			const Vector<N, integer>& extent,
			const Cursor& startCursor,
			const VisitFunctor& visitor,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Cursor cursor = startCursor;

			for (integer i = 0;i < width;++i)
			{
				visitor(*cursor);

				cursor.increment<Index>();
			}
		}

		// Two-view parallel traversal

		template <int Index, int N, 
			typename Left_Cursor,
			typename Right_Cursor,
			typename VisitFunctor>
		void visitDimension(
			const Vector<N, integer>& extent,
			const Left_Cursor& leftStartCursor,
			const Right_Cursor&  rightStartCursor,
			const VisitFunctor& visitor,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			const integer width = extent[Index];

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
				
				leftCursor.increment<Index>();
				rightCursor.increment<Index>();
			}
		}

		template <int Index, int N, 
			typename Left_Cursor, 
			typename Right_Cursor,
			typename VisitFunctor>
		void visitDimension(
			const Vector<N, integer>& extent,
			const Left_Cursor& leftStartCursor,
			const Right_Cursor& rightStartCursor,
			const VisitFunctor& visitor,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Left_Cursor leftCursor = leftStartCursor;
			Right_Cursor rightCursor = rightStartCursor;

			for (integer i = 0;i < width;++i)
			{
				visitor(*leftCursor, *rightCursor);

				leftCursor.increment<Index>();
				rightCursor.increment<Index>();
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
			Detail_Visit::NormalTag,
			Detail_Visit::TerminateTag>::type Tag;

		Detail_Visit::visitDimension<N - 1>(
			input.extent(), 
			input.constCursor(Point<N, integer>(0)), 
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
			Detail_Visit::NormalTag,
			Detail_Visit::TerminateTag>::type Tag;

		Detail_Visit::visitDimension<N - 1>(
			input.extent(), 
			input.cursor(Point<N, integer>(0)), 
			visitor,
			Tag());
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
			Detail_Visit::NormalTag,
			Detail_Visit::TerminateTag>::type Tag;

		Detail_Visit::visitDimension<N - 1>(
			left.extent(),
			left.constCursor(Point<N, integer>(0)),
			right.cursor(Point<N, integer>(0)),
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
			Detail_Visit::NormalTag,
			Detail_Visit::TerminateTag>::type Tag;

		Detail_Visit::visitDimension<N - 1>(
			left.extent(),
			left.cursor(Point<N, integer>(0)),
			right.cursor(Point<N, integer>(0)),
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
			Detail_Visit::NormalTag,
			Detail_Visit::TerminateTag>::type Tag;

		Detail_Visit::visitDimension<N - 1>(
			left.extent(),
			left.constCursor(Point<N, integer>(0)),
			right.constCursor(Point<N, integer>(0)),
			visitor,
			Tag());
	}

	namespace Detail_VisitPosition
	{

		class NormalTag {};
		class TerminateTag {};

		// Single view traversal

		template <int Index, int N, typename Cursor, typename VisitPositionFunctor>
		void visitPositionDimension(
			const Vector<N, integer>& extent,
			const Point<N, integer>& startPosition,
			const Cursor& startCursor,
			const VisitPositionFunctor& visitPosition,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Cursor cursor = startCursor;
			Point<N, integer> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitPositionDimension<Index - 1>(
					extent, position, cursor, visitPosition, Tag());
				
				cursor.increment<Index>();
				++position[Index];
			}
		}

		template <int Index, int N, typename Cursor, typename VisitPositionFunctor>
		void visitPositionDimension(
			const Vector<N, integer>& extent,
			const Point<N, integer>& startPosition,
			const Cursor& startCursor,
			const VisitPositionFunctor& visitPosition,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Cursor cursor = startCursor;
			Point<N, integer> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				visitPosition(position, *cursor);

				cursor.increment<Index>();
				++position[Index];
			}
		}

		// Two-view parallel traversal

		template <int Index, int N, 
			typename Left_Cursor,
			typename Right_Cursor,
			typename VisitPositionFunctor>
		void visitPositionDimension(
			const Vector<N, integer>& extent,
			const Point<N, integer>& startPosition,
			const Left_Cursor& leftStartCursor,
			const Right_Cursor&  rightStartCursor,
			const VisitPositionFunctor& visitPosition,
			NormalTag)
		{
			ASSERT2(Index > 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Left_Cursor leftCursor = leftStartCursor;
			Right_Cursor rightCursor = rightStartCursor;
			Point<N, integer> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				typedef typename boost::mpl::if_c<
					(Index > 1),
					NormalTag,
					TerminateTag>::type Tag;

				visitPositionDimension<Index - 1>(
					extent, position, leftCursor, rightCursor, visitPosition, Tag());
				
				leftCursor.increment<Index>();
				rightCursor.increment<Index>();
				++position[Index];
			}
		}

		template <int Index, int N, 
			typename Left_Cursor, 
			typename Right_Cursor,
			typename VisitPositionFunctor>
		void visitPositionDimension(
			const Vector<N, integer>& extent,
			const Point<N, integer>& startPosition,
			const Left_Cursor& leftStartCursor,
			const Right_Cursor& rightStartCursor,
			const VisitPositionFunctor& visitPosition,
			TerminateTag)
		{
			ASSERT2(Index >= 0 && Index < N, Index, N);

			const integer width = extent[Index];

			Left_Cursor leftCursor = leftStartCursor;
			Right_Cursor rightCursor = rightStartCursor;
			Point<N, integer> position = startPosition;

			for (integer i = 0;i < width;++i)
			{
				visitPosition(position, *leftCursor, *rightCursor);

				leftCursor.increment<Index>();
				rightCursor.increment<Index>();
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
			Point<N, integer>(0),
			input.constCursor(Point<N, integer>(0)), 
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
			Point<N, integer>(0),
			input.cursor(Point<N, integer>(0)), 
			visitPosition,
			Tag());
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
			Point<N, integer>(0),
			left.constCursor(Point<N, integer>(0)),
			right.cursor(Point<N, integer>(0)),
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
			Point<N, integer>(0),
			left.cursor(Point<N, integer>(0)),
			right.cursor(Point<N, integer>(0)),
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
			Point<N, integer>(0),
			left.constCursor(Point<N, integer>(0)),
			right.constCursor(Point<N, integer>(0)),
			visitPosition,
			Tag());
	}

}

#endif
