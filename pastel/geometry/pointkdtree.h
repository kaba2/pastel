// Description: PointKdTree class
// Detail: A multi-level point kd-tree

#ifndef PASTEL_POINTKDTREE_H
#define PASTEL_POINTKDTREE_H

#include "pastel/sys/pointpolicy_concept.h"
#include "pastel/sys/real_concept.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/fastlist.h"
#include "pastel/sys/tristate.h"
#include "pastel/sys/poolallocator.h"
#include "pastel/sys/iterator_range.h"
#include "pastel/sys/vector_pointpolicy.h"

#include "pastel/geometry/alignedbox.h"

#include <boost/mpl/if.hpp>
#include <boost/iterator/indirect_iterator.hpp>

namespace Pastel
{

	//! A multi-level point kd-tree
	template <typename Real, int N = Dynamic, 
		typename PointPolicy = Vector_PointPolicy<Real, N> >
	class PointKdTree
	{
	public:
		enum
		{
			N_ = N
		};
		typedef Real Real_;
		typedef PointPolicy PointPolicy_;

		typedef typename PointPolicy::Point Point;
		class Cursor;

	private:
		typedef PoolAllocator NodeAllocator;
		typedef PoolAllocator PointAllocator;
		typedef PoolAllocator BoundAllocator;

		class PointInfo;
		class Node;

		typedef FastList<PointInfo, PointAllocator> PointSet;
		typedef typename PointSet::iterator Point_Iterator;

	public:
		typedef typename PointSet::const_iterator 
			Point_ConstIterator;
		typedef BidirectionalIterator_Range<Point_ConstIterator>
			Point_ConstRange;

		typedef boost::indirect_iterator<Point_ConstIterator, const Point> 
			PointData_ConstIterator;
		typedef BidirectionalIterator_Range<PointData_ConstIterator>
			PointData_ConstRange;

		//! Constructs an empty tree.
		/*!
		Exception safety:
		strong

		Time complexity:
		constant
		*/
		explicit PointKdTree(
			const PointPolicy& pointPolicy = PointPolicy(),
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

		//! Returns the point policy.
		/*!
		Exception safety:
		nothrow
		*/
		const PointPolicy& pointPolicy() const;

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
		integer dimension() const;

		//! Subdivides the tree.
		/*!
		This operation is always immediate. The tree will be
		up to date after this operation.
		*/
		template <typename SplitRule_PointKdTree>
		void refine(const SplitRule_PointKdTree& splitRule,
			integer bucketSize = 8);

		//! Insert a point into the tree.
		Point_ConstIterator insert(
			const Point& point, 
			bool hidden = false);

		//! Insert points into the tree.
		/*!
		Exception safety:
		strong

		begin, end:
		An iterator range consisting of points to insert.

		iteratorSet:
		An output iterator to which the corresponding point
		iterators are reported.
		*/
		template <typename Input_Point_ConstIterator,
			typename Point_ConstIterator_Iterator>
		void insert(
			const ForwardIterator_Range<Input_Point_ConstIterator>& pointSet, 
			Point_ConstIterator_Iterator iteratorSet,
			bool hidden = false);

		//! Insert points into the tree.
		/*!
		This is a convenience function that calls:
		
		insert(pointSet, NullIterator())
		
		See the documentation for that function.
		*/
		template <typename Input_Point_ConstIterator>
		void insert(
			const ForwardIterator_Range<Input_Point_ConstIterator>& pointSet);

		//! Removes a point from the tree.
		void erase(const Point_ConstIterator& iter);

		//! Removes a set of points from the tree.
		template <typename Point_ConstIterator_ConstIterator>
		void erase(const ForwardIterator_Range<Point_ConstIterator_ConstIterator>& pointSet);

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

	private:
		class SplitPredicate;

		class PointInfo
		{
		public:
			friend class PointKdTree;

			// Implicit conversion allowed.
			PointInfo(
				const Point& point,
				Node* leafNode = 0,
				bool hidden = false)
				: point_(point)
				, leafNode_(leafNode)
				, hidden_(hidden)
			{
			}

			const Point& operator*() const
			{
				return point_;
			}

			const Point& point() const
			{
				return point_;
			}

			Cursor leaf() const
			{
				return Cursor((Node*)leafNode_);
			}

			bool hidden() const
			{
				return hidden_;
			}
		
		private:
			void setLeaf(
				const Node* leafNode) const
			{
				leafNode_ = leafNode;
			}

			void setHidden(bool hidden) const
			{
				hidden_ = hidden;
			}

			Point point_;
			mutable const Node* leafNode_;
			mutable bool hidden_;
		};

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
		3) 0 <= splitAxis < dimension()

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
		begin, end:
		The iterator range [begin, end[ contains the
		new points to insert.

		returns:
		The first iterator of the inserted points in
		'pointSet_'.

		Also updates the bounding box.
		*/
		template <typename Input_Point_ConstIterator>
		Point_Iterator copyToEnd(
			const Input_Point_ConstIterator& begin,
			const Input_Point_ConstIterator& end,
			bool hidden);			

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
		template <typename SplitRule_PointKdTree>
		void refine(
			Node* node,
			Vector<Real, N>& minBound,
			Vector<Real, N>& maxBound,
			const SplitRule_PointKdTree& splitRule,
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

		/*
		pointSet_:
		Contains all the visible points in the tree. It is ordered 
		such that the points of each leaf node are positioned 
		sequentially in a range, and for a split node the 
		points in a left node are always listed before those 
		in a right node.

		hiddenSet_:
		Contains all the points in the tree which are not 
		visible in nodes. The points in this set may or
		may not have an associated node. 

		insertionSet_:
		Contains points waiting for insertion in the next 'update()'
		call. The points in this set do not have an associated node. 

		nodeAllocator_:
		Allocates memory for the nodes of the tree.
		Because all nodes are of the same size,
		we can provide for extremely fast
		node allocation via a specialized allocator.

		root_:
		The root node of the tree.

		leaves_:
		The number of leaf nodes in the tree.

		pointPolicy_:
		See 'pointpolicies.txt'.

		bound_:
		An axis aligned bounding box for the
		points in the tree. Not necessary of minimum
		size, since the bound is updated only on 
		insertion (i.e., no shrinking is done).

		simulateKdTree_:
		If true, no shrinking is done to nodes.
		*/

		PointSet pointSet_;
		PointSet hiddenSet_;
		PointSet insertionSet_;
		NodeAllocator nodeAllocator_;
		Node* root_;
		integer leaves_;
		PointPolicy pointPolicy_;
		AlignedBox<Real, N> bound_;
		bool simulateKdTree_;
	};

}

#include "pastel/geometry/pointkdtree_splitpredicate.h"
#include "pastel/geometry/pointkdtree_node.h"
#include "pastel/geometry/pointkdtree_cursor.h"

#include "pastel/geometry/pointkdtree.hpp"
#include "pastel/geometry/pointkdtree_private.hpp"

#endif
