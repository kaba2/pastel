#ifndef PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_HPP
#define PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_HPP

#include "pastel/geometry/rangetree_range_search.h"
#include "pastel/sys/binary_search.h"

#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace Pastel
{

	template <
		typename Settings, 
		template <typename> class Customization,
		typename X_Less,
		typename Y_Less,
		typename Point_ConstIterator_Output>
	integer rangeSearch(
		const RangeTree<Settings, Customization>& tree,
		const typename Settings::Point& min,
		const typename Settings::Point& max,
		const X_Less& xLess,
		const Y_Less& yLess,
		const Point_ConstIterator_Output& output)
	{
		if (tree.empty())
		{
			return 0;
		}

		static PASTEL_CONSTEXPR bool DiscardOutput =
			std::is_same<Point_ConstIterator_Output, Null_Output>::value;

		using Tree = RangeTree<Settings, Customization>;
		using Node = typename Tree::Node_ConstIterator;
		using Point = typename Tree::Point;
		using Entry = typename Tree::Entry;

		Node splitNode = tree.root();

		// Find the node at which search-paths for
		// the minimum and the maximum separate.
		while (true)
		{
			if (!xLess(min, *splitNode->split()))
			{
				splitNode = splitNode->child(true);
			}
			else if (!xLess(*splitNode->split(), max))
			{
				splitNode = splitNode->child(false);
			}
			else
			{
				break;
			}
		}

		// Find out the first point on the
		// correct y-range. After this there is no need
		// to search for the y-range due to fractional
		// cascading.

		auto indicator = [&](integer i)
		{
			return yLess(min, *splitNode->entryRange()[i].point());
		};

		integer yIndex = binarySearch(
			(integer)0, splitNode->entries(),
			indicator);

		auto report = [&](const Node& node, integer yStart)
		{
			if (DiscardOutput)
			{
				return;
			}

			integer n = node->entries();
			auto entrySet = node->entryRange();
			for (integer i = yStart; i < n && yLess(*entrySet[i].point(), max);++i)
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

			Node node = splitNode->child(right);
			while (!node->isEnd())
			{
				bool goRight = xLess(*node->split(), extremum);

				Node child = node->child(goRight);
				Node sibling = node->child(!goRight);

				if (goRight == right)
				{
					// Use fractional cascading to find the 
					// sibling's minimum y-index.
					integer siblingIndex = 
						node->entryRange()[yIndex].cascade(goRight);

					// Report the points in the sibling node.
					report(sibling, siblingIndex);
				}

				if (child->isEnd())
				{
					// Report the points in the leaf node.
					report(node, yIndex);
				}
				else
				{
					// Use fractional cascading to find the 
					// child's minimum y-index.
					yIndex = node->entryRange()[yIndex].cascade(!goRight);
				}

				node = child;
			}
		}

		return 0;
	}

}

#endif
