// Description: Point kd-tree
// Documentation: pointkdtree.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_H
#define PASTELGEOMETRY_POINTKDTREE_H

#include "pastel/geometry/pointkdtree/pointkdtree_concepts.h"
#include "pastel/geometry/pointkdtree/pointkdtree_fwd.h"
#include "pastel/geometry/pointkdtree/pointkdtree_node.h"
#include "pastel/geometry/pointkdtree/pointkdtree_cursor.h"
#include "pastel/geometry/splitrules.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tristate.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/set/set_concept.h"

#include "pastel/geometry/shape/alignedbox.h"

#include <boost/mpl/if.hpp>

namespace Pastel
{

	template <typename Settings>
	using Empty_PointKdTree_Customization = 
		PointKdTree_Concepts::Customization<Settings>;

	//! Point kd-tree
	template <
		typename Settings,
		template <typename> class Customization = Empty_PointKdTree_Customization>
	class PointKdTree
	{
	public:
		PASTEL_CONCEPT_CHECK(Settings, PointKdTree_Settings_Concept);

		using Fwd = PointKdTree_Fwd<Settings>;

		PASTEL_FWD(Real);
		PASTEL_FWD(Locator);
		static constexpr integer N = Fwd::N;

		PASTEL_FWD(NodeAllocator);
		PASTEL_FWD(PointAllocator);
		PASTEL_FWD(BoundAllocator);

		PASTEL_FWD(PointSet);
		PASTEL_FWD(Point_ConstIterator);
		PASTEL_FWD(Point_ConstRange);
		PASTEL_FWD(Point_Iterator);

		PASTEL_FWD(PointData_ConstIterator);
		PASTEL_FWD(PointData_ConstRange);
		
		PASTEL_FWD(Point);
		PASTEL_FWD(PointInfo);
		PASTEL_FWD(Cursor);
		PASTEL_FWD(Node);
		PASTEL_FWD(SplitPredicate);

		//! Constructs an empty tree.
		/*!
		Exception safety:
		strong

		Time complexity:
		constant
		*/
		explicit PointKdTree(
			const Locator& locator = Locator(),
			bool simulateKdTree = false);

		//! Constructs a copy from another tree.
		/*!
		Exception safety:
		?

		Time complexity:
		O(that.points() * thatDepth + that.nodes())
		*/
		PointKdTree(const PointKdTree& that);

		//! Destructs the tree.
		/*!
		Exception safety:
		nothrow

		Time complexity:
		O(nodes()
		*/
		~PointKdTree();

		//! Assigns another tree.
		/*!
		Exception safety:
		strong
		*/
		PointKdTree& operator=(const PointKdTree& that);

		//! Swaps two trees.
		/*!
		Exception safety:
		nothrow
		*/
		void swap(PointKdTree& that);

		//! Returns the locator.
		/*!
		Exception safety:
		nothrow
		*/
		const Locator& locator() const;
		
		//! Extends the bounding box of the tree to cover the given box.
		/*!
		Exception safety:
		strong (FIX: make it nothrow)
		*/
		void reserveBound(const AlignedBox<Real, N>& boxToCover);

		//! Returns the bounding box of the tree.
		/*!
		Exception safety:
		nothrow
		*/
		const AlignedBox<Real, N>& bound() const;

		//! Returns true if there are no points in the tree.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		bool empty() const;

		//! Returns the root node of the tree.
		/*!
		Exception safety:
		nothrow
		*/
		Cursor root() const;

		//! Returns an iterator to the beginning of the point list.
		/*!
		Exception safety:
		nothrow
		*/
		Point_ConstIterator begin() const;

		//! Returns an iterator to the end of the point list.
		/*!
		Exception safety:
		nothrow
		*/
		Point_ConstIterator end() const;

		//! Returns an iterator range to the point list.
		/*!
		Exception safety:
		nothrow
		*/
		Point_ConstRange range() const;

		//! Returns an iterator to the beginning of the hidden point list.
		/*!
		Exception safety:
		nothrow
		*/
		Point_ConstIterator hiddenBegin() const;

		//! Returns an iterator to the end of the hidden point list.
		/*!
		Exception safety:
		nothrow
		*/
		Point_ConstIterator hiddenEnd() const;

		//! Returns an iterator range to the hidden point list.
		/*!
		Exception safety:
		nothrow
		*/
		Point_ConstRange hiddenRange() const;

		//! Adapts an iterator to referencing the points themselves.
		PointData_ConstIterator asPointData(
			const Point_ConstIterator& iter) const;

		//! Adapts a range to referencing the points themselves.
		PointData_ConstRange asPointData(
			const Point_ConstRange& range) const;

		//! Returns the underlying point-set.
		decltype(auto) pointSet() const
		{
			return locationSet(
				rangeSet(asPointData(range())), 
				locator_);
		}

