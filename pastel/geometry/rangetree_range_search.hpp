#ifndef PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_HPP
#define PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_HPP

#include "pastel/geometry/rangetree_range_search.h"
#include "pastel/sys/binary_search.h"

#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace Pastel
{

	namespace Range_Search_
	{

		template <
			typename Settings,
			template <typename> class Customization,
			typename Point_ConstIterator_Output>
		integer rangeSearch(
			const RangeTree<Settings, Customization>& tree,
			const typename Settings::Point& min,
			const typename Settings::Point& max,
			const Point_ConstIterator_Output& output,
			const typename RangeTree<Settings, Customization>::Node_ConstIterator& node,
			integer depth)
		{
			// Due to the 2d range-tree being treated as a whole,
			// the maximum depth is tree.orders() - 2.
			ASSERT_OP(depth, <, tree.orders() - 1);

			// Invariant: the 'node' is the root of some 
			// tree in the range tree.

			// We assume that the search interval is given by
			// {x : min <= x <= max}, i.e. a closed interval.

			static PASTEL_CONSTEXPR bool DiscardOutput =
				std::is_same<Point_ConstIterator_Output, Null_Output>::value;

			using Tree = RangeTree<Settings, Customization>;
			using Fwd = Tree;
			PASTEL_FWD(MultiLess);
			PASTEL_FWD(Point);
			PASTEL_FWD(Entry);
			PASTEL_FWD(Node_ConstIterator);

			MultiLess multiLess;

			bool lastLevel = node->isBottom();

			// Find the node at which the search-paths for
			// the minimum and the maximum separate.
			Node_ConstIterator splitNode = node;
			while (!splitNode->isLeaf())
			{
				// Invariant: the split-node is not an end-node.
				ASSERT(!splitNode->isEnd());

				// The left child contains all points
				// < splitNode->split().
				
				// The right child contains all points
				// >= splitNode->split().

				// If min >= splitNode->split(),
				// only points on the right child need 
				// to be reported.
				bool onRight =
					!multiLess(min, *splitNode->split(), depth);

				if (!onRight)
				{
					// Now min < splitNode->split().

					// If max < splitNode->split(),
					// only points on the left child need 
					// to be reported.

					bool onLeft =
						multiLess(max, *splitNode->split(), depth);

					if (!onLeft)
					{
						// Now min < splitNode->split() <= max.

						// Therefore this is the node where the 
						// search-paths split.
						break;
					}
				}

				// Continue from the next child.
				// Note that both children of a non-leaf 
				// node exist.
				splitNode = splitNode->child(onRight);
			}

			auto report = [&](
				const Node_ConstIterator& node,
				integer start)
			{
				ASSERT(node->isBottom());

				if (DiscardOutput)
				{
					return;
				}

				auto entrySet = node->entryRange();
				integer n = node->entries();

				ASSERT(start == n ||
					!multiLess(*entrySet[start].point(), min, tree.orders() - 1));

				// Report, in the lowest tree, all points which
				// fall into the [min, max] interval in the with
				// respect to the last strict weak order. Since
				// we get the index of first such point from 'lastStart',
				// due to fractional cascading, and the points are
				// ordered with respect to the last order, we need only 
				// report the points sequentially until we come
				// up to a point which falls outside the interval.
				
				for (integer i = start; i < n && 
					!multiLess(max, *entrySet[i].point(), tree.orders() - 1); ++i)
				{
					output(entrySet[i].point());
				}
			};

			auto recurse = [&](
				const Node_ConstIterator& node,
				integer start)
			{
				if (lastLevel)
				{
					// Report the points.
					report(node, start);
				}
				else
				{
					// Recurse down.
					rangeSearch(tree, min, max, output, 
						node->down(), depth + 1);
				}
			};

			integer splitStart = 0;
			if (lastLevel)
			{
				// Find out the first point >= min 
				// with respect to the last order. 

				auto indicator = [&](integer i)
				{
					return !multiLess(
						*splitNode->entryRange()[i].point(), 
						min, tree.orders() - 1);
				};

				splitStart = binarySearch(
					(integer)0, splitNode->entries(),
					indicator);

				// After this there is no need to search for the 
				// last range due to fractional cascading. This is 
				// important to achieving O(log(n)^(d - 1)) complexity;
				// otherwise the complexity is O(log(n)^d).
			}

			if (splitNode->isLeaf())
			{
				// If the split-node is a leaf, i.e.
				// the search-paths of the minimum and
				// the maximum are the same, then just 
				// report the split-node.

				if (!multiLess(*splitNode->split(), min, depth) &&
					!multiLess(max, *splitNode->split(), depth))
				{
					recurse(splitNode, splitStart);
				}
				
				return 0;
			}

			// Report the subtrees between the search-paths
			// of min and max.
			const Point* extremumSet[] = { &min, &max };
			for (bool right : {false, true})
			{
				const Point& extremum = *extremumSet[right];

				Node_ConstIterator node = splitNode->child(right);
				integer lastStart = splitNode->entryRange()[splitStart].cascade(right);
				while (!node->isLeaf())
				{
					bool goRight = !multiLess(extremum, *node->split(), depth);

					Node_ConstIterator child = node->child(goRight);
					Node_ConstIterator sibling = node->child(!goRight);

					integer childStart = 0;
					integer siblingStart = 0;
					if (lastLevel)
					{
						// Find out the indices of the children by
						// fractional cascading.
						childStart = node->entryRange()[lastStart].cascade(goRight);
						siblingStart = node->entryRange()[lastStart].cascade(!goRight);
					}

					if (goRight == right)
					{
						// Recurse to the sibling node.
						recurse(sibling, siblingStart);
					}

					lastStart = childStart;
					node = child;
				}

				ASSERT(node->isLeaf());

				// In a leaf node, all the points are equivalent
				// with respect to the current order. The 'split'
				// field contains one of these points, and therefore
				// can be used to decide whether to report none,
				// or all, of the points.
				if (!multiLess(*node->split(), min, depth) &&
					!multiLess(max, *node->split(), depth))
				{
					// All of the points in the leaf node are
					// in the range with respect to the current
					// order.

					// Recurse to the leaf node.
					recurse(node, lastStart);
				}
			}

			return 0;
		}

	}

	template <
		typename Settings,
		template <typename> class Customization,
		typename Point_ConstIterator_Output>
	integer rangeSearch(
		const RangeTree<Settings, Customization>& tree,
		const typename Settings::Point& min,
		const typename Settings::Point& max,
		const Point_ConstIterator_Output& output)
	{
		if (tree.empty())
		{
			return 0;
		}

		return Range_Search_::rangeSearch(tree, min, max, output, tree.root(), 0);
	}

}

#endif
