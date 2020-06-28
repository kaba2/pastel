#ifndef PASTELSYS_VIEW_VISIT_MORE2_HPP
#define PASTELSYS_VIEW_VISIT_MORE2_HPP

#include "pastel/sys/view/view_visit_more2.h"
#include "pastel/sys/view/rowview.h"

namespace Pastel
{

	namespace VisitRows_
	{

		template <int N, typename Contained_View, typename RowVisitorFunctor>
		class ConstRowViewVisitor
		{
		public:
			explicit ConstRowViewVisitor(
				const RowVisitorFunctor& rowVisitor,
				const Contained_View& view,
				integer freeIndex)
				: rowVisitor_(rowVisitor)
				, view_(view)
				, freeIndex_(freeIndex)
			{
			}

			void operator()(
				const Vector<integer, N>& position) const
			{
				rowVisitor_(
					constRowView(view_, freeIndex_, position));
			}

		private:
			const RowVisitorFunctor& rowVisitor_;
			const Contained_View& view_;
			integer freeIndex_;
		};

		template <int N, typename Contained_View, typename RowVisitorFunctor>
		class RowViewVisitor
		{
		public:
			explicit RowViewVisitor(
				const RowVisitorFunctor& rowVisitor,
				const Contained_View& view,
				integer freeIndex)
				: rowVisitor_(rowVisitor)
				, view_(view)
				, freeIndex_(freeIndex)
			{
			}

			void operator()(
				const Vector<integer, N>& position) const
			{
				rowVisitor_(
					rowView(view_, freeIndex_, position));
			}

		private:
			const RowVisitorFunctor& rowVisitor_;
			const Contained_View& view_;
			integer freeIndex_;
		};

	}

	template <int N, 
		typename Input_Element, typename Input_ConstView,
		typename RowVisitorFunctor>
		void visitRows(const ConstView<N, Input_Element, Input_ConstView>& view,
		integer freeIndex,
		const RowVisitorFunctor& rowVisitor)
	{
		PASTEL_STATIC_ASSERT(N >= 1);
		ENSURE2(freeIndex >= 0 && freeIndex < N, freeIndex, N);

		AlignedBox<integer, N> rectangle(Vector<integer, N>(0), view.extent());

		if (rectangle.empty())
		{
			return;
		}

		rectangle.max()[freeIndex] = 1;

		VisitRows_::ConstRowViewVisitor<
			N, ConstView<N, Input_Element, Input_ConstView>, 
			RowVisitorFunctor>
			rowViewVisitor(rowVisitor, view, freeIndex);

		visit(rectangle, rowViewVisitor);
	}

	template <int N, 
		typename Input_Element, typename Input_View,
		typename RowVisitorFunctor>

		void visitRows(const View<N, Input_Element, Input_View>& view,
		integer freeIndex,
		const RowVisitorFunctor& rowVisitor)
	{
		PASTEL_STATIC_ASSERT(N >= 1);
		ENSURE2(freeIndex >= 0 && freeIndex < N, freeIndex, N);

		AlignedBox<integer, N> rectangle(Vector<integer, N>(0), view.extent());

		if (rectangle.empty())
		{
			return;
		}

		rectangle.max()[freeIndex] = 1;

		VisitRows_::RowViewVisitor<N, 
			View<N, Input_Element, Input_View>, 
			RowVisitorFunctor>
			rowViewVisitor(rowVisitor, view, freeIndex);

		visit(rectangle, rowViewVisitor);
	}

	namespace VisitRows_
	{

		template <int N, 
			typename Left_View, 
			typename Right_View,
			typename RowVisitorFunctor>
		class ConstConstRowViewVisitor2
		{
		public:
			explicit ConstConstRowViewVisitor2(
				const RowVisitorFunctor& rowVisitor,
				const Left_View& left,
				const Right_View& right,
				integer freeIndex)
				: rowVisitor_(rowVisitor)
				, left_(left)
				, right_(right)
				, freeIndex_(freeIndex)
			{
			}

			void operator()(
				const Vector<integer, N>& position) const
			{
				rowVisitor_(
					constRowView(left_, freeIndex_, position),
					constRowView(right_, freeIndex_, position));
			}

		private:
			const RowVisitorFunctor& rowVisitor_;
			const Left_View& left_;
			const Right_View& right_;
			integer freeIndex_;
		};

		template <int N, 
			typename Left_View, 
			typename Right_View,
			typename RowVisitorFunctor>
		class ConstRowViewVisitor2
		{
		public:
			explicit ConstRowViewVisitor2(
				const RowVisitorFunctor& rowVisitor,
				const Left_View& left,
				const Right_View& right,
				integer freeIndex)
				: rowVisitor_(rowVisitor)
				, left_(left)
				, right_(right)
				, freeIndex_(freeIndex)
			{
			}

