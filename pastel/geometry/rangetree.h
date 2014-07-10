// Description: Range tree

#ifndef PASTELGEOMETRY_RANGETREE_H
#define PASTELGEOMETRY_RANGETREE_H

#include "pastel/geometry/rangetree_concepts.h"
#include "pastel/geometry/rangetree_fwd.h"
#include "pastel/geometry/rangetree_entry.h"
#include "pastel/geometry/rangetree_node.h"

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
		PASTEL_FWD(Point);
		PASTEL_FWD(MultiLess);
		PASTEL_FWD(PointSet);
		PASTEL_FWD(Point_Iterator);
		PASTEL_FWD(Point_ConstIterator);
		PASTEL_FWD(Entry);
		PASTEL_FWD(Node);
		PASTEL_FWD(Node_Iterator);
		PASTEL_FWD(Node_ConstIterator);

		//! Constructs an empty tree.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		RangeTree()
		: end_(new Node)
		, root_(end_.get())
		, pointSet_()
		, orders_(0)
		{
		}

		//! Move-constructs from another tree.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		RangeTree(RangeTree&& that)
		: RangeTree()
		{
			swap(that);
		}

		//! Copy-constructs from another tree.
		/*!
		Time complexity: 
		O(n log(n)^(d - 1))
		where 
		n = that.size(),
		d = that.orders().

		Exception safety: strong
		*/
		RangeTree(const RangeTree& that)
		: RangeTree()
		{
			RangeTree copy(that.pointSet_, that.orders());
			swap(copy);
		}

		//! Constructs from a given point-set.
		/*!
		Preconditions:
		orders > 0

		Time complexity: 
		O(n log(n)^(d - 1))
		where
		n is the size of pointSet,
		d = orders.
		*/
		template <typename Point_Input>
		explicit RangeTree(
			Point_Input pointSet,
			integer orders)
		: RangeTree()
		{
			ENSURE_OP(orders, >, 0);

			orders_ = orders;
			
			std::vector<Point_Iterator> iteratorSet;
			
			integer nHint = pointSet.nHint();
			iteratorSet.reserve(nHint);
			pointSet_.reserve(nHint);
	
			while (!pointSet.empty())
			{
				pointSet_.emplace_back(pointSet());
				iteratorSet.emplace_back(
					std::prev(pointSet_.end()));
			}

			// Sort the points in increasing order by the last coordinate.

			auto lastLess = [&](
				const Point_Iterator& left, 
				const Point_Iterator& right)
			{
				return MultiLess()(*left, *right, orders - 1);
			};

			boost::stable_sort(iteratorSet, lastLess);

			root_ = construct(nullptr, false, 0, iteratorSet);
		}

		//! Destructs the tree.
		/*!
		Time complexity:
		O(n log(n)^(d - 1))
		where
		n = size(),
		d = orders().

		Exception safety: nothrow
		*/
		~RangeTree()
		{
			clear();
		}

		//! Swaps two trees.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(RangeTree& that)
		{
			end_.swap(that.end_);
			std::swap(root_, that.root_);
			elementSet_.swap(that.elementSet_);
			std::swap(orders_, that.orders_);
		}

		//! Removes all points from the tree.
		/*!
		Time complexity:
		O(n log(n)^(d - 1))
		where
		n = size(),
		d = orders().

		Exception safety: nothrow
		*/
		void clear()
		{
			clear(root_);
			root_ = end_.get();
			pointSet_.clear();
		}

		//! Returns whether the tree is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return root_ == end_.get();
		}

		//! Returns the number of points in the tree.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return pointSet_.size();
		}

		//! Returns the number of strict weak orders.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer orders() const
		{
			return orders_;
		}

		//! Returns the root node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node_ConstIterator root() const
		{
			return root_;
		}

		//! Returns the end node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node_ConstIterator end() const
		{
			return end_.get();
		}

	private:
		Node_Iterator construct(
			Node_Iterator parent,
			bool right,
			integer depth,
			std::vector<Point_Iterator>& pointSet)
		{
			ASSERT_OP(depth, >=, 0);
			ASSERT_OP(depth, <, orders() - 1);
			ASSERT(!pointSet.empty());

			// Invariant:
			// The points in 'pointSet' are in
			// increasing order in the last coordinate.

			Node* node = 0;
			if (depth < orders() - 2)
			{
				node = new Node;
				node->isolate(end_.get());
			}
			else
			{
				// Only the level orders() - 2
				// stores a point-set.
				node = new Node(pointSet);
				node->isolate(end_.get());
			}

			MultiLess multiLess;

			// Sort the points in lexicographical order
			// with respect to <_depth.

			auto xLess = [&](
				const Point_Iterator& left,
				const Point_Iterator& right)
			-> bool
			{
				return multiLess(*left, *right, depth);
			};

			auto xEquivalent = [&](
				const Point_Iterator& left,
				const Point_Iterator& right)
			-> bool
			{
				return !xLess(left, right) &&
					!xLess(right, left);
			};

			std::vector<Point_Iterator> sortedSet(pointSet);
			boost::stable_sort(sortedSet, xLess);

			// The child nodes are created if and only if
			// the node contains points that are not equivalent
			// with respect to the current strict weak order.
			if (xLess(*sortedSet.begin(), *std::prev(sortedSet.end())))
			{
				// There are several different points.

				// Remove multiple equivalent points with 
				// respect to the current order.
				auto uniqueEnd = 
					std::unique(sortedSet.begin(), sortedSet.end(), xEquivalent);

				// Pick the median of the unique points.
				Point_Iterator median =
					*(sortedSet.begin() + (std::distance(sortedSet.begin(), uniqueEnd) / 2));

				// Set the split-point to that median.
				node->split_ = median;

				// Partition with respect to the median with 
				// respect to the current order.

				auto lessMedian = [&](const Point_Iterator& point)
				-> bool
				{
					return xLess(point, median);
				};

				// The partitioning must be stable for the children
				// to stay ordered with respect to the last order.
				auto leftEnd = boost::stable_partition(pointSet, lessMedian);

				// Recurse to the left child.
				{
					std::vector<Point_Iterator> leftSet(
						pointSet.begin(), leftEnd);
					node->child(false) = construct(node, false, depth, leftSet);
				}

				// Recurse to the right child.
				{
					std::vector<Point_Iterator> rightSet(
						leftEnd, pointSet.end());
					node->child(true) = construct(node, true, depth, rightSet);
				}
			}
			else
			{
				// The points in this node are all equivalent
				// with respect to the current order. 

				// Store into the 'split' field any point
				// in this node. This is needed in a range 
				// search to decide whether the leaf nodes 
				// of the minimum and maximum should be 
				// reported or not.
				node->split_ = *pointSet.begin();
			}

			if (depth < orders() - 2)
			{
				// Recurse to the down child.
				node->down() = construct(nullptr, false, depth + 1, pointSet);
			}
			else if (parent)
			{
				// The down-recursion stops at level orders() - 2.

				// Compute the fractional cascading links
				// for the 'parent'.

				integer j = 0;
				
				// The last entry acts as a sentinel, and does 
				// not contain a point, so we will skip it here.
				for (integer i = 0;i < parent->entries();++i)
				{
					Entry& entry = parent->entrySet_[i];
					
					while (j < node->entries() &&
						multiLess(*node->entrySet_[j].point(), *entry.point(), orders() - 1))
					{
						++j;
					}

					entry.cascade(right) = j;
				}

				// Link the sentinel entry of the parent 
				// to the sentinel entry of the child.
				parent->entrySet_.back().cascade(right) = node->entries();
			}

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

		//! The sentinel node.
		std::unique_ptr<Node> end_;

		//! The root node.
		Node_Iterator root_;

		//! The set of points.
		PointSet pointSet_;

		//! Number of strict weak orders.
		integer orders_;
	};

}

namespace Pastel
{

	template <
		typename Point_,
		typename MultiLess_>
	class RangeTree_Settings
	{
	public:
		using Point = Point_;
		using MultiLess = MultiLess_;
	};

}

#include "pastel/geometry/rangetree_invariants.h"
#include "pastel/geometry/rangetree_search_range.h"

#endif