		//! Returns the number of nodes in the tree.
		/*!
		Exception safety:
		nothrow

		This number includes both intermediate nodes
		and leaf nodes.
		*/
		integer nodes() const;

		//! Returns the number of leaf nodes in the tree.
		/*!
		Exception safety:
		nothrow
		*/
		integer leaves() const;

		//! Returns the number of points in the tree.
		/*!
		Exception safety:
		nothrow
		*/
		integer points() const;

		//! Returns the dimension of the tree.
		/*!
		Exception safety:
		nothrow
		*/
		integer n() const;

		//! Subdivides the tree.
		/*!
		This operation is always immediate. The tree will be
		up to date after this operation.
		*/
		template <typename SplitRule = SlidingMidpoint2_SplitRule>
		void refine(
			const SplitRule& splitRule = SplitRule(),
			integer bucketSize = 8);

		//! Insert a point into the tree.
		Point_ConstIterator insert(
			const Point& point, 
			bool hidden = false);

		//! Insert points into the tree.
		/*!
		Exception safety:
		strong

		pointSet:
		A set of elements convertible to a Point.

		Optional arguments
		------------------

		report (Output_Concept(ConstIterator) : nullOutput()):
		An output to report the Point_ConstIterators
		of the inserted points.

		hidden (bool : false):
		Whether the points should be inserted as 
		hidden points or not.
		*/
		template <
			typename PointId_Set,
			typename... ArgumentSet,
			Requires<
				Models<PointId_Set, Set_Concept>
			> = 0
		>
		void insertSet(
			const PointId_Set& pointSet, 
			ArgumentSet&&... argumentSet);

		//! Removes a point from the tree.
		void erase(const Point_ConstIterator& iter);

		//! Removes a set of points from the tree.
		template <typename Point_ConstIterator_ConstRange>
		void erase(const Point_ConstIterator_ConstRange& pointSet);

		//! Hides all points in the tree.
		void hide();

		//! Hides a point in the tree.
		void hide(const Point_ConstIterator& iter);
	
		//! Shows all hided points in the tree.
		void show();

		//! Shows a hided point in the tree.
		void show(const Point_ConstIterator& iter);

		//! Clears off subdivision and points.
		/*!
		Exception safety:
		nothrow
		*/
		void clear();

		//! Clears the points but leaves the subdivision intact.
		/*!
		Exception safety:
		nothrow
		*/
		void erase(bool eraseHidden = true);

		//! Clears the points in a subtree but leaves subdivision intact.
		void erase(const Cursor& cursor, bool eraseHidden = true);

		//! Collapses the tree into a single leaf node.
		void merge();

		//! Collapse a subtree into a leaf node.
		void merge(const Cursor& cursor);

		//! Converts time to a cascading index.
		/*!
		This function is solely for compatibility so that
		the interface is similar enough to the temporal kd-tree
		for the same nearest neighbor search to work for both.
		*/
		integer timeToIndex(const Real& time) const
		{
			return 0;
		}

	private:
		//! Allocates the root node etc.
		void initialize();

		//! Copy constructs a subtree.
		void copyConstruct(
			Node* thisNode,
			Node* thatNode);

		//! Allocate a leaf node.
		Node* allocateLeaf(
			Node* parent,
			const Point_ConstIterator& first,
			const Point_ConstIterator& last,
			integer points);

		//! Runs destructors for all nodes of a subtree.
		void destructSubtree(Node* node);

		//! Compute a bounding box for points.
		AlignedBox<Real, N> computeBound(
			const Point_ConstIterator& begin, 
			const Point_ConstIterator& end) const;

		//! Compute a bounding box for points.
		std::pair<Real, Real> computeBound(
			const Point_ConstIterator& begin, 
			const Point_ConstIterator& end,
			integer axis) const;

		//! Collapse a subtree into a leaf node.
		void merge(Node* node);

		//! Deallocate the nodes of a subtree.
		void clear(Node* node);

		//! Sets the leaf nodes of a range of points.
		void setLeaf(
			const Point_ConstIterator& begin,
			const Point_ConstIterator& end,
			Node* node);

		//! Remove points under a subtree.
		void erase(Node* node);

		//! Clear point ranges in a subtree, and set bucket nodes.
		/*!
		Note points are not actually removed. Use erase()
		for this. This is actually foremost a helper function for
		erase().
		*/
		void clearPoints(Node* node);

		//! Updates hierarchical bound information.
		void updateBounds(Node* node, const AlignedBox<Real, N>& bound);

		//! Updates the hierarchical information in a node.
		void updateHierarchical(Node* node);

		//! Updates the nodes in a subtree.
		void updateDownwards(Node* node);
		
		//! Updates the path to the root.
		/*!
		With lazy updates, this means invalidating the nodes.
		With immediate updates, this means updating hierarchical
		information.
		*/
		void updateUpwards(Node* node);