			void operator()(
				const Vector<integer, N>& position) const
			{
				rowVisitor_(
					constRowView(left_, freeIndex_, position),
					rowView(right_, freeIndex_, position));
			}

		private:
			const RowVisitorFunctor& rowVisitor_;
			const Left_View& left_;
			const Right_View& right_;
			integer freeIndex_;
		};

		template <int N, 
			typename Left_View, 
			typename Right_View,
			typename RowVisitorFunctor>
		class RowViewVisitor2
		{
		public:
			explicit RowViewVisitor2(
				const RowVisitorFunctor& rowVisitor,
				const Left_View& left,
				const Right_View& right,
				integer freeIndex)
				: rowVisitor_(rowVisitor)
				, left_(left)
				, right_(right)
				, freeIndex_(freeIndex)
			{
			}

			void operator()(
				const Vector<integer, N>& position) const
			{
				rowVisitor_(
					rowView(left_, freeIndex_, position),
					rowView(right_, freeIndex_, position));
			}

		private:
			const RowVisitorFunctor& rowVisitor_;
			const Left_View& left_;
			const Right_View& right_;
			integer freeIndex_;
		};

	}

	template <int N, 
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename RowVisitorFunctor>
		void visitRows(
			const View<N, Left_Element, Left_View>& left,
			const View<N, Right_Element, Right_View>& right,
			integer freeIndex,
			const RowVisitorFunctor& rowVisitor)
	{
		PASTEL_STATIC_ASSERT(N >= 1);
		ENSURE2(freeIndex >= 0 && freeIndex < N, freeIndex, N);

		AlignedBox<integer, N> leftRectangle(
			Vector<integer, N>(0),
			left.extent());

		AlignedBox<integer, N> rightRectangle(
			Vector<integer, N>(0),
			right.extent());

		if (leftRectangle.empty() ||
			rightRectangle.empty())
		{
			return;
		}

		leftRectangle.max()[freeIndex] = 1;
		rightRectangle.max()[freeIndex] = 1;

		ENSURE(leftRectangle.max() == rightRectangle.max());

		VisitRows_::RowViewVisitor2<N, 
			View<N, Left_Element, Left_View>, 
			View<N, Right_Element, Right_View>, 
			RowVisitorFunctor>
			rowViewVisitor(rowVisitor, left, right, freeIndex);

		visit(leftRectangle, rowViewVisitor);
	}

	template <int N, 
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename RowVisitorFunctor>
		void visitRows(
			const ConstView<N, Left_Element, Left_View>& left,
			const View<N, Right_Element, Right_View>& right,
			integer freeIndex,
			const RowVisitorFunctor& rowVisitor)
	{
		PASTEL_STATIC_ASSERT(N >= 1);
		ENSURE2(freeIndex >= 0 && freeIndex < N, freeIndex, N);

		AlignedBox<integer, N> leftRectangle(
			Vector<integer, N>(0),
			left.extent());

		AlignedBox<integer, N> rightRectangle(
			Vector<integer, N>(0),
			right.extent());

		if (leftRectangle.empty() ||
			rightRectangle.empty())
		{
			return;
		}

		leftRectangle.max()[freeIndex] = 1;
		rightRectangle.max()[freeIndex] = 1;

		ENSURE(leftRectangle.max() == rightRectangle.max());

		VisitRows_::ConstRowViewVisitor2<N, 
			ConstView<N, Left_Element, Left_View>, 
			View<N, Right_Element, Right_View>, 
			RowVisitorFunctor>
			rowViewVisitor(rowVisitor, left, right, freeIndex);

		visit(leftRectangle, rowViewVisitor);
	}

	template <int N, 
		typename Left_Element, typename Left_View,
		typename Right_Element, typename Right_View,
		typename RowVisitorFunctor>
		void visitRows(
			const ConstView<N, Left_Element, Left_View>& left,
			const ConstView<N, Right_Element, Right_View>& right,
			integer freeIndex,
			const RowVisitorFunctor& rowVisitor)
	{
		PASTEL_STATIC_ASSERT(N >= 1);
		ENSURE2(freeIndex >= 0 && freeIndex < N, freeIndex, N);

		AlignedBox<integer, N> leftRectangle(
			Vector<integer, N>(0),
			left.extent());

		AlignedBox<integer, N> rightRectangle(
			Vector<integer, N>(0),
			right.extent());

		if (leftRectangle.empty() ||
			rightRectangle.empty())
		{
			return;
		}

		leftRectangle.max()[freeIndex] = 1;
		rightRectangle.max()[freeIndex] = 1;

		ENSURE(leftRectangle.max() == rightRectangle.max());

		VisitRows_::ConstConstRowViewVisitor2<N, 
			ConstView<N, Left_Element, Left_View>, 
			ConstView<N, Right_Element, Right_View>, 
			RowVisitorFunctor>
			rowViewVisitor(rowVisitor, left, right, freeIndex);

		visit(leftRectangle, rowViewVisitor);
	}

}

#endif
