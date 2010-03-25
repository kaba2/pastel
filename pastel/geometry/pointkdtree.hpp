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
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, leaves_(0)
		, objectPolicy_(objectPolicy)
		, dimension_(N)
	{
		ENSURE_OP(N, !=, Dynamic);
		//BOOST_STATIC_ASSERT(N != Dynamic);

		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new ObjectAllocator(objectList_.get_allocator()->unitSize())));

		initialize();
	}

	template <typename Real, int N, typename ObjectPolicy>
	PointKdTree<Real, N, ObjectPolicy>::PointKdTree(
		Dimension dimension,
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, leaves_(0)
		, objectPolicy_(objectPolicy)
		, dimension_(dimension)
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
		, leaves_(0)
		, objectPolicy_(that.objectPolicy_)
		, dimension_(that.dimension_)
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

		/*
		Time complexity:
		
		* Freeing the raw memory for the m nodes takes O(m), 
		although we do not necessarily need to run their destructors.

		* Destructing the object list with n objects 
		all at once takes O(n).
		*/

		destructSubtree(root_);
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
		std::swap(leaves_, that.leaves_);
		std::swap(objectPolicy_, that.objectPolicy_);
		std::swap(dimension_, that.dimension_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	const ObjectPolicy& PointKdTree<Real, N, ObjectPolicy>::objectPolicy() const
	{
		return objectPolicy_;
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
	void PointKdTree<Real, N, ObjectPolicy>::refine(
		integer maxDepth,
		integer bucketSize)
	{
		ENSURE_OP(maxDepth, >=, 0);
		ENSURE_OP(bucketSize, >=, 1);

		if (maxDepth == 0)
		{
			// Nothing to be done.
			return;
		}

		Vector<Real, N> minBound(-infinity<Real>());
		Vector<Real, N> maxBound(infinity<Real>());

		refine(root_, 
			maxDepth,
			0,
			bucketSize,
			minBound,
			maxBound);
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator 
		PointKdTree<Real, N, ObjectPolicy>::insert(const Object& object)
	{
		// Copy the object to the end of objectList_.

		objectList_.push_back(object);
		
		ObjectIterator iter = objectList_.end();
		--iter;

		// Splice the point to the leaf node.

		AlignedBox<Real, N> objectBound(ofDimension(dimension_));

		insert(root_, iter, iter, 1, objectBound);

		return iter;
	}

	template <typename Real, int N, typename ObjectPolicy>
	template <typename InputIterator,
		typename ConstObjectIterator_OutputIterator>
	void PointKdTree<Real, N, ObjectPolicy>::insert(
		const InputIterator& begin, 
		const InputIterator& end,
		ConstObjectIterator_OutputIterator iteratorSet)
	{
		if (begin == end)
		{
			// Nothing to do.
			return;
		}

		// Prepare for insertion.
		const ObjectIterator first = insertPrepare(begin, end);

		// Copy the new object iterators to the user.
		std::copy(countingIterator(first),
			countingIterator(objectList_.end()),
			iteratorSet);

		// Send the objects down the tree.
		const integer objects = std::distance(begin, end);
		ObjectIterator last = objectList_.end();
		--last;

		AlignedBox<Real, N> objectBound(ofDimension(dimension_));
		insert(root_, first, last, objects, objectBound);
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

		// Prepare for insertion.
		const ObjectIterator first = insertPrepare(begin, end);

		// Send the objects down the tree.
		const integer objects = std::distance(begin, end);

		ObjectIterator last = objectList_.end();
		--last;

		AlignedBox<Real, N> objectBound(ofDimension(dimension_));
		insert(root_, first, last, objects, objectBound);
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
		updateObjectsUpwards(node);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::clear()
	{
		objectList_.clear();
		destructSubtree(root_);
		nodeAllocator_.clear();
		root_ = 0;
		leaves_ = 0;

		initialize();
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::eraseObjects()
	{
		// We can clear all of the objects faster and
		// more storage-efficiently than using 
		// eraseObjects(root()).

		// Actually remove the objects.
		
		objectList_.clear();

		// Clear the object ranges in the subtree.

		clearObjects(root_);
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
		// We can merge all of the nodes faster and
		// more storage-efficiently than using
		// merge(root_).

		ConstObjectIterator begin = root_->first();
		ConstObjectIterator last = root_->last();

		destructSubtree(root_);
		nodeAllocator_.clear();

		root_ = allocateLeaf(
			0,
			begin, 
			last,
			objects());

		leaves_ = 1;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::merge(
		const Cursor& cursor)
	{
		// Note: We take the optimization for 
		// the case cursor == root() inside this
		// function.
		merge(cursor.node_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	Vector<Real, N> PointKdTree<Real, N, ObjectPolicy>::point(const Object& object) const
	{
		return Vector<Real, N>(ofDimension(dimension_), 
			withAliasing((Real*)objectPolicy_.point(object))); 
	}

}

#endif
