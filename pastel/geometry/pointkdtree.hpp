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
	PointKdTree<N, Real, ObjectPolicy>::PointKdTree()
		: objectList_()
		, nodeAllocator_(sizeof(IntermediateNode), 1024)
		, root_(0)
		, bound_(N)
		, leaves_(0)
		, objectPolicy_()
		, dimension_(N)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new Allocator(objectList_.get_allocator()->unitSize(), 1024)));
		root_ = (Node*)nodeAllocator_.allocate();
		new(root_) LeafNode(objectList_.end(), objectList_.end(), 0);
		++leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	PointKdTree<N, Real, ObjectPolicy>::PointKdTree(
		integer dimension,
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(IntermediateNode), 1024)
		, root_(0)
		, bound_(dimension)
		, leaves_(0)
		, objectPolicy_(objectPolicy)
		, dimension_(dimension)
	{
		ENSURE2((N != Dynamic && dimension == N) || 
			(N == Dynamic && dimension > 0), dimension, N);

		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new Allocator(objectList_.get_allocator()->unitSize(), 1024)));
		root_ = (Node*)nodeAllocator_.allocate();
		new(root_) LeafNode(objectList_.end(), objectList_.end(), 0);
		++leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	PointKdTree<N, Real, ObjectPolicy>::PointKdTree(const PointKdTree& that)
		: objectList_()
		, nodeAllocator_(sizeof(IntermediateNode), 1024)
		, root_(0)
		, bound_(that.dimension_)
		, leaves_(0)
		, objectPolicy_(that.objectPolicy_)
		, dimension_(that.dimension_)
	{
		objectList_.set_allocator(ObjectContainer::allocator_ptr(
			new Allocator(objectList_.get_allocator()->unitSize(), 1024)));
		root_ = (Node*)nodeAllocator_.allocate();
		new(root_) LeafNode(objectList_.end(), objectList_.end(), 0);
		++leaves_;

		// First copy the structure of the tree.

		copyConstruct(root_, that.root_);

		// Then insert the objects to the nodes.

		insert(that.begin(), that.end());
	}

	template <int N, typename Real, typename ObjectPolicy>
	PointKdTree<N, Real, ObjectPolicy>::~PointKdTree()
	{
		// This is what we assume for memory allocation.
		BOOST_STATIC_ASSERT(sizeof(LeafNode) <= sizeof(IntermediateNode));
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
	}

	template <int N, typename Real, typename ObjectPolicy>
	const ObjectPolicy& PointKdTree<N, Real, ObjectPolicy>::objectPolicy() const
	{
		return objectPolicy_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::reserveBound(
		const AlignedBox<N, Real>& boxToCover)
	{
		if (extendToCover(
			boxToCover, bound_))
		{
			updateBound(root_, bound_.min(), bound_.max());
		}
	}

	template <int N, typename Real, typename ObjectPolicy>
	const AlignedBox<N, Real>& PointKdTree<N, Real, ObjectPolicy>::bound() const
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
	typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		PointKdTree<N, Real, ObjectPolicy>::end() const
	{
		return objectList_.end();
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
	template <typename SubdivisionRule>
	void PointKdTree<N, Real, ObjectPolicy>::refine(
		integer maxDepth,
		integer maxObjects,
		const SubdivisionRule& subdivisionRule)
	{
		ENSURE_OP(maxDepth, >=, 0);
		ENSURE_OP(maxObjects, >, 0);

		if (maxDepth == 0)
		{
			// Nothing to be done.
			return;
		}

		refine(root_, maxDepth, maxObjects,
			subdivisionRule,
			0, bound().min(), bound().max());
	}

	template <int N, typename Real, typename ObjectPolicy>
	template <typename InputIterator>
	void PointKdTree<N, Real, ObjectPolicy>::insert(
		InputIterator begin, InputIterator end)
	{
		if (begin == end)
		{
			// Nothing to do.
			return;
		}

		// Copy objects to a list which shares
		// an allocator with the objectList_.
		ObjectContainer list(begin, end, objectList_.get_allocator());

		// Possibly extend the bounding box.

		integer objects = 0;

		bool neededToExtend = false;
		ObjectIterator iter = list.begin();
		const ObjectIterator iterEnd = list.end();
		while(iter != iterEnd)
		{
			neededToExtend |= extendToCover(
				objectPolicy_.point(*iter), bound_);

			++objects;
			++iter;
		}

		if (neededToExtend)
		{
			updateBound(root_, bound_.min(), bound_.max());
		}

		// Use a combination of splicing and insertion to
		// get the objects to the leaf nodes.

		spliceInsert(root_, list, list.begin(), list.end(), objects);
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::clear()
	{
		objectList_.clear();
		nodeAllocator_.clear();
		root_ = 0;
		bound_ = AlignedBox<N, Real>(dimension_);
		leaves_ = 0;

		root_ = (Node*)nodeAllocator_.allocate();
		new(root_) LeafNode(objectList_.end(), objectList_.end(), 0);
		++leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::clearObjects()
	{
		clearObjects(root_);

		objectList_.clear();
	}

	// Private

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::copyConstruct(
		Node* thisSomeNode, Node* thatSomeNode)
	{
		if (!thatSomeNode->leaf())
		{
			IntermediateNode* thatNode = 
				(IntermediateNode*)thatSomeNode;

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

			IntermediateNode* thisNode = 
				(IntermediateNode*)thisSomeNode;
			
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
		const Vector<N, Real>* splitDirection,
		const Real& boundMin, const Real& boundMax,
		const Real& positiveMin, const Real& negativeMax)
	{
		ASSERT2(splitAxis >= 0 && splitAxis < dimension(), splitAxis, dimension());
		ASSERT(node->leaf());

		ObjectIterator nodeEnd = node->last();
		if (node->objects() > 0)
		{
			++nodeEnd;
		}

		// Reorder the objects along the split position.

		const SplitPredicate splitPredicate(
			splitPosition, splitAxis, 
			splitDirection,
			objectPolicy_);

		const std::pair<std::pair<ObjectIterator, integer>,
			std::pair<ObjectIterator, integer> > result =
			partition(objectList_, node->begin(), nodeEnd,
			splitPredicate);

		ObjectIterator negativeStart = objectList_.end();
		ObjectIterator negativeLast = objectList_.end();

		const integer negativeObjects = result.first.second;
		if (negativeObjects > 0)
		{
			negativeStart = result.first.first;
			negativeLast = result.second.first;
			--negativeLast;
		}

		ObjectIterator positiveStart = objectList_.end();
		ObjectIterator positiveLast = objectList_.end();

		const integer positiveObjects = result.second.second;
		if (positiveObjects > 0)
		{
			positiveStart = result.second.first;
			positiveLast = nodeEnd;
			--positiveLast;
		}

		// Allocate the new leaf nodes.

		LeafNode* negativeLeaf = (LeafNode*)nodeAllocator_.allocate();
		new(negativeLeaf) LeafNode(negativeStart, negativeLast, negativeObjects);

		LeafNode* positiveLeaf = (LeafNode*)nodeAllocator_.allocate();
		new(positiveLeaf) LeafNode(positiveStart, positiveLast, positiveObjects);

		// Reuse the memory space of the node to be subdivided.
		// This is ok, because the memory block is of the size
		// of the node class that takes more memory.

		node->~LeafNode();

		new(node) IntermediateNode(
			positiveLeaf,
			negativeLeaf,
			splitPosition,
			splitAxis,
			splitDirection,
			boundMin,
			boundMax,
			positiveMin,
			negativeMax);

		// One leaf node got splitted into two,
		// so it's only one up.
		++leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::updateBound(
		Node* someNode,
		const Point<N, Real>& minBound,
		const Point<N, Real>& maxBound)
	{
		if (!someNode->leaf())
		{
			IntermediateNode* node = (IntermediateNode*)someNode;
			const integer splitAxis = node->splitAxis();

			node->setMin(minBound[splitAxis]);
			node->setMax(maxBound[splitAxis]);

			Point<N, Real> negativeMax = maxBound;
			negativeMax[splitAxis] = node->negativeMax();

			updateBound(
				node->negative(),
				minBound,
				negativeMax);

			Point<N, Real> positiveMin = minBound;
			positiveMin[splitAxis] = node->positiveMin();

			updateBound(
				node->positive(),
				positiveMin,
				maxBound);
		}
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::clearObjects(
		Node* someNode)
	{
		if (someNode->leaf())
		{
			// Clear the object references.

			LeafNode* node = (LeafNode*)someNode;
			node->setBegin(objectList_.end());
			node->setLast(objectList_.end());
			node->setObjects(0);
		}
		else
		{
			// Recurse deeper.
			
			IntermediateNode* node = (IntermediateNode*)someNode;
			clearObjects(node->positive());
			clearObjects(node->negative());
		}
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::spliceInsert(
		Node* someNode,
		ObjectContainer& list,
		ObjectIterator begin, ObjectIterator end,
		integer objects)
	{
		ASSERT1(objects >= 0, objects);

		if (objects == 0)
		{
			ASSERT(begin == end);
			return;
		}

		if (someNode->leaf())
		{
			// If this is a leaf node, splice the objects
			// to this node.

			LeafNode* node = (LeafNode*)someNode;

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

			IntermediateNode* node = (IntermediateNode*)someNode;

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
		integer maxObjects,
		const SubdivisionRule& subdivisionRule,
		integer depth,
		const Point<N, Real>& minBound,
		const Point<N, Real>& maxBound)
	{
		Real negativeSplitMax = 0;
		Real positiveSplitMin = 0;
		Real splitPosition = 0;
		integer splitAxis = 0;

		if (someNode->leaf())
		{
			LeafNode* node = (LeafNode*)someNode;

			if (depth < maxDepth && node->objects() > maxObjects)
			{
				Vector<N, Real> splitDirection(
					ofDimension(dimension_));

				Vector<N, Real>* splitDirectionPtr =
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
						AlignedBox<N, Real>(minBound, maxBound),
						Plane<N, Real>(asPoint(splitDirection * splitPosition), 
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
			IntermediateNode* node = 
				(IntermediateNode*)someNode;

			splitPosition = node->splitPosition();
			splitAxis = node->splitAxis();
			negativeSplitMax = node->negativeMax();
			positiveSplitMin = node->positiveMin();
		}

		// A leaf node might or might not have been turned
		// into an intermediate node.
		if (!someNode->leaf())
		{
			IntermediateNode* node = 
				(IntermediateNode*)someNode;

			Point<N, Real> negativeMax(maxBound);
			negativeMax[splitAxis] = negativeSplitMax;

			refine(node->negative(), 
				maxDepth, maxObjects, subdivisionRule,
				depth + 1,
				minBound,
				negativeMax);

			Point<N, Real> positiveMin(minBound);
			positiveMin[splitAxis] = positiveSplitMin;

			refine(node->positive(), maxDepth, maxObjects, subdivisionRule,
				depth + 1, 
				positiveMin,
				maxBound);
		}
	}

}

#endif
