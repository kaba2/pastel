// Description: PointKdTree class
// Detail: A multi-level point kd-tree

#ifndef PASTEL_POINTKDTREE_H
#define PASTEL_POINTKDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/fastlist.h"
#include "pastel/sys/tristate.h"
#include "pastel/sys/poolallocator.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/vector_pointpolicy.h"

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
		typedef typename PointPolicy::Object Object;
		class Cursor;

	private:
		typedef PoolAllocator NodeAllocator;
		typedef PoolAllocator ObjectAllocator;
		typedef PoolAllocator BoundAllocator;

		class ObjectInfo;
		class Node;

		typedef FastList<ObjectInfo, ObjectAllocator> ObjectContainer;
		typedef typename ObjectContainer::iterator ObjectIterator;

	public:
		enum
		{
			N_ = N
		};
		typedef Real Real_;
		typedef PointPolicy PointPolicy_;

		typedef typename ObjectContainer::const_iterator 
			ConstObjectIterator;
		typedef boost::indirect_iterator<ConstObjectIterator, const Object> 
			ConstObjectDataIterator;

		//! Constructs an empty tree.
		/*!
		Exception safety:
		strong

		Time complexity:
		constant
		*/
		explicit PointKdTree(
			bool simulateKdTree = false,
			const PointPolicy& pointPolicy = PointPolicy());

		//! Constructs an empty tree.
		/*!
		Preconditions:
		dimension > 0
		dimension == N || N == Dynamic
		
		Exception safety:
		strong

		Time complexity:
		constant
		*/
		explicit PointKdTree(
			Dimension dimension,
			bool simulateKdTree = false,
			const PointPolicy& pointPolicy = PointPolicy());

		//! Constructs a copy from another tree.
		/*!
		Exception safety:
		?

		Time complexity:
		O(that.objects() * thatDepth + that.nodes())
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

		//! Returns the object policy.
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

		//! Returns true if there are no objects in the tree.
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

		//! Returns an iterator to the beginning of the object list.
		/*!
		Exception safety:
		nothrow
		*/
		ConstObjectIterator begin() const;

		ConstObjectDataIterator objectBegin() const;

		//! Returns an iterator to the end of the object list.
		/*!
		Exception safety:
		nothrow
		*/
		ConstObjectIterator end() const;

		ConstObjectDataIterator objectEnd() const;

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

		//! Returns the number of objects in the tree.
		/*!
		Exception safety:
		nothrow
		*/
		integer objects() const;

		//! Returns the dimension of the tree.
		/*!
		Exception safety:
		nothrow
		*/
		integer dimension() const;

		template <typename SplitRule_PointKdTree>
		void refine(const SplitRule_PointKdTree& splitRule,
			integer bucketSize = 8);

		//! Insert an object into the tree.
		ConstObjectIterator insert(const Object& object);

		//! Insert objects into the tree.
		/*!
		Exception safety:
		strong

		begin, end:
		An iterator range consisting of objects to insert.
		*/
		template <typename InputIterator>
		void insert(
			const InputIterator& begin, 
			const InputIterator& end);

		//! Insert objects into the tree.
		/*!
		Exception safety:
		strong

		begin, end:
		An iterator range consisting of objects to insert.

		iteratorSet:
		An output iterator to which the corresponding object
		iterators are reported.
		*/
		template <typename InputIterator,
			typename ConstObjectIterator_OutputIterator>
		void insert(
			const InputIterator& begin, 
			const InputIterator& end,
			ConstObjectIterator_OutputIterator iteratorSet);

		//! Removes a point from the tree.
		void erase(const ConstObjectIterator& iter);

		//! Clears off subdivision and objects.
		/*!
		Exception safety:
		nothrow
		*/
		void clear();

		//! Clears the objects but leaves the subdivision intact.
		/*!
		Exception safety:
		nothrow
		*/
		void eraseObjects();

		//! Clears the objects in a subtree but leaves subdivision intact.
		void eraseObjects(const Cursor& cursor);

		//! Collapses the tree into a single leaf node.
		void merge();

		//! Collapse a subtree into a leaf node.
		void merge(const Cursor& cursor);

		//! Returns the position of a given object.
		Vector<Real, N> point(const Object& object) const;

	private:
		class SplitPredicate;

		class ObjectInfo
		{
		public:
			friend class PointKdTree;

			// Implicit conversion allowed.
			ObjectInfo(
				const Object& object,
				Node* leafNode = 0)
				: object_(object)
				, leafNode_(leafNode)
			{
			}

			const Object& operator*() const
			{
				return object_;
			}

			const Object& object() const
			{
				return object_;
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

			Object object_;
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
			const ConstObjectIterator& first,
			const ConstObjectIterator& last,
			integer objects);

		//! Runs destructors for all nodes of a subtree.
		void destructSubtree(Node* node);

		//! Compute a bounding box for objects.
		AlignedBox<Real, N> computeBound(
			const ConstObjectIterator& begin, 
			const ConstObjectIterator& end) const;

		//! Compute a bounding box for objects.
		std::pair<Real, Real> computeBound(
			const ConstObjectIterator& begin, 
			const ConstObjectIterator& end,
			integer axis) const;

		//! Collapse a subtree into a leaf node.
		void merge(Node* node);

		//! Deallocate the nodes of a subtree.
		void erase(Node* node);

		//! Sets the leaf nodes of a range of objects.
		void setLeaf(
			const ConstObjectIterator& begin,
			const ConstObjectIterator& end,
			Node* node);

		//! Remove objects under a subtree.
		void eraseObjects(Node* node);

		//! Clear object ranges in a subtree, and set bucket nodes.
		/*!
		Note objects are not actually removed. Use eraseObjects()
		for this. This is actually foremost a helper function for
		eraseObjects().
		*/
		void clearObjects(Node* node);

		//! Updates hierarchical object information.
		void updateObjects(Node* node);

		//! Updates hierarchical bound information.
		void updateBounds(Node* node, const AlignedBox<Real, N>& bound);

		//! Propagates hierarchical information upwards.
		void updateObjectsUpwards(Node* node);

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

		//! Inserts new objects at the end of the objectList_.
		/*!
		returns:
		The first iterator of the inserted objects.

		Also updates the bounding box.
		*/
		template <typename InputIterator>
		ObjectIterator insertPrepare(
			const InputIterator& begin,
			const InputIterator& end);			

		//! Propagates new objects to leaf nodes.
		/*!
		Preconditions:
		1) [first, last] is an inclusive iterator range in 'objectList_'.
		2) count > 0
		
		If the node is an intermediate node,
		this function reorders 'objectList_'
		in the given range so that the objects
		going to the left node are listed
		before those going to the right node.
		This reordering is done by splicing a
		so no copying of objects is involved. 
		The function then recurses to both nodes.

		If the node is a leaf node,
		the given range is spliced to the correct
		position such that all objects of a leaf node
		are listed sequentially.
		*/
		void insert(
			Node* node,
			const ObjectIterator& first, 
			const ObjectIterator& last,
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
		objectList_:
		Contains all objects in the tree ordered
		in such a way that the objects of each leaf node 
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

		dimension_:
		The dimension of the tree.

		bound_:
		An axis aligned bounding box for the
		points in the tree.

		simulateKdTree_:
		If true, no shrinking is done to nodes.
		*/

		ObjectContainer objectList_;
		NodeAllocator nodeAllocator_;
		Node* root_;
		integer leaves_;
		PointPolicy pointPolicy_;
		integer dimension_;
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
