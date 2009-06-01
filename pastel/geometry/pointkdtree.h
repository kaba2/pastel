#ifndef PASTEL_POINTKDTREE_H
#define PASTEL_POINTKDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/fastlist.h"
#include "pastel/sys/arenaallocator.h"
#include "pastel/sys/tristate.h"
//#include "pastel/sys/poolallocator.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	class PointPolicy
	{
	public:
		typedef Point<N, Real> Object;

		const Point<N, Real>& point(const Object& object) const
		{
			return object;
		}

		Real point(const Object& object, integer axis) const
		{
			return object[axis];
		}
	};

	//! A kd-tree
	/*!
	class ObjectPolicy
	{
	public:
		typedef UnspecifiedType Object;

		Point<2, Real> point(const Object& that) const;
		Real point(const Object& that, integer axis) const;
	};
	*/

	template <int N, typename Real,
		typename ObjectPolicy = PointPolicy<N, Real> >
	class PointKdTree
	{
	public:
		typedef typename ObjectPolicy::Object Object;

	private:
		typedef ArenaAllocator Allocator;

		typedef FastList<Object, Allocator> ObjectContainer;
		typedef typename ObjectContainer::iterator ObjectIterator;

		class Node;
		class LeafNode;
		class IntermediateNode_Low;
		class IntermediateNode_High;
		class Bounds;

		// There are two versions of the intermediate node class.
		// IntermediateNode_Low uses bit tricks to pack the split axis
		// for compact storage. This trick only
		// works for dimension <= 8. Although saving memory,
		// the packing and unpacking degrades performance slightly.
		// For higher dimensions one needs to store the split axis 
		// in its own variable, as is done in IntermediateNode_High.

		/*
		typedef typename boost::mpl::if_c<
			(N != Dynamic && N <= 8),
			IntermediateNode_Low,
			IntermediateNode_High>::type IntermediateNode;
		*/

		typedef IntermediateNode_High IntermediateNode;

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
		PointKdTree();

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
		explicit PointKdTree(
			integer dimension,
			const ObjectPolicy& objectPolicy = ObjectPolicy());

		//! Constructs a copy from another tree.
		/*!
		Time complexity:
		?

		Exception safety:
		?
		*/
		PointKdTree(const PointKdTree& that);

		//! Destructs the tree.
		/*!
		Time complexity:
		Linear on nodes().

		Exception safety:
		nothrow
		*/
		~PointKdTree();

		//! Assigns another tree.
		/*!
		Time complexity:
		?

		Exception safety:
		strong
		*/
		PointKdTree& operator=(const PointKdTree& that);

		//! Swaps two trees.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		void swap(PointKdTree& that);

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
		void reserveBound(const AlignedBox<N, Real>& boxToCover);

		//! Returns the bounding box of the tree.
		/*!
		Time complexity:
		Constant

		Exception safety:
		nothrow
		*/
		const AlignedBox<N, Real>& bound() const;

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
				const AlignedBox<N, Real>& bound,
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

		FIX: Make this private.
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

		//! Remove an object from the tree.
		/*!
		Time complexity:
		?

		Exception safety:
		nothrow
		*/
		void erase(
			const Object& object);

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
		class RefineEntry
		{
		public:
			RefineEntry()
				: cursor_()
				, depth_(0)
				, bound_()
			{
			}

			RefineEntry(
				const Cursor& cursor,
				integer depth,
				const AlignedBox<N, Real>& bound)
				: cursor_(cursor)
				, depth_(depth)
				, bound_(bound)
			{
			}

			Cursor cursor_;
			integer depth_;
			AlignedBox<N, Real> bound_;
		};

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
			
			bool operator()(const Object& object) const
			{
				// The kd-tree nodes are
				// half-open boxes of the form [min, max[.

				return objectPolicy_.point(object, splitAxis_) < 
					splitPosition_;
			}

		private:
			Real splitPosition_;
			integer splitAxis_;
			ObjectPolicy objectPolicy_;
		};

		void subdivide(
			const Cursor& cursor,
			const Real& splitPosition,
			integer splitAxis,
			const Real& boundMin,
			const Real& boundMax);

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
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound);

		ObjectContainer objectList_;
		Allocator nodeAllocator_;
		Node* root_;
		AlignedBox<N, Real> bound_;
		integer leaves_;

		// Note: this is not the same as
		// objectList_.size(), since objectList_ can
		// contain the same object multiple times.
		integer objects_;

		ObjectPolicy objectPolicy_;
		integer dimension_;
	};

}

#include "pastel/geometry/pointkdtree.hpp"

#endif
