// Description: KdTree class
// Detail: A kd-tree data structure for ray casting shapes

#ifndef PASTELGEOMETRY_KDTREE_H
#define PASTELGEOMETRY_KDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/fastlist.h"
#include "pastel/sys/arena_allocator.h"
#include "pastel/sys/tristate.h"
//#include "pastel/sys/pool_allocator.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class PointPolicy
	{
	public:
		typedef Vector<Real, N> Object;

		AlignedBox<Real, N> bound(const Vector<Real, N>& object) const
		{
			return AlignedBox<Real, N>(object);
		}

		Tuple<Real, 2> bound(const Vector<Real, N>& object, integer axis) const
		{
			return Tuple<Real, 2>(object[axis]);
		}
	};

	//! A kd-tree
	/*!
	class ObjectPolicy
	{
	public:
		typedef UnspecifiedType Object;

		AlignedBox<Real, N> bound(const Object& that) const;
		Tuple<Real, 2> bound(const Object& that, index) const;
	};
	*/

	template <typename Real, int N = Dynamic,
		typename ObjectPolicy = PointPolicy<Real, N> >
	class KdTree
	{
	public:
		typedef typename ObjectPolicy::Object Object;

	private:
		typedef ArenaAllocator Allocator;

		typedef FastList<Object, Allocator> ObjectContainer;
		typedef typename ObjectContainer::iterator ObjectIterator;

		class Node;
		class LeafNode;
		class SplitNode_Low;
		class SplitNode_High;

		// There are two versions of the intermediate node class.
		// SplitNode_Low uses bit tricks to pack the split axis
		// for compact storage. This trick only
		// works for dimension <= 8. Although saving memory,
		// the packing and unpacking degrades performance slightly.
		// For higher dimensions one needs to store the split axis 
		// in its own variable, as is done in SplitNode_High.

		/*
		typedef typename boost::mpl::if_c<
			(N != Dynamic && N <= 8),
			SplitNode_Low,
			SplitNode_High>::type SplitNode;
		*/

		typedef SplitNode_High SplitNode;

	public:
		enum
		{
			N_ = N
		};
		typedef Real Real_;
		typedef ObjectPolicy ObjectPolicy_;

		typedef typename ObjectContainer::const_iterator 
			ConstObjectIterator;

		class Cursor;

		//! Constructs an empty tree.
		/*!
		Time complexity:
		Constant

		Exception safety:
		strong
		*/
		KdTree();

		//! Constructs an empty tree.
		/*!
		Preconditions:
		dimension > 0
		dimension == N || N == Dynamic
		
		Time complexity:
		Constant

		Exception safety:
		strong
		*/
		explicit KdTree(
			integer dimension,
			const ObjectPolicy& objectPolicy = ObjectPolicy());

		//! Constructs a copy from another tree.
		/*!
		Time complexity:
		?

		Exception safety:
		?
		*/
		KdTree(const KdTree& that);

		//! Destructs the tree.
		/*!
		Time complexity:
		Linear on nodes().

		Exception safety:
		nothrow
		*/
		~KdTree();

		//! Assigns another tree.
		/*!
		Time complexity:
		?

		Exception safety:
		strong
		*/
		KdTree& operator=(const KdTree& that);

		//! Swaps two trees.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		void swap(KdTree& that);

		//! Returns the object policy.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		const ObjectPolicy& objectPolicy() const;

		//! Extends the bounding box of the tree to cover the given box.
		/*!
		Time complexity:
		O(dimension())

		Exception safety:
		strong (FIX: make it nothrow)
		*/
		void reserveBound(const AlignedBox<Real, N>& boxToCover);

		//! Returns the bounding box of the tree.
		/*!
		Time complexity:
		Constant

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
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		Cursor root() const;

		//! Returns an iterator to the beginning of the object list.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow

		Note that some objects might be listed multiple times
		if they pass over multiple leaf nodes. Points
		are the only kinds of objects that are never
		listed in more than one leaf node.
		*/
		ConstObjectIterator begin() const;

		//! Returns an iterator to the end of the object list.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		ConstObjectIterator end() const;

		//! Returns the number of nodes in the tree.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow

		This number includes both intermediate nodes
		and leaf nodes.
		*/
		integer nodes() const;

		//! Returns the number of leaf nodes in the tree.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		integer leaves() const;

		//! Returns the number of objects in the tree.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		integer objects() const;

		//! Returns the dimension of the tree.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		integer dimension() const;

		//! Refines the tree using the given splitting rule.
		/*!
		Preconditions:
		maxDepth >= 0
		maxObjects > 0

		Time complexity:
		?

		Exception safety:
		Basic

		class SubdivisionRule
		{
		public:
			std::pair<Real, integer> operator()(
				const Vector<Real, N>& minBound,
				const Vector<Real, N>& maxBound,
				const ObjectPolicy& objectPolicy,
				const ConstObjectIterator& objectBegin,
				const ConstObjectIterator& objectEnd) const;
		};
		*/
		template <typename SubdivisionRule>
		void refine(
			integer maxDepth,
			integer maxObjects,
			const SubdivisionRule& subdivisionRule);

		//! Subdivides a leaf node with the given plane.
		/*!
		Preconditions:
		cursor.leaf() == true
		'cursor' points to a node in this tree.
		0 <= splitAxis < dimension()

		Time complexity:
		Linear on cursor.objects().

		Exception safety:
		strong

		cursor:
		A cursor to a leaf node of the tree to subdivide.

		splitPosition:
		The position of the splitting plane on the splitting axis.

		splitAxis:
		The axis of the splitting plane normal.
		*/
		void subdivide(
			const Cursor& cursor,
			const Real& splitPosition,
			integer splitAxis);

		//! Insert objects in the tree.
		/*!
		Time complexity:
		O(objects * depth).

		Exception safety:
		strong

		begin, end:
		An iterator range consisting of objects to insert.
		*/
		template <typename InputIterator>
		void insert(
			InputIterator begin, InputIterator end);

		//! Clears off subdivision and objects.
		/*!
		Time complexity:
		O(nodes() + objects())

		Exception safety:
		nothrow
		*/
		void clear();

		//! Clears the objects but leaves the subdivision intact.
		/*!
		Time complexity:
		O(objects())

		Exception safety:
		nothrow
		*/
		void clearObjects();

	private:
		class SplitPredicate
		{
		public:
			SplitPredicate(
				const Real& splitPosition,
				integer splitAxis,
				const ObjectPolicy& objectPolicy)
				: splitPosition_(splitPosition)
				, splitAxis_(splitAxis)
				, objectPolicy_(objectPolicy)
			{
			}

			TriState::Enum operator()(const Object& object) const
			{
				// The kd-tree nodes are
				// half-open boxes of the form [min, max[.

				const Tuple<Real, 2> bound = objectPolicy_.bound(object, splitAxis_);
				if (bound[1] < splitPosition_)
				{
					return TriState::True;
				}
				if (bound[0] >= splitPosition_)
				{
					return TriState::False;
				}

				return TriState::Both;
			}

		private:
			Real splitPosition_;
			integer splitAxis_;
			ObjectPolicy objectPolicy_;
		};

		template <typename InputIterator>
		void insert(
			const Cursor& cursor,
			InputIterator begin, InputIterator end);

		void clearObjects(const Cursor& cursor);

		void spliceInsert(
			const Cursor& cursor,
			ObjectContainer& list,
			ObjectIterator begin, ObjectIterator last,
			integer count);

		template <typename SubdivisionRule>
		void refine(
			integer maxDepth,
			integer maxObjects,
			const SubdivisionRule& subdivisionRule,
			const Cursor& cursor,
			integer depth,
			const Vector<Real, N>& minBound,
			const Vector<Real, N>& maxBound);

		ObjectContainer objectList_;
		Allocator nodeAllocator_;
		Node* root_;
		AlignedBox<Real, N> bound_;
		integer leaves_;

		// Note: this is not the same as
		// objectList_.size(), since objectList_ can
		// contain the same object multiple times.
		integer objects_;

		ObjectPolicy objectPolicy_;
		integer dimension_;
	};

}

#include "pastel/geometry/kdtree.hpp"

#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/kdtree_refine.h"

#endif
