// Description: Temporal kd-tree

#ifndef PASTELGEOMETRY_TDTREE_H
#define PASTELGEOMETRY_TDTREE_H

#include "pastel/geometry/tdtree_concepts.h"
#include "pastel/geometry/tdtree_fwd.h"
#include "pastel/geometry/tdtree_cursor.h"
#include "pastel/geometry/tdtree_entry.h"
#include "pastel/geometry/tdtree_node.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/longestmedian_splitrule.h"

#include "pastel/sys/fair_stable_partition.h"

#include <boost/range/algorithm/stable_sort.hpp>

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
		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Entry);
		PASTEL_FWD(Cursor);
		PASTEL_FWD(Node);
		PASTEL_FWD(Cursor);
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
			typename Real_Input = Infinite_Counting_Input<Real>,
			typename SplitRule = LongestMedian_SplitRule>
		explicit TdTree(
			Point_Input pointSet,
			Real_Input timeSet = Real_Input(),
			const Locator& locator = Locator(),
			const SplitRule& splitRule = SplitRule())
		: TdTree()
		{
			static PASTEL_CONSTEXPR bool Simple = 
				std::is_same<Real_Input, Infinite_Counting_Input<Real>>::value;

			locator_ = locator;
			simple_ = Simple;

			std::vector<Iterator> iteratorSet;

			integer nHint = pointSet.nHint();
			iteratorSet.reserve(nHint);
			pointSet_.reserve(nHint);
	
			while (!pointSet.empty())
			{
				ENSURE(!timeSet.empty());
				pointSet_.emplace_back(pointSet.get(), timeSet.get());
				iteratorSet.emplace_back(
					std::prev(pointSet_.end()));

				pointSet.pop();
				timeSet.pop();
			}

			if (!Simple)
			{
				// Sort the points in increasing order by time.
				
				auto timeLess = [&](
					const Iterator& left, 
					const Iterator& right)
				{
					return left->time() < right->time();
				};

				boost::stable_sort(iteratorSet, timeLess);

				// Check explicitly for the simplicity of 
				// the time-coordinates.
				simple_ = isSimple(iteratorSet);
			}

			// Compute a minimum bounding box for the points.
			auto bound = boundingAlignedBox(
				transformInput(rangeInput(iteratorSet),
				[&](const Iterator& point)
				{
					return point->point();
				}),
				locator);

			bound_ = bound;
			root_ = construct(nullptr, false, iteratorSet, bound, splitRule);
		}

		//! Destructs the tree.
		/*!
		Time complexity:
		O(n log(n))
		where
		n = size().

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
		O(n log(n))
		where
		n = size().

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
		integer points() const
		{
			return pointSet_.size();
		}

		//! Returns the number of points in the tree.
		/*!
		This is a convenience function which returns
		points().
		*/
		integer size() const
		{
			return points();
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

		//! Returns the spatial dimension.
		/*!
		This is a convenience function which returns
		dimension().
		*/
		integer n() const
		{
			return dimension();
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
		Cursor root() const
		{
			return Cursor(root_);
		}

		PASTEL_ITERATOR_FUNCTIONS(begin, pointSet_.begin());
		PASTEL_ITERATOR_FUNCTIONS(end, pointSet_.end());

		//! Returns the end node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Cursor endNode() const
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

		//! Returns whether the time-coordinates are simple.
		/*!
		The time-coordinates of the stored points are simple, if
		pointSet[i]->time() == a * i + b,
		for some integers a and b, and all i.
		*/
		bool simple() const
		{
			return simple_;
		}

	private:
		bool isSimple(const std::vector<Iterator>& pointSet) const
		{
			if (pointSet.empty())
			{
				return true;
			}

			auto isInteger = [](const Real& that)
			{
				return (integer)that == that;
			};

			if (!isInteger(pointSet.front()->time()))
			{
				return false;
			}

			if (pointSet.size() == 1)
			{
				return true;
			}

			if (!isInteger(pointSet[1]->time() - pointSet[0]->time()))
			{
				return false;
			}

			integer delta = 
				pointSet[1]->time() - 
				pointSet[0]->time();

			integer t = pointSet.front()->time();
			for (integer i = 0;i < pointSet.size();++i)
			{
				if (pointSet[i]->time() != t)
				{
					return false;
				}

				t += delta;
			}

			return true;
		}

		//! Returns the first point with point->time() >= time.
		/*!
		Time complexity:
		O(1), if simple(),
		O(log(n)), otherwise.

		Exception safety: 
		nothrow
		*/
		integer timeToIndex(const Real& time) const
		{
			if (!simple())
			{
				// The time-coordinates are not simple.
				// We need to do a binary search  to convert 
				// time to a fractional cascading index.

				auto indicator = [&](integer i)
				{
					return pointSet_[i]->time() >= time;
				};

				return binarySearch(0, points(), indicator);
			}

			// From now on the time-coordinates are simple.

			if (pointSet_.empty())
			{
				// There are no points.
				return 0;
			}

			if (pointSet_.size() == 1)
			{
				// There is only one point.
				return (time > pointSet_.front()->time());
			}
			
			// There are at least two points.

			integer tBegin = pointSet[0]->time();

			// Compute the distance between subsequent 
			// time-coordinates. By simplicity this is
			// a constant among all subsequent point-pairs.
			integer tDelta = pointSet_[1]->time() - tBegin;

			// Round up any fractional time; by simplicity
			// all time-coordinates are integral.
			integer tTime = std::ceil(time);

			// By simplicity, it holds that
			// pointSet_[i]->time() == tDelta i + tBegin.

			// Therefore
			// i = (pointSet_[i]->time() - tBegin) / tDelta.

			// For a general time-point, round up to the 
			// next integer.
			return divideAndRoundUp(tTime - tBegin, tDelta);
		}

		template <typename SplitRule>
		Node* construct(
			Node* parent,
			bool right,
			std::vector<Iterator>& pointSet,
			AlignedBox<Real, N>& bound,
			const SplitRule& splitRule)
		{
			// Invariant:
			// The points in 'pointSet' are in
			// increasing order in the temporal
			// coordinate.

			Node* node = new Node(pointSet);
			node->isolate(end_.get());

			if (parent)
			{
				node->min_ = bound.min()[parent->splitAxis()];
				node->max_ = bound.max()[parent->splitAxis()];

				// Compute the fractional cascading links
				// for the 'parent'.

				integer j = 0;

				// The last entry acts as a sentinel, and does 
				// not contain a point, so we will skip it here.
				for (integer i = 0; i < parent->points(); ++i)
				{
					Entry& entry = parent->entrySet_[i];

					while (j < node->points() &&
						node->entrySet_[j].point()->time() < entry.point()->time())
					{
						++j;
					}

					entry.cascade(right) = j;
				}

				// Link the sentinel entry of the parent 
				// to the sentinel entry of the child.
				parent->entrySet_.back().cascade(right) = node->points();
			}

			if (pointSet.size() <= 3)
			{
				// This is a leaf node.
				return node;
			}

			// Choose the splitting plane according
			// to the splitting rule.

			auto pointFromIterator =
				[&](const Iterator& that)
			{
				return that->point();
			};

			integer splitAxis = 0;
			Real splitPosition = 0;
			std::tie(splitPosition, splitAxis) = splitRule(
				transformInput(rangeInput(pointSet), pointFromIterator),
				locator(), bound);

			ENSURE_OP(splitAxis, >=, 0);
			ENSURE_OP(splitAxis, <, dimension());
			ENSURE(splitPosition >= bound.min()[splitAxis]);
			ENSURE(splitPosition <= bound.max()[splitAxis]);

			auto trindicator = [&](const Iterator& that)
				-> integer
			{
				if (locator()(that->point(), splitAxis) == splitPosition)
				{
					return 0;
				}

				if (locator()(that->point(), splitAxis) < splitPosition)
				{
					return -1;
				}

				return 1;
			};

			// Partition the elements with respect to the split-plane.
			// The partitioning must be stable for the children
			// to stay ordered with respect to time.
			auto leftEnd = fairStablePartition(
				range(pointSet.begin(), pointSet.end()), trindicator);

			// Set the split-point for the node.
			node->split_ = *leftEnd;
			node->splitAxis_ = splitAxis;

			// Recurse to the left child.
			{
				Real oldMax = bound.max()[splitAxis];
				bound.max()[splitAxis] = splitPosition;

				std::vector<Iterator> leftSet(
					pointSet.begin(), leftEnd);

				Node* left = construct(node, false, leftSet, bound, splitRule);
				node->child(false) = left;
				bound.max()[splitAxis] = oldMax;

				left->prevMin_ = bound.min()[splitAxis];
				left->prevMax_ = bound.max()[splitAxis];
			}

			// Recurse to the right child.
			{
				Real oldMin = bound.min()[splitAxis];
				bound.min()[splitAxis] = splitPosition;
				
				std::vector<Iterator> rightSet(
					leftEnd, pointSet.end());
				
				Node* right = construct(node, true, rightSet, bound, splitRule);
				node->child(true) = right;
				bound.min()[splitAxis] = oldMin;

				right->prevMin_ = bound.min()[splitAxis];
				right->prevMax_ = bound.max()[splitAxis];
			}

			// Return the node.
			return node;
		}
		
		void clear(Node* node)
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
		Node* root_;

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
