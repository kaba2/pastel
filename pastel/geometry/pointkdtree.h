#ifndef PASTEL_POINTKDTREE_H
#define PASTEL_POINTKDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/fastlist.h"
#include "pastel/sys/arenaallocator.h"
#include "pastel/sys/tristate.h"
//#include "pastel/sys/poolallocator.h"

#include "pastel/geometry/alignedbox.h"

#include <boost/mpl/if.hpp>

namespace Pastel
{

	template <int N, typename Real>
	class PointPolicy
	{
	public:
		typedef Point<N, Real> Object;
		//typedef TrueType ArbitrarySplits;
		typedef FalseType ArbitrarySplits;

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
		typedef (TrueType | FalseType) ArbitrarySplits;

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
		typedef typename ObjectPolicy::ArbitrarySplits ArbitrarySplits;

	private:
		typedef ArenaAllocator Allocator;

		typedef FastList<Object, Allocator> ObjectContainer;
		typedef typename ObjectContainer::iterator ObjectIterator;

		class Node;
		class LeafNode;
		class IntermediateNode_BspTree;
		class IntermediateNode_KdTree;

		typedef boost::is_same<TrueType, typename ObjectPolicy::ArbitrarySplits>
			UseArbitrarySplits;

		typedef typename boost::mpl::if_<
			UseArbitrarySplits,
			IntermediateNode_BspTree,
			IntermediateNode_KdTree>::type IntermediateNode;

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
		class SplitPredicate_KdTree
		{
		public:
			SplitPredicate_KdTree(
				const Real& splitPosition,
				integer splitAxis,
				const Vector<N, Real>* splitDirection,
				const ObjectPolicy& objectPolicy)
				: splitPosition_(splitPosition)
				, splitAxis_(splitAxis)
				, objectPolicy_(objectPolicy)
			{
				// splitDirection is not used.
			}
			
			TriState::Enum operator()(const Object& object) const
			{
				return triLess(objectPolicy_.point(object, splitAxis_),
					splitPosition_);
			}

		private:
			Real splitPosition_;
			integer splitAxis_;
			const ObjectPolicy& objectPolicy_;
		};

		class SplitPredicate_BspTree
		{
		public:
			SplitPredicate_BspTree(
				const Real& splitPosition,
				integer splitAxis,
				const Vector<N, Real>* splitDirection,
				const ObjectPolicy& objectPolicy)
				: splitPosition_(splitPosition)
				, splitDirection_(*splitDirection)
				, objectPolicy_(objectPolicy)
			{
				ASSERT(splitDirection);
				// splitAxis is not used.
			}
			
			TriState::Enum operator()(const Object& object) const
			{
				return triLess(
					dot(asVector(objectPolicy_.point(object)), splitDirection_), 
					splitPosition_);
			}

		private:
			Real splitPosition_;
			const Vector<N, Real>& splitDirection_;
			const ObjectPolicy& objectPolicy_;
		};

		typedef typename boost::mpl::if_<
			UseArbitrarySplits,
			SplitPredicate_BspTree, 
			SplitPredicate_KdTree>::type SplitPredicate;

		//! Subdivides a leaf node with the given plane.
		/*!
		Preconditions:
		1) cursor.leaf() == true
		2) 'cursor' points to a node in this tree.
		3) 0 <= splitAxis < dimension()
		4) If splitDirection != 0, then 
		splitAxis is the standard basis axis which 
		has minimal angle with *splitDirection.
		5) If splitDirection != 0, then
		splitPosition = dot(asVector(planePosition), splitDirection)

		Time complexity:
		Linear on cursor.objects().

		Exception safety:
		strong

		cursor:
		A cursor to a leaf node of the tree to subdivide.

		splitPosition:
		Signed distance of the splitting plane from the origin.

		splitAxis:
		Index of the standard basis axis which is closest
		(in angle) to the splitting plane normal.

		splitDirection:
		If not 0, denotes the normal of the splitting
		plane normal. If 0, the splitting plane normal
		is the standard basis axis given by 'splitAxis'.
		*/
		void subdivide(
			const Cursor& cursor,
			const Real& splitPosition,
			integer splitAxis,
			const Vector<N, Real>* splitDirection,
			const Real& boundMin,
			const Real& boundMax,
			const Real& positiveMin,
			const Real& negativeMax);

		//! Inserts objects into a specific leaf node.
		/*!
		Preconditions:
		1) cursor.leaf() == true
		*/

		template <typename InputIterator>
		void insert(
			const Cursor& cursor,
			InputIterator begin, InputIterator end);

		//! Removes all objects, but retains nodes.
		void clearObjects(const Cursor& cursor);

		//! Propagates objects by reordering object list.
		/*!
		Preconditions:
		1) [begin, end] is an iterator range in 'list'.
		2) count >= 0
		3) 'list' shares the same allocator with
		'objectList_'.
		
		Note that the 'list' given to this function
		is not the internal 'objectList_'.

		If the node is an intermediate node,
		this function reorders 'list'
		in the given range so that the objects
		going to the negative node are listed
		before those going to the positive node.
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
			const Cursor& cursor,
			ObjectContainer& list,
			ObjectIterator begin, ObjectIterator last,
			integer count);

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
		// It always holds that the
		// objects are located inside
		// the 'bound_'.
		AlignedBox<N, Real> bound_;
		integer leaves_;
		ObjectPolicy objectPolicy_;
		integer dimension_;
	};

}

#include "pastel/geometry/pointkdtree.hpp"

#endif
