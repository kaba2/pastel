#ifndef PASTEL_POINTKDTREE_HPP
#define PASTEL_POINTKDTREE_HPP

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/intersect_alignedbox_plane.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/fastlist_tools.h"

#include <boost/operators.hpp>

#include "pastel/geometry/pointkdtree_splitpredicate.h"
#include "pastel/geometry/pointkdtree_node.h"
#include "pastel/geometry/pointkdtree_cursor.h"

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	PointKdTree<N, Real, ObjectPolicy>::PointKdTree(
		integer bucketSize,
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(SplitNode), 1024)
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

	template <int N, typename Real, typename ObjectPolicy>
	PointKdTree<N, Real, ObjectPolicy>::PointKdTree(
		Dimension dimension,
		integer bucketSize,
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(SplitNode), 1024)
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

	template <int N, typename Real, typename ObjectPolicy>
	PointKdTree<N, Real, ObjectPolicy>::PointKdTree(const PointKdTree& that)
		: objectList_()
		, nodeAllocator_(sizeof(SplitNode), 1024)
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

	template <int N, typename Real, typename ObjectPolicy>
	PointKdTree<N, Real, ObjectPolicy>::~PointKdTree()
	{
		// This is what we assume for memory allocation.
		BOOST_STATIC_ASSERT(sizeof(LeafNode) <= sizeof(SplitNode));
		BOOST_STATIC_ASSERT(N > 0 || N == Dynamic);

		nodeAllocator_.clear();
	}

	template <int N, typename Real, typename ObjectPolicy>
	PointKdTree<N, Real, ObjectPolicy>&
		PointKdTree<N, Real, ObjectPolicy>::operator=(
		const PointKdTree& that)
	{
		PointKdTree copy(that);
		swap(copy);
		return *this;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::swap(
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

	template <int N, typename Real, typename ObjectPolicy>
	const ObjectPolicy& PointKdTree<N, Real, ObjectPolicy>::objectPolicy() const
	{
		return objectPolicy_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::reserveBound(
		const AlignedBox<Real, N>& boxToCover)
	{
		if (extendToCover(
			boxToCover, bound_))
		{
			updateBound(root_, bound_.min(), bound_.max());
		}
	}

	template <int N, typename Real, typename ObjectPolicy>
	const AlignedBox<Real, N>& PointKdTree<N, Real, ObjectPolicy>::bound() const
	{
		return bound_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	bool PointKdTree<N, Real, ObjectPolicy>::empty() const
	{
		return objectList_.empty();
	}

	template <int N, typename Real, typename ObjectPolicy>
	typename PointKdTree<N, Real, ObjectPolicy>::Cursor
		PointKdTree<N, Real, ObjectPolicy>::root() const
	{
		return Cursor(root_);
	}

	template <int N, typename Real, typename ObjectPolicy>
	typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		PointKdTree<N, Real, ObjectPolicy>::begin() const
	{
		return objectList_.begin();
	}

	template <int N, typename Real, typename ObjectPolicy>
	typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectDataIterator
		PointKdTree<N, Real, ObjectPolicy>::objectBegin() const
	{
		return ConstObjectDataIterator(objectList_.begin());
	}

	template <int N, typename Real, typename ObjectPolicy>
	typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		PointKdTree<N, Real, ObjectPolicy>::end() const
	{
		return objectList_.end();
	}

	template <int N, typename Real, typename ObjectPolicy>
	typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectDataIterator
		PointKdTree<N, Real, ObjectPolicy>::objectEnd() const
	{
		return ConstObjectDataIterator(objectList_.end());
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer PointKdTree<N, Real, ObjectPolicy>::nodes() const
	{
		return nodeAllocator_.allocated();
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer PointKdTree<N, Real, ObjectPolicy>::leaves() const
	{
		return leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer PointKdTree<N, Real, ObjectPolicy>::objects() const
	{
		return objectList_.size();
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer PointKdTree<N, Real, ObjectPolicy>::dimension() const
	{
		return dimension_;
	}
	
	template <int N, typename Real, typename ObjectPolicy>
	integer PointKdTree<N, Real, ObjectPolicy>::bucketSize() const
	{
		return bucketSize_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	template <typename SubdivisionRule>
	void PointKdTree<N, Real, ObjectPolicy>::refine(
		const SubdivisionRule& subdivisionRule,
		integer maxDepth)
	{
		ENSURE_OP(maxDepth, >=, 0);

		if (maxDepth == 0)
		{
			// Nothing to be done.
			return;
		}

		refine(root_, maxDepth,
			subdivisionRule,
			0, bound().min(), bound().max());
	}

	template <int N, typename Real, typename ObjectPolicy>
	template <typename InputIterator>
	void PointKdTree<N, Real, ObjectPolicy>::insert(
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
				objectPolicy_.point(*iter), bound_);

			list.push_back(
				ObjectInfo(*iter, (LeafNode*)0));

			++objects;
			++iter;
		}

		if (neededToExtend)
		{
			updateBound(root_, bound_.min(), bound_.max());
		}

		// Splice the points to the leaf nodes.

		spliceInsert(root_, list, list.begin(), list.end(), objects);
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::erase(
		const ConstObjectIterator& iter)
	{
		const Cursor bucket = iter->bucket();
		
		LeafNode* leafNode = (LeafNode*)bucket.node_;
		leafNode->erase(iter, objectList_.end());
		objectList_.erase(iter);
		
		updateEmptyBits(leafNode);
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::clear()
	{
		objectList_.clear();
		nodeAllocator_.clear();
		root_ = 0;
		bound_ = AlignedBox<Real, N>(dimension_);
		leaves_ = 0;

		initialize();
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::clearObjects()
	{
		clearObjects(root());
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::clearObjects(
		const Cursor& cursor)
	{
		if (cursor.leaf())
		{
			// Clear the object references.

			LeafNode* node = (LeafNode*)cursor.node_;

			ConstObjectIterator iter = cursor.begin();
			const ConstObjectIterator iterEnd = cursor.end();
			while (iter != iterEnd)
			{
				iter = objectList_.erase(iter);
			}

			node->setBegin(objectList_.end());
			node->setLast(objectList_.end());
			node->setObjects(0);
		}
		else
		{
			// Recurse deeper.
			
			SplitNode* node = (SplitNode*)cursor.node_;
			node->setEmpty();
			clearObjects(cursor.positive());
			clearObjects(cursor.negative());
		}
	}

	// Private

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::initialize()
	{
		root_ = (Node*)nodeAllocator_.allocate();
		new(root_) LeafNode((SplitNode*)0, objectList_.end(), objectList_.end(), 0);
		++leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::copyConstruct(
		Node* thisSomeNode, Node* thatSomeNode)
	{
		if (!thatSomeNode->leaf())
		{
			SplitNode* thatNode = 
				(SplitNode*)thatSomeNode;

			ASSERT(thisSomeNode->leaf());

			subdivide(
				(LeafNode*)thisSomeNode, 
				thatNode->splitPosition(),
				thatNode->splitAxis(), 
				thatNode->splitDirection(),
				thatNode->min(),
				thatNode->max(),
				thatNode->positiveMin(),
				thatNode->negativeMax());

			SplitNode* thisNode = 
				(SplitNode*)thisSomeNode;
			
			copyConstruct(
				thisNode->negative(),
				thatNode->negative());

			copyConstruct(
				thisNode->positive(),
				thatNode->positive());
		}
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::subdivide(
		LeafNode* node,
		const Real& splitPosition, integer splitAxis,
		const Vector<Real, N>* splitDirection,
		const Real& boundMin, const Real& boundMax,
		const Real& positiveMin, const Real& negativeMax)
	{
		ASSERT2(splitAxis >= 0 && splitAxis < dimension(), splitAxis, dimension());
		ASSERT(node->leaf());

		ConstObjectIterator nodeEnd = node->last();
		if (node->objects() > 0)
		{
			++nodeEnd;
		}

		const integer objects = node->objects();

		// Reorder the objects along the split position.

		const SplitPredicate splitPredicate(
			splitPosition, splitAxis, 
			splitDirection,
			objectPolicy_);

		const std::pair<std::pair<ObjectIterator, integer>,
			std::pair<ObjectIterator, integer> > result =
			partition(objectList_, node->begin(), nodeEnd,
			splitPredicate);

		ConstObjectIterator negativeStart = objectList_.end();
		ConstObjectIterator negativeLast = objectList_.end();

		const integer negativeObjects = result.first.second;
		if (negativeObjects > 0)
		{
			negativeStart = result.first.first;
			negativeLast = result.second.first;
			--negativeLast;
		}

		ConstObjectIterator positiveStart = objectList_.end();
		ConstObjectIterator positiveLast = objectList_.end();

		const integer positiveObjects = result.second.second;
		if (positiveObjects > 0)
		{
			positiveStart = result.second.first;
			positiveLast = nodeEnd;
			--positiveLast;
		}

		// Allocate the new leaf nodes.

		LeafNode* negativeLeaf = (LeafNode*)nodeAllocator_.allocate();
		new(negativeLeaf) LeafNode(
			(SplitNode*)node, negativeStart, negativeLast, negativeObjects);

		{
			ConstObjectIterator iter = negativeStart;
			while(iter != positiveStart)
			{
				iter->setBucket(negativeLeaf);
				++iter;
			}
		}

		LeafNode* positiveLeaf = (LeafNode*)nodeAllocator_.allocate();
		new(positiveLeaf) LeafNode(
			(SplitNode*)node, positiveStart, positiveLast, positiveObjects);

		{
			ConstObjectIterator iter = positiveStart;
			while(iter != nodeEnd)
			{
				iter->setBucket(positiveLeaf);
				++iter;
			}
		}

		// Reuse the memory space of the node to be subdivided.
		// This is ok, because the memory block is of the size
		// of the node class that takes more memory.

		SplitNode* parent = node->parent();

		node->~LeafNode();

		new(node) SplitNode(
			parent,
			positiveLeaf,
			negativeLeaf,
			splitPosition,
			splitAxis,
			splitDirection,
			boundMin,
			boundMax,
			positiveMin,
			negativeMax,
			objects == 0);

		// One leaf node got splitted into two,
		// so it's only one up.
		++leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::updateBound(
		Node* someNode,
		const Point<Real, N>& minBound,
		const Point<Real, N>& maxBound)
	{
		if (!someNode->leaf())
		{
			SplitNode* node = (SplitNode*)someNode;
			const integer splitAxis = node->splitAxis();

			node->setMin(minBound[splitAxis]);
			node->setMax(maxBound[splitAxis]);

			Point<Real, N> negativeMax = maxBound;
			negativeMax[splitAxis] = node->negativeMax();

			updateBound(
				node->negative(),
				minBound,
				negativeMax);

			Point<Real, N> positiveMin = minBound;
			positiveMin[splitAxis] = node->positiveMin();

			updateBound(
				node->positive(),
				positiveMin,
				maxBound);
		}
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::spliceInsert(
		Node* someNode,
		ObjectContainer& list,
		const ObjectIterator& begin, 
		const ObjectIterator& end,
		integer objects)
	{
		ASSERT1(objects >= 0, objects);

		if (objects == 0)
		{
			ASSERT(begin == end);
			return;
		}

		someNode->setNonEmpty();

		if (someNode->leaf())
		{
			// If this is a leaf node, splice the objects
			// to this node.

			LeafNode* node = (LeafNode*)someNode;

			ObjectIterator iter = begin;
			while(iter != end)
			{
				iter->setBucket(node);
				++iter;
			}

			ObjectIterator last = end;
			--last;

			objectList_.splice(node->begin(), list, begin, end);

			node->setBegin(begin);

			if (node->objects() == 0)
			{
				// If there are currently no
				// objects in the node, set the 'last' iterator.
				node->setLast(last);
			}

			// Update the object count.
			node->setObjects(node->objects() + objects);
		}
		else
		{
			// Otherwise carry out a partitioning of the objects.

			SplitNode* node = (SplitNode*)someNode;

			const SplitPredicate splitPredicate(
				node->splitPosition(), node->splitAxis(), 
				node->splitDirection(), objectPolicy_);

			const std::pair<
				std::pair<ObjectIterator, integer>,
				std::pair<ObjectIterator, integer> > result =
				partition(list, begin, end,
				splitPredicate);

			const ObjectIterator positiveBegin = result.second.first;
			const integer positiveObjects = result.second.second;
			const ObjectIterator negativeBegin = result.first.first;
			const integer negativeObjects = result.first.second;

			// Note that it is important to
			// splice the negative objects first, because
			// positiveBegin is part of the positive object range.
			if (negativeObjects > 0)
			{
				// If there are objects going to the negative node,
				// recurse deeper.

				spliceInsert(node->negative(),
					list, negativeBegin, positiveBegin, negativeObjects);
			}
			if (positiveObjects > 0)
			{
				// If there are objects going to the positive node,
				// recurse deeper.

				spliceInsert(node->positive(),
					list, positiveBegin, end, positiveObjects);
			}
		}
	}

	template <int N, typename Real, typename ObjectPolicy>
	template <typename SubdivisionRule>
	void PointKdTree<N, Real, ObjectPolicy>::refine(
		Node* someNode,
		integer maxDepth,
		const SubdivisionRule& subdivisionRule,
		integer depth,
		const Point<Real, N>& minBound,
		const Point<Real, N>& maxBound)
	{
		Real negativeSplitMax = 0;
		Real positiveSplitMin = 0;
		Real splitPosition = 0;
		integer splitAxis = 0;

		if (someNode->leaf())
		{
			LeafNode* node = (LeafNode*)someNode;

			if (depth < maxDepth && node->objects() > bucketSize_)
			{
				Vector<Real, N> splitDirection(
					ofDimension(dimension_));

				Vector<Real, N>* splitDirectionPtr =
					UseArbitrarySplits::value ? &splitDirection : 0;

				const std::pair<Real, integer> result = 
					subdivisionRule(
					*this,
					Cursor(node),
					minBound,
					maxBound,
					splitDirectionPtr);

				splitPosition = result.first;
				splitAxis = result.second;
				negativeSplitMax = splitPosition;
				positiveSplitMin = splitPosition;

				if (UseArbitrarySplits::value)
				{
					integer computedSplitAxis = 0;
					const bool overlapped = intersect(
						AlignedBox<Real, N>(minBound, maxBound),
						Plane<Real, N>(asPoint(splitDirection * splitPosition), 
						splitDirection),
						computedSplitAxis,
						negativeSplitMax,
						positiveSplitMin);
					//ASSERT(overlapped);
					//ASSERT(computedSplitAxis == splitAxis);
					if (!overlapped)
					{
						negativeSplitMax = maxBound[splitAxis];
						positiveSplitMin = negativeSplitMax;
					}
				}

				subdivide(node, splitPosition, splitAxis, 
					splitDirectionPtr,
					minBound[splitAxis], maxBound[splitAxis],
					positiveSplitMin, negativeSplitMax);
			}
		}
		else
		{
			SplitNode* node = 
				(SplitNode*)someNode;

			splitPosition = node->splitPosition();
			splitAxis = node->splitAxis();
			negativeSplitMax = node->negativeMax();
			positiveSplitMin = node->positiveMin();
		}

		// A leaf node might or might not have been turned
		// into an intermediate node.
		if (!someNode->leaf())
		{
			SplitNode* node = 
				(SplitNode*)someNode;

			Point<Real, N> negativeMax(maxBound);
			negativeMax[splitAxis] = negativeSplitMax;

			refine(
				node->negative(), 
				maxDepth, 
				subdivisionRule,
				depth + 1,
				minBound,
				negativeMax);

			Point<Real, N> positiveMin(minBound);
			positiveMin[splitAxis] = positiveSplitMin;

			refine(
				node->positive(), 
				maxDepth, 
				subdivisionRule,
				depth + 1, 
				positiveMin,
				maxBound);
		}
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::updateEmptyBits(LeafNode* leafNode)
	{
		// Propagate information to parents.

		SplitNode* node = leafNode->parent();
		while(node)
		{
			if (node->negative()->empty() &&
				node->positive()->empty())
			{
				if (node->empty())
				{
					break;
				}
				else
				{
					node->setEmpty();
				}
			}
			else
			{
				if (node->empty())
				{
					node->setNonEmpty();
				}
				else
				{
					break;
				}
			}
			node = node->parent();
		}
	}

}

#endif
