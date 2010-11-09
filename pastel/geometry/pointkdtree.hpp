#ifndef PASTEL_POINTKDTREE_HPP
#define PASTEL_POINTKDTREE_HPP

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/fastlist_tools.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>::PointKdTree(
		bool simulateKdTree,
		const PointPolicy& pointPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, leaves_(0)
		, pointPolicy_(pointPolicy)
		, bound_(pointPolicy.dimension())
		, simulateKdTree_(simulateKdTree)
	{
		ENSURE(N == Dynamic || 
			N == pointPolicy.dimension());

		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new ObjectAllocator(objectList_.get_allocator()->unitSize())));

		initialize();
	}

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>::PointKdTree(const PointKdTree& that)
		: objectList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, leaves_(0)
		, pointPolicy_(that.pointPolicy_)
		, bound_(that.bound_)
		, simulateKdTree_(that.simulateKdTree)
	{
		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new ObjectAllocator(objectList_.get_allocator()->unitSize())));

		initialize();

		// First copy the structure of the tree.

		copyConstruct(root_, that.root_);

		// Then insert the objects to the nodes.

		insert(
			forwardRange(that.objectBegin(), that.objectEnd()));
	}

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>::~PointKdTree()
	{
		PASTEL_STATIC_ASSERT(N > 0 || N == Dynamic);

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

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>&
		PointKdTree<Real, N, PointPolicy>::operator=(
		const PointKdTree& that)
	{
		PointKdTree copy(that);
		swap(copy);
		return *this;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::swap(
		PointKdTree& that)
	{
		objectList_.swap(that.objectList_);
		nodeAllocator_.swap(that.nodeAllocator_);
		std::swap(root_, that.root_);
		std::swap(leaves_, that.leaves_);
		std::swap(pointPolicy_, that.pointPolicy_);
		bound_.swap(that.bound_);
		std::swap(simulateKdTree_, that.simulateKdTree_);
	}

	template <typename Real, int N, typename PointPolicy>
	const PointPolicy& PointKdTree<Real, N, PointPolicy>::pointPolicy() const
	{
		return pointPolicy_;
	}

	template <typename Real, int N, typename PointPolicy>
	const AlignedBox<Real, N>& 
		PointKdTree<Real, N, PointPolicy>::bound() const
	{
		return bound_;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::reserveBound(
		const AlignedBox<Real, N>& boxToCover)
	{
		extendToCover(boxToCover, bound_);
	}

	template <typename Real, int N, typename PointPolicy>
	bool PointKdTree<Real, N, PointPolicy>::empty() const
	{
		return objectList_.empty();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Cursor
		PointKdTree<Real, N, PointPolicy>::root() const
	{
		return Cursor(root_);
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator
		PointKdTree<Real, N, PointPolicy>::begin() const
	{
		return objectList_.begin();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstObjectDataIterator
		PointKdTree<Real, N, PointPolicy>::objectBegin() const
	{
		return ConstObjectDataIterator(objectList_.begin());
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator
		PointKdTree<Real, N, PointPolicy>::end() const
	{
		return objectList_.end();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstObjectDataIterator
		PointKdTree<Real, N, PointPolicy>::objectEnd() const
	{
		return ConstObjectDataIterator(objectList_.end());
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::nodes() const
	{
		return nodeAllocator_.allocated();
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::leaves() const
	{
		return leaves_;
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::objects() const
	{
		return objectList_.size();
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::dimension() const
	{
		return pointPolicy_.dimension();
	}
	
	template <typename Real, int N, typename PointPolicy>
	template <typename SplitRule_PointKdTree>
	void PointKdTree<Real, N, PointPolicy>::refine(
		const SplitRule_PointKdTree& splitRule,
		integer bucketSize)
	{
		Vector<Real, N> minBound(bound_.min());
		Vector<Real, N> maxBound(bound_.max());

		refine(root_, 
			minBound,
			maxBound,
			splitRule,
			0,
			bucketSize);
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator 
		PointKdTree<Real, N, PointPolicy>::insert(const Object& object)
	{
		// Copy the object to the end of objectList_.

		objectList_.push_back(object);
		
		ObjectIterator iter = objectList_.end();
		--iter;

		// Splice the point to the leaf node.

		AlignedBox<Real, N> objectBound(
			ofDimension(dimension()));

		insert(root_, iter, iter, 1, objectBound);

		extendToCover(objectBound, bound_);

		return iter;
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename InputIterator,
		typename ConstObjectIterator_OutputIterator>
	void PointKdTree<Real, N, PointPolicy>::insert(
		const ForwardRange<InputIterator>& objectSet, 
		ConstObjectIterator_OutputIterator iteratorSet)
	{
		if (objectSet.empty())
		{
			// Nothing to do.
			return;
		}

		const InputIterator begin = objectSet.begin();
		const InputIterator end = objectSet.end();

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

		AlignedBox<Real, N> objectBound(
			ofDimension(dimension()));
		insert(root_, first, last, objects, objectBound);

		extendToCover(objectBound, bound_);
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename InputIterator>
	void PointKdTree<Real, N, PointPolicy>::insert(
		const ForwardRange<InputIterator>& objectSet)
	{
		if (objectSet.empty())
		{
			// Nothing to do.
			return;
		}

		const InputIterator begin = objectSet.begin();
		const InputIterator end = objectSet.end();

		// Prepare for insertion.
		const ObjectIterator first = insertPrepare(begin, end);

		// Send the objects down the tree.
		const integer objects = std::distance(begin, end);

		ObjectIterator last = objectList_.end();
		--last;

		AlignedBox<Real, N> objectBound(
			ofDimension(dimension()));
		insert(root_, first, last, objects, objectBound);

		extendToCover(objectBound, bound_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erase(
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

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::clear()
	{
		objectList_.clear();
		destructSubtree(root_);
		nodeAllocator_.clear();
		root_ = 0;
		leaves_ = 0;
		bound_ = AlignedBox<Real, N>(
			ofDimension(dimension()));

		initialize();
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::eraseObjects()
	{
		// We can clear all of the objects faster and
		// more storage-efficiently than using 
		// eraseObjects(root()).

		// Actually remove the objects.
		
		objectList_.clear();

		// Clear the object ranges in the subtree.

		clearObjects(root_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::eraseObjects(
		const Cursor& cursor)
	{
		eraseObjects(cursor.node_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::merge()
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

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::merge(
		const Cursor& cursor)
	{
		// Note: We take the optimization for 
		// the case cursor == root() inside this
		// function.
		merge(cursor.node_);
	}

	template <typename Real, int N, typename PointPolicy>
	Vector<Real, N> PointKdTree<Real, N, PointPolicy>::point(
		const Object& object) const
	{
		return pointPolicy_(object);
	}

}

#endif
