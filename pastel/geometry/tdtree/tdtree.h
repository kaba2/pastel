// Description: Temporal kd-tree

#ifndef PASTELGEOMETRY_TDTREE_H
#define PASTELGEOMETRY_TDTREE_H

#include "pastel/geometry/tdtree/tdtree_concepts.h"
#include "pastel/geometry/tdtree/tdtree_fwd.h"
#include "pastel/geometry/tdtree/tdtree_cursor.h"
#include "pastel/geometry/tdtree/tdtree_entry.h"
#include "pastel/geometry/tdtree/tdtree_node.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/bounding/bounding_alignedbox.h"
#include "pastel/geometry/splitrule/slidingmidpoint2_splitrule.h"

#include "pastel/sys/sequence/fair_stable_partition.h"
#include "pastel/sys/range/interval_range.h"
#include "pastel/sys/range/transformed_range.h"
#include "pastel/sys/range/zip_range.h"
#include "pastel/sys/locator/transform_locator.h"
#include "pastel/sys/math/sign.h"

#include <range/v3/all.hpp>

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
		PASTEL_FWD(Locator);
		PASTEL_FWD(PointSet);
		PASTEL_FWD(Point_Iterator);
		PASTEL_FWD(Point_ConstIterator);
		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Entry);
		PASTEL_FWD(Cursor);
		PASTEL_FWD(Node);
		
		// Using an enum here triggers a bug in
		// Visual Studio 2015 RC.
		static constexpr int N = Locator::N;

		//! Constructs an empty tree.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		//template <
		//	int N_ = N,
		//	Requires<
		//		BoolConstant<(N_ >= 0)>
		//	> = 0
		//>
		TdTree()
		: end_(new Node)
		, root_(end_.get())
		, pointSet_()
		, locator_()
		, simple_(true)
		, bound_()
		, nodes_(0)
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

		Optional arguments:
		
		timeSet:
		Time-points for the point-set.
		Default: equal to index 0, 1, 2,...

		splitRule:
		The split-rule to use.
		*/
		template <
			PointSet_Concept PointSet_,
			typename... ArgumentSet
		>
		explicit TdTree(
			const PointSet_& pointSet,
			ArgumentSet&&... argumentSet)
		: end_(new Node)
		, root_(end_.get())
		, pointSet_()
		, locator_(Pastel::pointSetLocator(pointSet))
		, simple_(true)
		, bound_(Pastel::pointSetDimension(pointSet))
		, nodes_(0)
		{
			auto&& timeSet = PASTEL_ARG_S(timeSet, intervalRange((integer)0, (integer)Infinity()));
			auto&& splitRule = PASTEL_ARG_S(splitRule, SlidingMidpoint2_SplitRule());
				
			enum : bool
			{
				Simple = false
			};

			simple_ = Simple;

			std::vector<Iterator> iteratorSet;

			integer n = rangeSize(pointSet);
			if (n < (integer)Infinity())
			{
				iteratorSet.reserve(n);
				pointSet_.reserve(n);
			}

			for (auto&& element : zipSet(pointSet, timeSet))
			{
				auto&& point = element.first;
				auto&& time = element.second;

				pointSet_.emplace_back(
					pointPointId(point), time);
				iteratorSet.emplace_back(
					std::prev(pointSet_.end()));
			}

			if (!Simple)
			{
				// Sort the points in increasing order by time.
				ranges::action::stable_sort(
					iteratorSet,
					[](auto&& a, auto&& b) 
					{
						return a->time() < b->time();
					});

				// Check explicitly for the simplicity of 
				// the time-coordinates.
				simple_ = isSimple(iteratorSet);
			}

			// Compute a minimum bounding box for the points.
			auto bound = 
				boundingAlignedBox(pointSet);

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
			using std::swap;

			end_.swap(that.end_);
			swap(root_, that.root_);
			pointSet_.swap(that.pointSet_);
			swap(locator_, that.locator_);
			swap(simple_, that.simple_);
			bound_.swap(that.bound_);
			swap(nodes_, that.nodes_);
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
			simple_ = true;
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

		//! Returns all points.
		/*!
		This is a convenience function which returns
		pointSet(-(Real)Infinity(), (Real)Infinity()).
		*/
		decltype(auto) pointSetRange() const
		{
			return pointSetRange(
				-(Real)Infinity(),
				(Real)Infinity());
		}

		//! Returns all points in the time-interval [tMin, tMax[.
		/*!
		Preconditions:
		tMin <= tMax

		returns:
		A PointSet of ConstIterators, which contains the points 
		in the time-interval [tMin, tMax[. In particular, the points
		in this set are not user-defined Points; a ConstIterator 
		contains more information than a Point (e.g. time).
		*/
		decltype(auto) pointSetRange(
			const Real& tMin, 
			const Real& tMax) const
		{
			PENSURE(tMin <= tMax);

			return intervalRange(
				begin() + timeToIndex(tMin), 
				begin() + timeToIndex(tMax));
		}

		decltype(auto) pointSetLocator() const
		{
			// Since the user-defined locator
			// works only for user-defined points, 
			// we need to adapt it to work with
			// ConstIterators.
			return transformLocator<ConstIterator>(
				locator(),
				[](const ConstIterator& iTemporalPoint)
				{
					return iTemporalPoint->point();
				}
			);
		}

		decltype(auto) location(
			const ConstIterator& point) const
		{
			return Pastel::location(point, pointSetLocator());
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
			return Cursor(end_.get());
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

		//! Returns the number of nodes.
		integer nodes() const {
			return nodes_;
		}

		//! Returns the height of the tree.
		integer height() const {
			return height(root());
		}

		//! Returns the height of a node.
		integer height(const Cursor& cursor) const {
			if (cursor.leaf()) {
				return 1;
			}
			return std::max(
				height(cursor.child(false)), 
				height(cursor.child(true))) + 1;
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

		//! Returns the first point with point->time() >= time.
		/*!
		Time complexity:
		O(1), if simple() or 'time' is not in time-range,
		O(log(n)), otherwise.

		Exception safety: 
		nothrow
		*/
		integer timeToIndex(const Real& time) const
		{
			if (pointSet_.empty())
			{
				// There are no points.
				return 0;
			}

			if (time <= pointSet_.front().time())
			{
				return 0;
			}

			if (time > pointSet_.back().time())
			{
				return points();
			}

			// From now on there are at least two points.
			ASSERT_OP(pointSet_.size(), >=, 2);

			if (!simple())
			{
				// The time-coordinates are not simple.
				// We need to do a binary search  to convert 
				// time to a fractional cascading index.

				auto indicator = [&](integer i)
				{
					return pointSet_[i].time() >= time;
				};

				return binarySearch((integer)0, points(), indicator);
			}

			// From now on the time-coordinates are simple.

			integer tBegin = pointSet_[0].time();

			// Compute the distance between subsequent 
			// time-coordinates. By simplicity this is
			// a constant among all subsequent point-pairs.
			integer tDelta = pointSet_[1].time() - tBegin;

			// By simplicity, it holds that
			// pointSet_[i]->time() == tDelta i + tBegin.

			// Therefore
			// i = (pointSet_[i]->time() - tBegin) / tDelta.

			// For a general time-point, round up to the 
			// next integer.
			return std::ceil((time - tBegin) / tDelta);
		}

	private:
		//! Returns whether points are distributed simply in time.
		/*!
		returns:
		Whether pointSet[i]->time() = b i + c, for all i,
		for some integers b and c.
		*/
		bool isSimple(const std::vector<Iterator>& pointSet) const
		{
			if (pointSet.empty())
			{
				// There are no points; the points are
				// vacuously simple.
				return true;
			}

			auto isInteger = [](const Real& that)
			{
				return (integer)that == that;
			};

			if (!isInteger(pointSet.front()->time()))
			{
				// The starting time is not an integer;
				// the points are not simple.
				return false;
			}

			if (pointSet.size() == 1)
			{
				// There is only one point, with integer
				// time-instant; the points are simple.
				return true;
			}

			if (!isInteger(pointSet[1]->time() - pointSet[0]->time()))
			{
				// The distance between the time-instants of the 
				// second point and the first point is not an integer; 
				// the points are not simple.
				return false;
			}

			// Find out the distance between time-instants.
			integer delta = 
				pointSet[1]->time() - 
				pointSet[0]->time();

			// Check if the distance between time-instants is the 
			// same for all subsequent indices.
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
			++nodes_;

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

			if (pointSet.size() <= 1)
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
			std::tie(splitPosition, splitAxis) = 
				splitRule(
					locationSet(
						transformRange(pointSet, pointFromIterator),
						locator()
					), 
					bound
				);

			ENSURE_OP(splitAxis, >=, 0);
			ENSURE_OP(splitAxis, <, dimension());
			ENSURE(splitPosition >= bound.min()[splitAxis]);
			ENSURE(splitPosition <= bound.max()[splitAxis]);

			auto trindicator = [&](const Iterator& that)
				-> integer
			{
				return sign(locator()(that->point(), splitAxis) - splitPosition);
			};

			// Partition the elements with respect to the split-plane.
			// The partitioning must be stable for the children
			// to stay ordered with respect to time, and fair so
			// that the number of points is eventually decreased.
			auto leftEnd = fairStablePartition(pointSet, trindicator);

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
		Specifically, whether pointSet_[i]->time() = b i + c, 
		for some integers b and c. Whenever this is true, the initial 
		fractional cascading step can be performed in constant 
		time; otherwise it takes logarithmic time.
		*/
		bool simple_;

		//! Minimum bounding box for the points.
		AlignedBox<Real, N> bound_;

		//! Number of nodes.
		integer nodes_;
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

#include "pastel/geometry/tdtree/tdtree_invariants.h"

#endif
