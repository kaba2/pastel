#ifndef PASTEL_BIHTREETOOLS_HPP
#define PASTEL_BIHTREETOOLS_HPP

#include "pastel/geometry/bihtree_tools.h"
#include "pastel/geometry/contains_alignedbox_alignedbox.h"
#include "pastel/geometry/intersect_line_alignedplane.h"
#include "pastel/sys/log.h"

#include <iostream>

namespace Pastel
{

	namespace Detail
	{

		template <typename Real, int N, typename Shape,
			typename ShapeBoundQuery>
			bool check(
			const BihTree<Real, N, Shape>& bihTree,
			const typename BihTree<Real, N, Shape>::
			ConstCursor& cursor,
			const AlignedBox<Real, N>& alignedBox,
			const ShapeBoundQuery& shapeQuery)
		{
			typedef typename BihTree<Real, N, Shape>::
				ConstCursor ConstCursor;

			if (cursor.isLeaf())
			{
				// If this is a leaf node,
				// check that all the contained shapes
				// are geometrically inside the
				// node aligned box.

				ConstMemoryView<Shape> shapes(
					bihTree.shapes(
					bihTree.leaf(cursor)));
				for (integer i = 0;i < shapes.size();++i)
				{
					if (REPORT(!contains(alignedBox,
						shapeQuery.boundingAlignedBox(shapes[i]))))
					{
						return false;
					}
				}
			}
			else
			{
				// This is an intermediate node.

				const integer splitAxis = cursor.splitAxis();

				// Check that the negative split position
				// is inside the node aligned box.
				const Real negativeSplit(cursor.negativeSplit());
				if (REPORT4(
					negativeSplit < alignedBox.min()[splitAxis] ||
					negativeSplit > alignedBox.max()[splitAxis],
					splitAxis,
					negativeSplit, alignedBox.min()[splitAxis],
					alignedBox.max()[splitAxis]))
				{
					log() << "negativeSplit = " << negativeSplit
						<< ", min = " << alignedBox.min()[splitAxis]
						<< ", max = " << alignedBox.max()[splitAxis] << logNewLine;
					return false;
				}

				// Recurse to the negative child.

				AlignedBox<Real, N> negativeAlignedBox(alignedBox);

				Vector<Real, N> maxTmp(negativeAlignedBox.max());
				maxTmp[splitAxis] = negativeSplit;
				negativeAlignedBox.setMax(maxTmp);

				ConstCursor negativeChild(cursor);
				negativeChild.nextNegative();

				if (!check(bihTree, negativeChild,
					negativeAlignedBox, shapeQuery))
				{
					return false;
				}

				// Check that the positive split position
				// is inside the node aligned box.

				Real positiveSplit(cursor.positiveSplit());
				if (REPORT4(
					positiveSplit < alignedBox.min()[splitAxis] ||
					positiveSplit > alignedBox.max()[splitAxis],
					splitAxis,
					positiveSplit, alignedBox.min()[splitAxis],
					alignedBox.max()[splitAxis]))
				{
					log() << "positiveSplit = " << positiveSplit
						<< ", min = " << alignedBox.min()[splitAxis]
						<< ", max = " << alignedBox.max()[splitAxis] << logNewLine;
					return false;
				}

				// Recurse to the positive child.

				AlignedBox<Real, N> positiveAlignedBox(alignedBox);
				positiveAlignedBox.min()[splitAxis] = positiveSplit;

				ConstCursor positiveChild(cursor);
				positiveChild.nextPositive();

				if (!check(bihTree, positiveChild,
					positiveAlignedBox, shapeQuery))
				{
					return false;
				}
			}

			return true;
		}

	}

	template <typename Real, int N, typename Shape,
		typename ShapeAlignedBoxQuery>
		bool check(const BihTree<Real, N, Shape>& bihTree,
		const ShapeAlignedBoxQuery& shapeQuery)
	{
		typedef typename BihTree<Real, N, Shape>::ConstCursor
			ConstCursor;

		ConstCursor node(bihTree.root());
		return Detail::check(bihTree, node,
			bihTree.boundingAlignedBox(), shapeQuery);
	}

	namespace Detail
	{

		template <typename Real, int N, typename Shape>
		class NodeRange
		{
		public:
			typedef typename
				BihTree<Real, N, Shape>::ConstCursor
				ConstCursor;

			NodeRange()
				: node_()
				, tMin_()
				, tMax_()
			{
			}

			NodeRange(
				const ConstCursor& node,
				const Real& tMin,
				const Real& tMax)
				: node_(node)
				, tMin_(tMin)
				, tMax_(tMax)
			{
			}

			ConstCursor node_;
			Real tMin_;
			Real tMax_;
		};

	}

