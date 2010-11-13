// Description: PointKdTree class
// Detail: A multi-level point kd-tree

#ifndef PASTEL_POINTKDTREE_H
#define PASTEL_POINTKDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/fastlist.h"
#include "pastel/sys/tristate.h"
#include "pastel/sys/poolallocator.h"
#include "pastel/sys/forwardrange.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/vector_pointpolicy.h"

#include <boost/mpl/if.hpp>
#include <boost/iterator/indirect_iterator.hpp>

namespace Pastel
{

	//! A multi-level point kd-tree
	/*!
	See also:
	pointpolicy_concept.h
	*/

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

		typedef FastList<PointInfo, PointAllocator> PointContainer;
		typedef typename PointContainer::iterator Point_Iterator;

	public:
		typedef typename PointContainer::const_iterator 
			Point_ConstIterator;
		typedef boost::indirect_iterator<Point_ConstIterator, const Point> 
			PointData_ConstIterator;

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

		PointData_ConstIterator pointBegin() const;

		//! Returns an iterator to the end of the point list.
		/*!
		Exception safety:
		nothrow
		*/
		Point_ConstIterator end() const;

		PointData_ConstIterator pointEnd() const;

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

		template <typename SplitRule_PointKdTree>
		void refine(const SplitRule_PointKdTree& splitRule,
			integer bucketSize = 8);

		//! Insert a point into the tree.
		Point_ConstIterator insert(const Point& point);

		//! Insert points into the tree.
		/*!
		Exception safety:
		strong

		begin, end:
		An iterator range consisting of points to insert.
		*/
		template <typename InputIterator>
		void insert(
			const ForwardRange<InputIterator>& pointSet);

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
		template <typename InputIterator,
			typename Point_ConstIterator_OutputIterator>
		void insert(
			const ForwardRange<InputIterator>& pointSet, 
			Point_ConstIterator_OutputIterator iteratorSet);

		//! Removes a point from the tree.
		void erase(const Point_ConstIterator& iter);

		//! Removes a set of points from the tree.
		template <typename Point_ConstIterator_ConstIterator>
		void erase(const ForwardRange<Point_ConstIterator_ConstIterator>& pointSet);

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
		void erasePoints();

		//! Clears the points in a subtree but leaves subdivision intact.
		void erasePoints(const Cursor& cursor);

		//! Collapses the tree into a single leaf node.
		void merge();

		//! Collapse a subtree into a leaf node.
		void merge(const Cursor& cursor);

		//! Returns the position of a given point.
		Vector<Real, N> point(const Point& point) const;

	private:
		class SplitPredicate;

		class PointInfo
		{
		public:
			friend class PointKdTree;

			// Implicit conversion allowed.
			PointInfo(
				const Point& point,
				Node* leafNode = 0)
				: point_(point)
				, leafNode_(leafNode)
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
		
		private:
			void setLeaf(
				const Node* leafNode) const
			{
				leafNode_ = leafNode;
			}

			Point point_;
			mutable const Node* leafNode_;
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
		void erase(Node* node);

		//! Sets the leaf nodes of a range of points.
		void setLeaf(
			const Point_ConstIterator& begin,
			const Point_ConstIterator& end,
			Node* node);

		//! Remove points under a subtree.
		void erasePoints(Node* node);

		//! Clear point ranges in a subtree, and set bucket nodes.
		/*!
		Note points are not actually removed. Use erasePoints()
		for this. This is actually foremost a helper function for
		erasePoints().
		*/
		void clearPoints(Node* node);

		//! Updates hierarchical bound information.
		void updateBounds(Node* node, const AlignedBox<Real, N>& bound);

		//! Propagates hierarchical information upwards.
		void updateHierarchicalUpwards(Node* node);

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

		//! Inserts new points at the end of the pointList_.
		/*!
		returns:
		The first iterator of the inserted points.

		Also updates the bounding box.
		*/
		template <typename InputIterator>
		Point_Iterator insertPrepare(
			const InputIterator& begin,
			const InputIterator& end);			

		//! Propagates new points to leaf nodes.
		/*!
		Preconditions:
		1) [first, last] is an inclusive iterator range in 'pointList_'.
		2) count > 0
		
		If the node is an intermediate node,
		this function reorders 'pointList_'
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

		/*
		pointList_:
		Contains all points in the tree ordered
		in such a way that the points of each leaf node 
		are positioned sequantially in a range.

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
		See the PointPolicy concept.

		bound_:
		An axis aligned bounding box for the
		points in the tree.

		simulateKdTree_:
		If true, no shrinking is done to nodes.
		*/

		PointContainer pointList_;
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
