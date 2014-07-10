// Description: Temporal kd-tree

#ifndef PASTELGEOMETRY_TDTREE_H
#define PASTELGEOMETRY_TDTREE_H

#include "pastel/geometry/tdtree_concepts.h"
#include "pastel/geometry/tdtree_fwd.h"
#include "pastel/geometry/tdtree_entry.h"
#include "pastel/geometry/tdtree_node.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/longestmedian_splitrule.h"

#include "pastel/sys/less_concept.h"
#include "pastel/sys/dereferenced_predicate.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/complement_indicator.h"
#include "pastel/sys/binary_search.h"

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
	using Empty_TdTree_Customization = 
		TdTree_Concepts::Customization<Settings>;

	//! Temporal kd-tree
	/*!
	Space complexity: O(n log(n))
	*/
	template <
		typename Settings,
		template <typename> class Customization = Empty_TdTree_Customization>
	class TdTree
	{
	public:
		using Fwd = TdTree_Fwd<Settings>;
		PASTEL_FWD(Real);
		PASTEL_FWD(Point);
		PASTEL_FWD(Point_Entry);
		PASTEL_FWD(Locator);
		PASTEL_FWD(PointSet);
		PASTEL_FWD(Point_Iterator);
		PASTEL_FWD(Point_ConstIterator);
		PASTEL_FWD(Entry);
		PASTEL_FWD(Node);
		PASTEL_FWD(Node_Iterator);
		PASTEL_FWD(Node_ConstIterator);
		static PASTEL_CONSTEXPR integer N = Locator::N;

		//! Constructs an empty tree.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		TdTree()
		: end_(new Node)
		, root_(end_.get())
		, pointSet_()
		, locator_()
		, simple_(false)
		{
		}

		//! Move-constructs from another tree.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		TdTree(TdTree&& that)
		: TdTree()
		{
			swap(that);
		}

		//! Copy-constructs from another tree.
		/*!
		Time complexity: 
		O(n log(n))
		where 
		n = that.size().

		Exception safety: strong
		*/
		TdTree(const TdTree& that)
		: TdTree()
		{
			TdTree copy(that.pointSet_, that.dimension());
			swap(copy);
		}

		//! Constructs from a given point-set.
		/*!
		Time complexity: 
		O(n log(n))
		where
		n is the size of 'pointSet'.
		*/
		template <
			typename Point_Input,
			typename Real_Input = Counting_Input<Real>,
			typename SplitRule = LongestMedian_SplitRule>
		explicit TdTree(
			Point_Input pointSet,
			Real_Input timeSet = Real_Input(),
			const Locator& locator = Locator(),
			const SplitRule& splitRule = SplitRule())
		: TdTree()
		{
			static PASTEL_CONSTEXPR bool Simple = 
				std::is_same<Real_Input, Counting_Input<Real>>::value;

			locator_ = locator;
			simple_ = Simple;

			std::vector<Point_Iterator> iteratorSet;

			integer nHint = pointSet.nHint();
			iteratorSet.reserve(nHint);
			pointSet_.reserve(nHint);
	
			while (!pointSet.empty())
			{
				ENSURE(!timeSet.empty());
				pointSet_.emplace_back(pointSet(), timeSet());
				iteratorSet.emplace_back(
					std::prev(pointSet_.end()));
			}

			if (!Simple)
			{
				// Sort the points in increasing order by time.
				
				auto timeLess = [&](
					const Point_Iterator& left, 
					const Point_Iterator& right)
				{
					return left->time() < right->time();
				};

				boost::stable_sort(iteratorSet, timeLess);

				// Check explicitly for the simplicity of 
				// the time-coordinates.
				simple_ = isSimple(iteratorSet);
			}

			bound_ = boundingAlignedBox(
				transformInput(rangeInput(iteratorSet),
				[&](const Point_Iterator& point)
				{
					return point->point();
				}),
				locator);

			root_ = construct(nullptr, false, iteratorSet, bound_, splitRule);
		}

		//! Destructs the tree.
		/*!
		Time complexity:
		O(n log(n)^(d - 1))
		where
		n = size(),
		d = dimension().

		Exception safety: nothrow
		*/
		~TdTree()
		{
			clear();
		}

		//! Swaps two trees.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(TdTree& that)
		{
			end_.swap(that.end_);
			std::swap(root_, that.root_);
			pointSet_.swap(that.pointSet_);
			locator_.swap(that.locator_);
			std::swap(simple_, that.simple_);
		}

		//! Removes all points from the tree.
		/*!
		Time complexity:
		O(n log(n)^(d - 1))
		where
		n = size(),
		d = dimension().

		Exception safety: nothrow
		*/
		void clear()
		{
			clear(root_);
			root_ = end_.get();
			pointSet_.clear();
			simple_ = false;
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

		//! Returns the spatial dimension.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer dimension() const
		{
			return locator_.n();
		}

		//! Returns the locator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Locator& locator() const
		{
			return locator_;
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

		//! Returns a minimum bounding box for the points.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const AlignedBox<Real, N>& bound() const
		{
			return bound_;
		}

	private:
		bool isSimple(const std::vector<Point_Iterator>& pointSet) const
		{
			integer t = pointSet.front()->time();
			for (auto&& point : pointSet)
			{
				if (point->time() != t)
				{
					return false;
				}
				++t;
			}

			return true;
		}

		template <typename SplitRule>
		Node_Iterator construct(
			Node_Iterator parent,
			bool right,
			std::vector<Point_Iterator>& pointSet,
			AlignedBox<Real, N>& bound,
			const SplitRule& splitRule)
		{
			ASSERT(!pointSet.empty());

			// Invariant:
			// The points in 'pointSet' are in
			// increasing order in the temporal
			// coordinate.

			Node* node = new Node(pointSet);
			node->isolate(end_.get());

			if (pointSet.size() <= 1)
			{
				// This is a leaf node.
				return node;
			}

			// Choose the splitting plane according
			// to the splitting rule.

			auto pointFromIterator = 
				[&](const Point_Iterator& that)
			{
				return that->point();
			};

			integer splitAxis = 0;
			Real splitPosition = 0;
			std::tie(splitPosition, splitAxis) =
				splitRule(
					transformInput(rangeInput(pointSet), pointFromIterator), 
					locator(), bound);

			ENSURE_OP(splitAxis, >=, 0);
			ENSURE_OP(splitAxis, <, dimension());
			ENSURE(splitPosition >= bound.min()[splitAxis]);
			ENSURE(splitPosition <= bound.max()[splitAxis]);

			{
				auto lessSplit = [&](const Point_Iterator& that)
				{
					return locator()(that->point(), splitAxis) < splitPosition;
				};

				auto greaterSplit = [&](integer i)
				{
					return locator()(pointSet[i]->point(), splitAxis) > splitPosition;
				};

				// Partition the elements with respect to the split-plane.
				// The partitioning must be stable for the children
				// to stay ordered with respect to time.
				auto partitionLeftEnd = 
					boost::stable_partition(pointSet, lessSplit);

				// In the case there are points which lie exactly on the
				// plane, we wish to distribute them evenly on both sides
				// of the splitting plane. For example, it is possible to
				// have all points equal, in which case this rule is
				// essential to be able to obtain a logarithmic depth for
				// the tree.
				integer equalBegin = 
					partitionLeftEnd - pointSet.begin();
				integer equalEnd = 
					binarySearch(equalBegin, (integer)pointSet.size(),
					greaterSplit);

				// Pick the median of the equivalent elements.
				integer leftEnd = (equalBegin + equalEnd) / 2;

				// Recurse to the left child.
				{
					Real oldMax = bound.max()[splitAxis];
					bound.max()[splitAxis] = splitPosition;

					std::vector<Point_Iterator> leftSet(
						pointSet.begin(), pointSet.begin() + leftEnd);
					node->child(false) = construct(node, false, leftSet, bound, splitRule);

					bound.max()[splitAxis] = oldMax;
				}

				// Recurse to the right child.
				{
					Real oldMin = bound.min()[splitAxis];
					bound.min()[splitAxis] = splitPosition;
					
					std::vector<Point_Iterator> rightSet(
						pointSet.begin() + leftEnd, pointSet.end());
					node->child(true) = construct(node, true, rightSet, bound, splitRule);

					bound.min()[splitAxis] = oldMin;
				}
			}

			// Compute the fractional cascading links
			// for the 'parent'.

			integer j = 0;
			
			// The last entry acts as a sentinel, and does 
			// not contain a point, so we will skip it here.
			for (integer i = 0;i < parent->entries();++i)
			{
				Entry& entry = parent->entrySet_[i];
				
				while (j < node->entries() &&
					node->entrySet_[j].point()->time() < entry.point()->time())
				{
					++j;
				}

				entry.cascade(right) = j;
			}

			// Link the sentinel entry of the parent 
			// to the sentinel entry of the child.
			parent->entrySet_.back().cascade(right) = node->entries();

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

		//! The set of space-time points.
		PointSet pointSet_;

		//! The locator.
		Locator locator_;

		//! Whether the temporal coordinates are subsequent integers.
		/*!
		Specifically, whether pointSet_[i]->time() = i + c, 
		for some integer c. Whenever this is true, the initial 
		fractional cascading step can be performed in constant 
		time.
		*/
		bool simple_;

		//! Minimum bounding box for the points.
		AlignedBox<Real, N> bound_;
	};

}

namespace Pastel
{

	template <typename Locator_>
	class TdTree_Settings
	{
	public:
		using Locator = Locator_;
	};

}

#include "pastel/geometry/tdtree_invariants.h"

#endif
