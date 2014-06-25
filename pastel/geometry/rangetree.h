// Description: Range tree

#ifndef PASTELGEOMETRY_RANGETREE_H
#define PASTELGEOMETRY_RANGETREE_H

#include "pastel/geometry/rangetree_concepts.h"
#include "pastel/geometry/rangetree_fwd.h"
#include "pastel/sys/less_concept.h"
#include "pastel/sys/dereferenced_predicate.h"
#include "pastel/sys/vector.h"

#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/stable_sort.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/range/algorithm/unique.hpp>

#include <algorithm>
#include <memory>
#include <vector>

namespace Pastel
{

	template <typename Settings>
	using Empty_RangeTree_Customization = 
		RangeTree_Concepts::Customization<Settings>;

	//! Range tree
	/*!
	Space complexity: O(n log(n))
	*/
	template <
		typename Settings,
		template <typename> class Customization = Empty_RangeTree_Customization>
	class RangeTree
	{
	public:
		using Fwd = RangeTree_Fwd<Settings>;
		PASTEL_FWD(PointSet);
		PASTEL_FWD(Point_Iterator);
		PASTEL_FWD(Point_ConstIterator);
		PASTEL_FWD(Point);
		PASTEL_FWD(Entry);
		PASTEL_FWD(Node);
		PASTEL_FWD(Node_Iterator);
		PASTEL_FWD(Node_ConstIterator);

		RangeTree()
		: end_(new Node)
		, root_(end_.get())
		, pointSet_()
		{
		}

		/*
		template <
			typename Type, 
			typename = PASTEL_ENABLE_IF((std::is_convertible<Type, Point>), void)>
		RangeTree(const std::initializer_list<Type>& pointSet)
		: RangeTree()
		{
			construct(pointSet)
		}
		*/

		template <
			typename Point_Range_,
			typename X_Less,
			typename Y_Less>
		RangeTree(
			const Point_Range_& pointSet,
			const X_Less& xLess,
			const Y_Less& yLess)
		: RangeTree()
		{
			integer n = boost::size(pointSet);

			std::vector<Point_Iterator> iteratorSet;
			iteratorSet.reserve(n);
			pointSet_.reserve(n);
	
			for (auto&& point : pointSet)
			{
				pointSet_.emplace_back(point);
				iteratorSet.emplace_back(
					std::prev(pointSet_.end()));
			}

			auto yLess_ = [&](
				const Point_Iterator& left, 
				const Point_Iterator& right)
			{
				return yLess(*left, *right);
			};

			// Sort the points in increasing order by the y-coordinate.
			boost::stable_sort(iteratorSet, yLess_);

			root_ = construct(0, false, iteratorSet, xLess, yLess).get();
		}

		~RangeTree()
		{
			clear();
		}

		void swap(RangeTree& that)
		{
			end_.swap(that.end_);
			std::swap(root_, that.root_);
			elementSet_.swap(that.elementSet_);
		}

		void clear()
		{
			clear(root_);
		}

		bool empty() const
		{
			return root_ == end_.get();
		}

		Node_ConstIterator root() const
		{
			return root_;
		}

		Node_ConstIterator end() const
		{
			return end_.get();
		}

	private:
		template <
			typename X_Less,
			typename Y_Less>
		std::unique_ptr<Node> construct(
			Node_Iterator parent,
			bool right,
			std::vector<Point_Iterator>& pointSet,
			const X_Less& xLess,
			const Y_Less& yLess)
		{
			// Invariant:
			// The points in 'pointSet' are in
			// increasing order in the y-coordinate.

			std::unique_ptr<Node> node(
				new Node(Point_Iterator(), pointSet));

			if (parent)
			{
				// Compute the fractional cascading links.
				integer j = 0;
				for (integer i = 0;i < parent->entrySet_.size();++i)
				{
					Entry& entry = parent->entrySet_[i];
					
					if (j < pointSet.size() &&
						yLess(*pointSet[j], *entry.point()))
					{
						++j;
					}

					ASSERT(j == pointSet.size() ||
						!yLess(*pointSet[j], *entry.point()));

					entry.cascade(right) = j;
				}
			}

			// Sort the points in lexicographical order.

			auto xyLess = [&](
				const Point_Iterator& left,
				const Point_Iterator& right)
			-> bool
			{
				if (xLess(*left, *right))
				{
					return true;
				}

				if (xLess(*right, *left))
				{
					return false;
				}

				return yLess(*left, *right);
			};

			std::vector<Point_Iterator> sortedSet(pointSet);
			boost::sort(sortedSet, xyLess);

			if (!xyLess(*pointSet.begin(), *std::prev(pointSet.end())))
			{
				// The points are all in the same position.
				// Create a leaf node.
				std::unique_ptr<Node> node(
					new Node(Point_Iterator(), pointSet));
				return node;
			}

			// There are several different points.

			// Remove multiple points from the same positions.
			auto uniqueEnd = 
				std::unique(sortedSet.begin(), sortedSet.end());

			// Pick the median of the unique points.
			Point_Iterator median =
				*(sortedSet.begin() + (std::distance(sortedSet.begin(), uniqueEnd) / 2));

			node->split_ = median;

			// Partition with respect to the median in
			// lexicographical order.

			auto lessMedian = [&](const Point_Iterator& point)
			-> bool
			{
				return xyLess(point, median);
			};

			auto leftEnd = boost::stable_partition(pointSet, lessMedian);
			integer nLeft = std::distance(pointSet.begin(), leftEnd);

			// Recurse to the right child.
			std::vector<Point_Iterator> rightSet(
				leftEnd, pointSet.end());
			node->child(true) = construct(node.get(), true, rightSet, xLess, yLess).get();

			// Recurse to the left child.
			std::vector<Point_Iterator> leftSet = std::move(pointSet);
			leftSet.resize(nLeft);
			node->child(false) = construct(node.get(), false, leftSet, xLess, yLess).get();

			// Return the node.
			return node;
		}

		void clear(Node_Iterator node)
		{
			if (node == end_.get())
			{
				return;
			}
			
			clear(node->child(false));
			clear(node->child(true));
			delete node;
		}

		std::unique_ptr<Node> end_;
		Node_Iterator root_;
		PointSet pointSet_;
	};

}

#include "pastel/geometry/rangetree_range_search.h"

#endif
