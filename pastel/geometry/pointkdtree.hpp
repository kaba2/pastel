#ifndef PASTEL_POINTKDTREE_HPP
#define PASTEL_POINTKDTREE_HPP

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/fastlist_tools.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy>
	PointKdTree<Real, N, ObjectPolicy>::PointKdTree(
		integer bucketSize,
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, bound_(N)
		, leaves_(0)
		, objectPolicy_(objectPolicy)
		, dimension_(N)
		, bucketSize_(bucketSize)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new ObjectAllocator(objectList_.get_allocator()->unitSize())));

		initialize();
	}

	template <typename Real, int N, typename ObjectPolicy>
	PointKdTree<Real, N, ObjectPolicy>::PointKdTree(
		Dimension dimension,
		integer bucketSize,
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, bound_(dimension)
		, leaves_(0)
		, objectPolicy_(objectPolicy)
		, dimension_(dimension)
		, bucketSize_(bucketSize)
	{
		ENSURE2((N != Dynamic && dimension == N) || 
			(N == Dynamic && dimension > 0), dimension, N);

		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new ObjectAllocator(objectList_.get_allocator()->unitSize())));

		initialize();
	}

	template <typename Real, int N, typename ObjectPolicy>
	PointKdTree<Real, N, ObjectPolicy>::PointKdTree(const PointKdTree& that)
		: objectList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, bound_(that.dimension_)
		, leaves_(0)
		, objectPolicy_(that.objectPolicy_)
		, dimension_(that.dimension_)
		, bucketSize_(that.bucketSize_)
	{
		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new ObjectAllocator(objectList_.get_allocator()->unitSize())));

		initialize();

		// First copy the structure of the tree.

		copyConstruct(root_, that.root_);

		// Then insert the objects to the nodes.

		insert(that.objectBegin(), that.objectEnd());
	}

	template <typename Real, int N, typename ObjectPolicy>
	PointKdTree<Real, N, ObjectPolicy>::~PointKdTree()
	{
		BOOST_STATIC_ASSERT(N > 0 || N == Dynamic);

		nodeAllocator_.clear();
	}

	template <typename Real, int N, typename ObjectPolicy>
	PointKdTree<Real, N, ObjectPolicy>&
		PointKdTree<Real, N, ObjectPolicy>::operator=(
		const PointKdTree& that)
	{
		PointKdTree copy(that);
		swap(copy);
		return *this;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::swap(
		PointKdTree& that)
	{
		objectList_.swap(that.objectList_);
		nodeAllocator_.swap(that.nodeAllocator_);
		std::swap(root_, that.root_);
		bound_.swap(that.bound_);
		std::swap(leaves_, that.leaves_);
		std::swap(objectPolicy_, that.objectPolicy_);
		std::swap(dimension_, that.dimension_);
		std::swap(bucketSize_, that.bucketSize_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	const ObjectPolicy& PointKdTree<Real, N, ObjectPolicy>::objectPolicy() const
	{
		return objectPolicy_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::reserveBound(
		const AlignedBox<Real, N>& boxToCover)
	{
		if (extendToCover(
			boxToCover, bound_))
		{
			updateBound(root_, bound_.min(), bound_.max());
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	const AlignedBox<Real, N>& PointKdTree<Real, N, ObjectPolicy>::bound() const
	{
		return bound_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	bool PointKdTree<Real, N, ObjectPolicy>::empty() const
	{
		return objectList_.empty();
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::Cursor
		PointKdTree<Real, N, ObjectPolicy>::root() const
	{
		return Cursor(root_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
		PointKdTree<Real, N, ObjectPolicy>::begin() const
	{
		return objectList_.begin();
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectDataIterator
		PointKdTree<Real, N, ObjectPolicy>::objectBegin() const
	{
		return ConstObjectDataIterator(objectList_.begin());
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
		PointKdTree<Real, N, ObjectPolicy>::end() const
	{
		return objectList_.end();
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectDataIterator
		PointKdTree<Real, N, ObjectPolicy>::objectEnd() const
	{
		return ConstObjectDataIterator(objectList_.end());
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer PointKdTree<Real, N, ObjectPolicy>::nodes() const
	{
		return nodeAllocator_.allocated();
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer PointKdTree<Real, N, ObjectPolicy>::leaves() const
	{
		return leaves_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer PointKdTree<Real, N, ObjectPolicy>::objects() const
	{
		return objectList_.size();
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer PointKdTree<Real, N, ObjectPolicy>::dimension() const
	{
		return dimension_;
	}
	
	template <typename Real, int N, typename ObjectPolicy>
	integer PointKdTree<Real, N, ObjectPolicy>::bucketSize() const
	{
		return bucketSize_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	template <typename SubdivisionRule>
	void PointKdTree<Real, N, ObjectPolicy>::refine(
		const SubdivisionRule& subdivisionRule,
		integer maxDepth)
	{
		ENSURE_OP(maxDepth, >=, 0);

		if (maxDepth == 0)
		{
			// Nothing to be done.
			return;
		}

		refine(root_, 
			maxDepth,
			subdivisionRule,
			0, 
			bound().min(), bound().max());
	}

	template <typename Real, int N, typename ObjectPolicy>
	template <typename InputIterator>
	void PointKdTree<Real, N, ObjectPolicy>::insert(
		const InputIterator& begin, 
		const InputIterator& end)
	{
		if (begin == end)
		{
			// Nothing to do.
			return;
		}

		// Copy objects to a list which shares
		// an allocator with the objectList_.
		ObjectContainer list(objectList_.get_allocator());

		// Possibly extend the bounding box.

		integer objects = 0;

		bool neededToExtend = false;
		InputIterator iter = begin;
		const InputIterator iterEnd = end;
		while(iter != iterEnd)
		{
			neededToExtend |= extendToCover(
				objectPolicy_.point(*iter), 
				bound_);

			list.push_back(
				ObjectInfo(*iter, (Node*)0));

			++objects;
			++iter;
		}

		if (neededToExtend)
		{
			updateBound(root_, bound_.min(), bound_.max());
		}

		// Splice the points to the leaf nodes.

		spliceInsert(root_, list, list.begin(), list.end(), objects, 0);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::erase(
		const ConstObjectIterator& iter)
	{
		Node* node = iter->leaf().node_;

		// Remove reference to the object
		// from this node.

		node->erase(iter, objectList_.end());

		// Actually remove the object from the object list.

		objectList_.erase(iter);

		// Propagate object set changes upwards.

		updateObjects(node);

		// Because we removed, rather than inserted, objects 
		// from the node, we can search the new
		// bucket node efficiently by starting from the 
		// current bucket node.

		node->setBucket(findBucket(node->bucket()));
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::clear()
	{
		objectList_.clear();
		nodeAllocator_.clear();
		root_ = 0;
		bound_ = AlignedBox<Real, N>(dimension_);
		leaves_ = 0;

		initialize();
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::eraseObjects()
	{
		eraseObjects(root_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::eraseObjects(
		const Cursor& cursor)
	{
		eraseObjects(cursor.node_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::merge()
	{
		merge(root_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::merge(
		const Cursor& cursor)
	{
		merge(cursor.node_);
	}

}

#endif
