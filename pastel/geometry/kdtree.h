#ifndef PASTELGEOMETRY_KDTREE_H
#define PASTELGEOMETRY_KDTREE_H

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

		AlignedBox<N, Real> bound(const Point<N, Real>& object) const
		{
			return AlignedBox<N, Real>(object);
		}

		Tuple<2, real> bound(const Point<N, Real>& object, integer axis) const
		{
			return Tuple<2, real>(object[axis]);
		}
	};

	//! A kd-tree
	/*!
	class ObjectPolicy
	{
	public:
		typedef UnspecifiedType Object;

		Tuple<2, Real> projection(const Object& that, index);
		AlignedBox<N, Real> bound(const Object& that);
	};
	*/

	template <int N, typename Real,
		typename ObjectPolicy = PointPolicy<N, Real> >
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
		class IntermediateNode_Low;
		class IntermediateNode_High;

		// There are two versions of the intermediate node class.
		// The first one uses bit tricks to pack the node
		// for compact storage. This trick only
		// works for dimension <= 8. For higher dimensions
		// one needs an additional byte per node (which
		// can mean still additional storage because
		// of padding).

		typedef typename boost::mpl::if_c<
			(N <= 8),
			IntermediateNode_Low,
			IntermediateNode_High>::type IntermediateNode;

	public:
		enum
		{
			N_ = N
		};
		typedef Real Real_;
		typedef ObjectPolicy ObjectPolicy_;

		typedef typename ObjectContainer::const_iterator ConstObjectIterator;

		class Cursor;

		//! Constructs an empty tree.
		KdTree();

		//! Constructs an empty tree.
		explicit KdTree(
			integer dimension,
			const ObjectPolicy& objectPolicy = ObjectPolicy());

		//! Constructs a copy from another tree.
		KdTree(const KdTree& that);

		//! Destructs the tree.
		~KdTree();

		//! Assigns another tree.
		KdTree& operator=(const KdTree& that);

		//! Swaps two trees.
		void swap(KdTree& that);

		//! Returns the object policy.
		const ObjectPolicy& objectPolicy() const;

		//! Extends the bounding box of the tree to cover the given box.
		void reserveBound(const AlignedBox<N, Real>& boxToCover);

		//! Returns the bounding box of the tree.
		const AlignedBox<N, Real>& bound() const;

		//! Returns true if there are no objects in the tree.
		bool empty() const;

		//! Returns the root node of the tree.
		Cursor root() const;

		//! Returns an iterator to the beginning of the object list.
		/*!
		Note that some objects might be listed multiple times
		if they pass over multiple leaf nodes. Points
		are the only kinds of objects that are never
		listed in more than one leaf node.
		*/
		ConstObjectIterator begin() const;

		//! Returns an iterator to the end of the object list.
		ConstObjectIterator end() const;

		//! Returns the number of nodes in the tree.
		integer nodes() const;

		//! Returns the number of leaf nodes in the tree.
		integer leaves() const;

		//! Returns the number of objects in the tree.
		integer objects() const;

		//! Returns the dimension of the tree.
		integer dimension() const;

		//! Subdivides a leaf node with the given plane.
		/*!
		Preconditions:
		cursor.leaf() == true
		'cursor' points to a node in this tree.

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
		begin, end:
		An iterator range consisting of objects to insert.
		*/

		template <typename InputIterator>
		void insert(
			InputIterator begin, InputIterator end);

		//! Remove an object from the tree.
		void erase(
			const Object& object);

		//! Clears off subdivision and objects.
		void clear();

		//! Clears the objects but leaves the subdivision intact.
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

				const Tuple<2, Real> bound = objectPolicy_.bound(object, splitAxis_);
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

#include "pastel/geometry/kdtree.hpp"

#endif