	template <typename Real, int N, typename Shape,
		typename IntersectionQuery>
		bool intersect(
		const BihTree<Real, N, Shape>& bihTree,
		const Line<Real, N>& line,
		IntersectionQuery& shapeQuery)
	{
		if (bihTree.empty())
		{
			// If the bih tree is empty,
			// the line can't intersect
			// any object.
			return false;
		}

		typedef Detail::NodeRange<Real, N, Shape> Range;
		typedef typename Range::ConstCursor ConstCursor;

		Vector<Real, 2> tRange;
		bool intersections = Pastel::intersect(
			line, bihTree.boundingAlignedBox(), tRange);
		if (!intersections)
		{
			// If the line does not intersect
			// the bih tree's bounding aligned box,
			// then it can't intersect any object
			// either.
			return false;
		}

		// This array contains the nodes to be
		// traversed in a stack (depth-first
		// traversal).

		std::vector<Range> nodeStack;

		// Traversal starts from the root
		// and proceeds top-down.
		nodeStack.push_back(
			Range(bihTree.root(),
			tRange[0], tRange[1]));

		bool intersectionFound = false;

		while (!nodeStack.empty())
		{
			// Pick a node and its range from the
			// stack.

			Range nodeRange(nodeStack.back());
			nodeStack.pop_back();

			// Pick up the variables of the
			// range to get shorter names.
			const Real tMin(nodeRange.tMin_);
			const Real tMax(nodeRange.tMax_);
			const ConstCursor node(nodeRange.node_);

			if (node.isLeaf())
			{
				//std::cout << "(" << tMin << ", " << tMax << "), ";

				// This node is a leaf node.
				// Test intersections with its shapes.

				ConstMemoryView<Shape> table(
					bihTree.shapes(
					bihTree.leaf(node)));

				if (shapeQuery.intersect(table, line))
				{
					intersectionFound = true;
				}
			}
			else
			{
				// This is an intermediate node.
				// Recurse deeper into the tree.

				const integer splitAxis = node.splitAxis();

				const Real atMin(line.position()[splitAxis] +
					line.direction()[splitAxis] * tMin);
				const Real atMax(line.position()[splitAxis] +
					line.direction()[splitAxis] * tMax);

				// Compute the negative child range

				const bool isMinInNegativeSide =
					atMin <= node.negativeSplit();
				const bool isMaxInNegativeSide =
					atMax <= node.negativeSplit();

				Real tNegativeFrom(tMin);
				Real tNegativeTo(tMax);

				const bool negativeIntersection =
					isMinInNegativeSide ^
					isMaxInNegativeSide;

				if (negativeIntersection)
				{
					Real tNegativeSplit;
					const bool result = Pastel::intersect(line,
						AlignedPlane3(node.negativeSplit(), splitAxis),
						tNegativeSplit);
					ENSURE(result);
					if (REPORT(tNegativeSplit < tMin ||
						tNegativeSplit > tMax))
					{
						log() << "tNegativeSplit = " << tNegativeSplit
							<< ", tMin = " << tMin
							<< ", tMax = " << tMax << logNewLine;
					}
					if (isMinInNegativeSide)
					{
						tNegativeTo = tNegativeSplit;
					}
					else
					{
						tNegativeFrom = tNegativeSplit;
					}
				}

				const bool goesThroughNegativeChild =
					isMinInNegativeSide ||
					isMaxInNegativeSide;

				// Compute the positive child range

				const bool isMinInPositiveSide =
					atMin >= node.positiveSplit();
				const bool isMaxInPositiveSide =
					atMax >= node.positiveSplit();

				Real tPositiveFrom(tMin);
				Real tPositiveTo(tMax);

				const bool positiveIntersection =
					isMinInPositiveSide ^
					isMaxInPositiveSide;

				if (positiveIntersection)
				{
					Real tPositiveSplit;
					const bool result = Pastel::intersect(line,
						AlignedPlane3(node.positiveSplit(), splitAxis),
						tPositiveSplit);
					ENSURE(result);
					if (REPORT(tPositiveSplit < tMin ||
						tPositiveSplit > tMax))
					{
						log() << "tPositiveSplit = " << tPositiveSplit
							<< ", tMin = " << tMin
							<< ", tMax = " << tMax << logNewLine;
					}
					if (isMinInPositiveSide)
					{
						tPositiveTo = tPositiveSplit;
					}
					else
					{
						tPositiveFrom = tPositiveSplit;
					}
				}

				const bool goesThroughPositiveChild =
					isMinInPositiveSide ||
					isMaxInPositiveSide;

				ConstCursor negativeNode(node);
				negativeNode.nextNegative();

				ConstCursor positiveNode(node);
				positiveNode.nextPositive();

				if (goesThroughNegativeChild)
				{
					if (goesThroughPositiveChild)
					{
						if (tNegativeFrom < tPositiveFrom)
						{
							nodeStack.push_back(
								Range(positiveNode,
								tPositiveFrom,
								tPositiveTo));
							nodeStack.push_back(
								Range(negativeNode,
								tNegativeFrom,
								tNegativeTo));
						}
						else
						{
							nodeStack.push_back(
								Range(negativeNode,
								tNegativeFrom,
								tNegativeTo));
							nodeStack.push_back(
								Range(positiveNode,
								tPositiveFrom,
								tPositiveTo));
						}
					}
					else
					{
						nodeStack.push_back(
							Range(negativeNode,
							tNegativeFrom,
							tNegativeTo));
					}
				}
				else
				{
					if (goesThroughPositiveChild)
					{
						nodeStack.push_back(
							Range(positiveNode,
							tPositiveFrom,
							tPositiveTo));
					}
					else
					{
						// The line intersect the empty region.
					}
				}
			}
		}

		return intersectionFound;
	}

}

#endif