		//! Subdivides a leaf node with the given plane.
		/*!
		Preconditions:
		1) node->leaf() == true
		2) 'node' points to a node in this tree.
		3) 0 <= splitAxis < n()

		Exception safety:
		strong

		node:
		A leaf node of the tree to subdivide.

		splitPosition:
		Signed distance of the splitting plane from the origin.

		splitAxis:
		Index of the standard basis axis to use for
		the splitting plane normal.
		*/
		void subdivide(
			Node* node,
			const Real& splitPosition,
			integer splitAxis,
			const Real& prevMin,
			const Real& prevMax);

		//! Inserts new points at the end of the pointSet_.
		/*!
		Also updates the bounding box.

		pointSet:
		A set of Points to insert.

		returns:
		The first Point_Iterator of the inserted points in
		'pointSet_'.
		*/
		template <typename PointId_Set>
		auto copyToEnd(
			const PointId_Set& pointSet, 
			bool hidden)
			-> Point_Iterator;

		//! Propagates new points to leaf nodes.
		/*!
		Preconditions:
		1) [first, last] is an inclusive iterator range in 'pointSet_'.
		2) count > 0

		node:
		The node to which recursively insert the points.

		first, last:
		The iterator range [first, last] contains the
		new points to insert.

		count:
		The number of points in [first, last].

		bound:
		This will be filled with the minimum bounding
		box for the new points.
		
		If the node is an intermediate node,
		this function reorders 'pointSet_'
		in the given range so that the points
		going to the left node are listed
		before those going to the right node.
		This reordering is done by splicing
		so no copying of points is involved. 
		The function then recurses to both nodes.

		If the node is a leaf node,
		the given range is spliced to the correct
		position such that all points of a leaf node
		are listed sequentially.
		*/
		void insert(
			Node* node,
			const Point_Iterator& first, 
			const Point_Iterator& last,
			integer count,
			AlignedBox<Real, N>& bound);

		//! Subdivides the tree using the given subdivision rule.
		/*!
		Preconditions:
		allLessEqual(minBound, maxBound)
		*/
		template <typename SplitRule>
		void refine(
			Node* node,
			AlignedBox<Real, N>& bound,
			const SplitRule& splitRule,
			integer depth,
			integer bucketSize);

		//! Actually inserts points into the tree.
		void commitInsertion();

		//! Actually removes a point from the tree.
		void commitErase(const Point_ConstIterator& iter);

		//! Actually hides a point in the tree.
		void commitHide(const Point_ConstIterator& iter);

		//! Actually shows a point in the tree.
		void commitShow(const Point_ConstIterator& iter);

		//! Sets the given points hidden.
		void setHidden(
			const Point_ConstIterator& begin,
			const Point_ConstIterator& end,
			bool hidden);

		//! The set of visible points. 
		/*!
		This set is ordered such that the points of each leaf 
		node are positioned sequentially in a range, and for a 
		split node the points in a left node are always listed 
		before those in a right node.
		*/
		PointSet pointSet_;

		//! The set of hidden points.
		/*!
		This set contains all the points in the tree which 
		are not visible in nodes. The points in this set may 
		or may not have an associated node. 
		*/
		PointSet hiddenSet_;

		//! The set of points waiting for insertion.
		/*! 
		These points will be inserted during the next 'update()' call.
		The points in this set do not have an associated node. 
		*/
		PointSet insertionSet_;

		//! Allocates memory for the nodes of the tree.
		/*!
		Because all nodes are of the same size, we can provide 
		for extremely fast node allocation via a specialized 
		allocator.
		*/
		NodeAllocator nodeAllocator_;

		//! The root node of the tree.
		Node* root_;

		//! The number of leaf nodes in the tree.
		integer leaves_;

		//! The point abstraction.
		Locator locator_;

		//! A bounding box for the points in the tree.
		/*!
		This box is not necessary of minimum extent, since 
		the bound is updated only on insertion (i.e., no 
		shrinking is done on removal).
		*/
		AlignedBox<Real, N> bound_;

		//! Whether to act like a kd-tree.
		/*
		When false, the behaviour is to compute the 
		empty space around the splitting plane, so that
		it can be skipped. A kd-tree does not do this.
		*/
		bool simulateKdTree_;
	};

}

namespace Pastel 
{

	template <typename Locator_>
	struct PointKdTree_Settings
	{
		using Locator = Locator_;
	};

}

#include "pastel/geometry/pointkdtree/pointkdtree.hpp"
#include "pastel/geometry/pointkdtree/pointkdtree_depth.h"
#include "pastel/geometry/pointkdtree/pointkdtree_equivalent.h"
#include "pastel/geometry/pointkdtree/pointkdtree_invariants.h"
#include "pastel/geometry/pointkdtree/pointkdtree_private.hpp"
#include "pastel/geometry/pointkdtree/pointkdtree_search_range.h"
#include "pastel/geometry/pointkdtree/pointkdtree_splitpredicate.h"

#include "pastel/geometry/splitrules.h"

#endif
