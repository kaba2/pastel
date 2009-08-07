// Description: PointKdTree class
// Detail: A kd-tree for points

#ifndef PASTEL_POINTKDTREE_H
#define PASTEL_POINTKDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/fastlist.h"
#include "pastel/sys/tristate.h"
#include "pastel/sys/poolallocator.h"

#include "pastel/geometry/alignedbox.h"

#include <boost/mpl/if.hpp>
#include <boost/iterator/indirect_iterator.hpp>

namespace Pastel
{

	template <typename Real, int N>
	class PointPolicy
	{
	public:
		typedef Point<Real, N> Object;

		const Point<Real, N>& point(const Object& object) const
		{
			return object;
		}

		Real point(const Object& object, integer axis) const
		{
			return object[axis];
		}
	};

	//! A kd-tree for point sets
	/*!
	class ObjectPolicy
	{
	public:
		typedef UserDefinedType Object;

		Point<Real, N> point(const Object& object) const;

		Real point(const Object& object, integer axis) const;
	};
	*/

	template <typename Real, int N = Dynamic, 
		typename ObjectPolicy = PointPolicy<Real, N> >
	class PointKdTree
	{
	public:
		typedef typename ObjectPolicy::Object Object;
		class Cursor;

	private:
		typedef PoolAllocator NodeAllocator;
		typedef PoolAllocator ObjectAllocator;

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
		typedef ObjectPolicy ObjectPolicy_;

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
		explicit PointKdTree(integer bucketSize = 16,
			const ObjectPolicy& objectPolicy = ObjectPolicy());

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
			integer bucketSize = 16,
			const ObjectPolicy& objectPolicy = ObjectPolicy());

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
		const ObjectPolicy& objectPolicy() const;

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

		//! Preferred number of points in a bucket node.
		/*!
		Exception safety:
		nothrow
		*/
		integer bucketSize() const;

		//! Refines the tree using the given splitting rule.
		/*!
		Preconditions:
		maxDepth >= 0
		maxObjects > 0

		Exception safety:
		Basic

		class SubdivisionRule
		{
		public:
			std::pair<Real, integer> operator()(
				const AlignedBox<Real, N>& bound,
				const ObjectPolicy& objectPolicy,
				const ConstObjectIterator& objectBegin,
				const ConstObjectIterator& objectEnd) const;
		};
		*/
		template <typename SubdivisionRule>
		void refine(
			const SubdivisionRule& subdivisionRule,
			integer maxDepth = 128);

		//! Insert objects in the tree.
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

	private:
		class SplitPredicate;

		class ObjectInfo
		{
		public:
			friend class PointKdTree;

			ObjectInfo(
				const Object& object,
				Node* leafNode)
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
		
			Cursor bucket() const
			{
				return Cursor((Node*)leafNode_->bucket());
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
			const ConstObjectIterator& begin,
			const ConstObjectIterator& last,
			integer objects);

		//! Collapse a subtree into a leaf node.
		void merge(Node* node);

		//! Deallocate the nodes of a subtree.
		void erase(Node* node);

		//! Find the bucket node starting from a given node.
		/*!
		The bucket node is searched from the given node 
		_upwards_.
		*/
		Node* findBucket(Node* node);

		//! Sets the buckets of non-empty leaf nodes of a subtree.
		void setBucket(Node* subtree, Node* bucket);

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

		//! Propagate _upwards_ the object set and count in a node.
		void updateObjects(Node* node);

		//! Propagate _upwards_ the object set in a node.
		/*!
		If the object count does not need to be propagated,
		this function is more efficient than updateObjects(), 
		because in most cases it does not need to traverse all 
		the way to the root.
		*/
		void updateObjectRanges(Node* node);

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
			const Real& boundMin,
			const Real& boundMax);

		//! Updates node bounds downwards.
		void updateBound(
			Node* node,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound);

		//! Propagates objects downwards by reordering object list.
		/*!
		Preconditions:
		1) [begin, end] is an iterator range in 'list'.
		2) count >= 0
		3) 'list' shares the same allocator with
		'objectList_'.
		
		Note that the 'list' given to this function
		is not necessarily the internal 'objectList_'.

		If the node is an intermediate node,
		this function reorders 'list'
		in the given range so that the objects
		going to the left node are listed
		before those going to the right node.
		This reordering is done by splicing a
		linked list so no copying of objects is 
		involved. The function then recurses to both 
		nodes.

		If the node is a leaf node,
		the object range is spliced to the internal
		'objectList_'. This is possible because
		both lists use a shared allocator.
		*/
		void spliceInsert(
			Node* node,
			ObjectContainer& list,
			const ObjectIterator& begin, 
			const ObjectIterator& end,
			integer count,
			Node* bucketNode);

		//! Subdivides the tree using the given subdivision rule.
		/*!
		Preconditions:
		1) maxDepth >= 0
		2) maxObjects >= 0
		3) depth >= 0
		4) depth <= maxDepth
		5) allLessEqual(minBound, maxBound)
		*/
		template <typename SubdivisionRule>
		void refine(
			Node* node,
			integer maxDepth,
			const SubdivisionRule& subdivisionRule,
			integer depth,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound);

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

		bound_:
		An axis aligned box containing all
		points in the tree.

		leaves_:
		The number of leaf nodes in the tree.

		objectPolicy_:
		A functor that maps the users Object's
		into geometric points. This allows
		to abstract the representation
		of points.

		dimension_:
		The dimension of the tree.

		bucketSize_:
		The maximum number of objects on a leaf node
		such that splitting is not performed.
		*/

		ObjectContainer objectList_;
		NodeAllocator nodeAllocator_;
		Node* root_;
		AlignedBox<Real, N> bound_;
		integer leaves_;
		ObjectPolicy objectPolicy_;
		integer dimension_;
		integer bucketSize_;
	};

}

#include "pastel/geometry/pointkdtree_splitpredicate.h"
#include "pastel/geometry/pointkdtree_node.h"
#include "pastel/geometry/pointkdtree_cursor.h"

#include "pastel/geometry/pointkdtree.hpp"
#include "pastel/geometry/pointkdtree_private.hpp"

#endif
