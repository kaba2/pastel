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
			ASSERT_OP(depth, <, tree.orders());

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

			// Find the node at which the search-paths for
			// the minimum and the maximum separate.
			Node_ConstIterator splitNode = node;
			while (true)
			{
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
					// only points on the right child need 
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

				// Find out the next child.
				Node_ConstIterator next = splitNode->child(onRight);
				if (next->isEnd())
				{
					// The next child does not exist. We are done.
					break;
				}

				// Continue from the next child.
				splitNode = next;
			}

			bool lastLevel = splitNode->isBottom();

			integer lastStart = 0;
			if (lastLevel)
			{
				// Find out the first point >= min 
				// on the last range. 

				auto indicator = [&](integer i)
				{
					return !multiLess(
						*splitNode->entryRange()[i].point(), 
						min, tree.orders() - 1);
				};

				lastStart = binarySearch(
					(integer)0, splitNode->entries(),
					indicator);

				// After this there is no need to search for the 
				// last range due to fractional cascading. This is 
				// important to achieving O(log(n)^(d - 1)) complexity;
				// otherwise the complexity is O(log(n)^d).
			}

			auto report = [&](
				const Node_ConstIterator& node,
				integer start)
			{
				ASSERT(node->isBottom());

				// Report, in the lowest tree, all points which
				// fall into the [min, max] interval in the with
				// respect to the last strict weak order. Since
				// we get the index of first such point from 'lastStart',
				// due to fractional cascading, and the points are
				// ordered with respect to the last order, we need only 
				// report the points sequentially until we come
				// up to a point which falls outside the interval.
				
				auto entrySet = node->entryRange();
				integer n = node->entries();
				for (integer i = lastStart; i < n && !multiLess(max, *entrySet[i].point(), 1); ++i)
				{
					output(*entrySet[i].point());
				}
			};

			// Report the subtrees between the search paths
			// of min and max.
			const Point* extremumSet[] = { &min, &max };
			for (bool right : {false, true})
			{
				const Point& extremum = *extremumSet[right];

				Node_ConstIterator node = splitNode->child(right);
				while (!node->isEnd())
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
						if (!lastLevel)
						{
							// Recurse down to the sibling node.
							rangeSearch(tree, min, max, output, 
								sibling->down(), depth + 1);
						}
						else
						{
							// Report the points in the sibling node.
							report(sibling, siblingStart);
						}
					}

					if (child->isEnd())
					{
						// Report the points in the leaf node.
						report(node, lastStart);
					}

					lastStart = childStart;
					node = child;
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
