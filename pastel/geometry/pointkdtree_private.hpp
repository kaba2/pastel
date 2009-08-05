#ifndef PASTEL_POINTKDTREE_PRIVATE_HPP
#define PASTEL_POINTKDTREE_PRIVATE_HPP

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	// Private

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::initialize()
	{
		root_ = allocateLeaf(
			0,
			objectList_.end(), 
			objectList_.end(),
			0);

		++leaves_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::copyConstruct(
		Node* thisNode, Node* thatNode)
	{
		ASSERT(thisNode);
		ASSERT(thatNode);

		if (!thatNode->leaf())
		{
			ASSERT(thisNode->leaf());

			subdivide(
				thisNode, 
				thatNode->splitPosition(),
				thatNode->splitAxis(), 
				thatNode->min(),
				thatNode->max());

			copyConstruct(
				thisNode->left(),
				thatNode->left());

			copyConstruct(
				thisNode->right(),
				thatNode->right());
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::Node*
		PointKdTree<Real, N, ObjectPolicy>::allocateLeaf(
		Node* parent,
		const ConstObjectIterator& begin,
		const ConstObjectIterator& last,
		integer objects)
	{
		// In case of a leaf node, the 'right' node
		// acts as the bucket pointer, and the 'left' node
		// being null pointer can be used to identify a leaf 
		// node. In creation, the bucket node of a leaf node 
		// is set to itself. This is a good default because 
		// it is correct in case the leaf node is either empty
		// or contains more points than 'bucketSize_'.

		Node* node = (Node*)nodeAllocator_.allocate();

		new(node) Node(
			parent,
			node,
			0,
			begin,
			last,
			objects, 
			0, 0, 0, 0);

		return node;
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::Node* 
		PointKdTree<Real, N, ObjectPolicy>::findBucket(
		Node* node)
	{
		ASSERT(node);

		// A bucket node of a leaf node is a node
		// defined as follows. The bucket node of 
		// an empty leaf node is the node itself.
		// If the leaf node is not empty, its
		// bucket node is a node such that:
		//
		// 1) The leaf node is in the subtree of the
		// bucket node.
		//
		// 2) The parent of the bucket node, if it
		// exists, contains more than 'bucketSize_'
		// number of points.
		//
		// 3) The children of the bucket node, if
		// they exist, contain less than or equal to 
		// 'bucketSize_' number of points.

		if (node->leaf() && node->objects() == 0)
		{
			return node;
		}

		// Search for the new bucket node upwards.

		Node* bucket = node;
		Node* parent = bucket->parent();
		while(parent)
		{
			if (parent->objects() > bucketSize_)
			{
				break;
			}
			
			bucket = parent;
			parent = parent->parent();
		}

		return bucket;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::setBucket(
		Node* subtree, Node* bucket)
	{
		ASSERT(subtree);
		ASSERT(bucket);

		// The problem is to set all the bucket nodes
		// of empty leaf nodes under a subtree.
		//
		// While this could be done recursively,
		// it is much faster to assign to the bucket nodes
		// of the leaf nodes of the points in the subtree.
		// This is because:
		//
		// 1) The empty leaf nodes need not be visited, since
		// they are their own bucket nodes.
		//
		// 2) The link from the point to the leaf node is
		// constant time.
		//
		// 3) By the bucket node invariants, there are
		// at most 'bucketSize_' number of points in 'subtree'.
		// In contrast, 'subtree' can be the root and the
		// tree have arbitrarily many leaf nodes.

		ConstObjectIterator iter = subtree->begin();
		const ConstObjectIterator iterEnd = subtree->end();
		while(iter != iterEnd)
		{
			Node* leaf = iter->leaf().node_;
			leaf->setBucket(bucket);
			++iter;
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::updateObjects(
		Node* node)
	{
		ASSERT(node);

		node = node->parent();
		while (node)
		{
			Node* left = node->left();
			Node* right = node->right();

			node->setObjects(
				left->objects() + right->objects());

			ConstObjectIterator begin = left->begin();
			if (begin == objectList_.end())
			{
				begin = right->begin();
			}

			ConstObjectIterator last = right->last();
			if (last == objectList_.end())
			{
				last = left->last();
			}

			node->setBegin(begin);
			node->setLast(last);

			node = node->parent();
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::updateObjectRanges(
		Node* node)
	{
		ASSERT(node);

		node = node->parent();
		while (node)
		{
			Node* left = node->left();
			Node* right = node->right();

			ConstObjectIterator begin = left->begin();
			if (begin == objectList_.end())
			{
				begin = right->begin();
			}

			ConstObjectIterator last = right->last();
			if (last == objectList_.end())
			{
				last = left->last();
			}

			if (node->begin() == begin &&
				node->last() == last)
			{
				break;
			}

			node->setBegin(begin);
			node->setLast(last);

			node = node->parent();
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::merge(
		Node* node)
	{
		ASSERT(node);

		if (!node->leaf())
		{
			erase(node->left());
			erase(node->right());

			node->setLeft(0);
			node->setRight(0);

			// The node is now a leaf node.

			node->setBucket(findBucket(node));
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::erase(
		Node* node)
	{
		ASSERT(node);

		if (!node->leaf())
		{
			erase(node->left());
			erase(node->right());
		}
		else
		{
			--leaves_;
		}

		// No need to run destructors for nodes.
		nodeAllocator_.deallocate(node);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::eraseObjects(
		Node* node)
	{
		ASSERT(node);

		// Actually remove the objects.

		ConstObjectIterator iter = node->begin();
		const ConstObjectIterator iterEnd = node->end();
		while(iter != iterEnd)
		{
			iter = objectList_.erase(iter);
		}

		// Update the state for this node.

		node->setBegin(objectList_.end());
		node->setLast(objectList_.end());
		node->setObjects(0);

		// updateObjects() propagates the updated 
		// state _upwards_ in the tree.

		updateObjects(node);

		// Clear the object ranges in the subtree.

		clearObjects(node);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::clearObjects(
		Node* node)
	{
		ASSERT(node);

		// Update the node state.

		node->setBegin(objectList_.end());
		node->setLast(objectList_.end());
		node->setObjects(0);

		if (node->leaf())
		{
			// Empty leaf nodes are their
			// own bucket nodes.
			node->setBucket(node);
		}
		else
		{
			// Recurse deeper.

			clearObjects(node->right());
			clearObjects(node->left());
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::subdivide(
		Node* node,
		const Real& splitPosition, integer splitAxis,
		const Real& boundMin, const Real& boundMax)
	{
		ASSERT(node);
		ASSERT(node->leaf());
		ASSERT2(splitAxis >= 0 && splitAxis < dimension(), splitAxis, dimension());

		ConstObjectIterator nodeEnd = node->end();

		const integer objects = node->objects();

		// Reorder the objects along the split position.

		const SplitPredicate splitPredicate(
			splitPosition, splitAxis, 
			objectPolicy_);

		const std::pair<std::pair<ObjectIterator, integer>,
			std::pair<ObjectIterator, integer> > result =
			partition(objectList_, node->begin(), nodeEnd,
			splitPredicate);

		ConstObjectIterator leftStart = objectList_.end();
		ConstObjectIterator leftLast = objectList_.end();

		const integer leftObjects = result.first.second;
		if (leftObjects > 0)
		{
			leftStart = result.first.first;
			leftLast = result.second.first;
			--leftLast;
		}

		ConstObjectIterator rightStart = objectList_.end();
		ConstObjectIterator rightLast = objectList_.end();

		const integer rightObjects = result.second.second;
		if (rightObjects > 0)
		{
			rightStart = result.second.first;
			rightLast = nodeEnd;
			--rightLast;
		}

		// Allocate the new leaf nodes.

		Node* left = allocateLeaf(
			node,
			leftStart, 
			leftLast, 
			leftObjects);

		// Assign correct leaf information to objects.
		{
			ConstObjectIterator iter = leftStart;
			while(iter != rightStart)
			{
				iter->setLeaf(left);
				++iter;
			}
		}

		Node* right = allocateLeaf(
			node, 
			rightStart, 
			rightLast, 
			rightObjects);

		// Assign correct leaf information to objects.
		{
			ConstObjectIterator iter = rightStart;
			while(iter != nodeEnd)
			{
				iter->setLeaf(right);
				++iter;
			}
		}

		Node* parentBucket = node->bucket();

		// Convert the subdivided leaf node into
		// a split node by adding details.

		node->setRight(right);
		node->setLeft(left);
		node->setSplitPosition(splitPosition);
		node->setSplitAxis(splitAxis);
		node->setMin(boundMin);
		node->setMax(boundMax);

		// One leaf node got splitted into two,
		// so it's only one up.

		++leaves_;

		// Possibly update bucket nodes.
		// Empty leaf nodes should have their
		// bucket node point to themselves,
		// which is set by default in construction.
		
		if (parentBucket->objects() <= bucketSize_)
		{
			if (left->objects() > 0)
			{
				left->setBucket(parentBucket);
			}
			if (right->objects() > 0)
			{
				right->setBucket(parentBucket);
			}
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::updateBound(
		Node* node,
		const Point<Real, N>& minBound,
		const Point<Real, N>& maxBound)
	{
		ASSERT(node);

		if (!node->leaf())
		{
			// Set the node bounds on the splitting axis
			// to the correct values, as traced by
			// minBound and maxBound.

			const integer splitAxis = node->splitAxis();
			const Real& splitPosition = node->splitPosition();

			node->setMin(minBound[splitAxis]);
			node->setMax(maxBound[splitAxis]);

			// Update the correct bounds and recurse.

			Point<Real, N> leftMax = maxBound;
			leftMax[splitAxis] = splitPosition;

			updateBound(
				node->left(),
				minBound,
				leftMax);

			Point<Real, N> rightMin = minBound;
			rightMin[splitAxis] = splitPosition;

			updateBound(
				node->right(),
				rightMin,
				maxBound);
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::spliceInsert(
		Node* node,
		ObjectContainer& list,
		const ObjectIterator& begin, 
		const ObjectIterator& end,
		integer objects,
		Node* bucketNode)
	{
		ASSERT(node);
		ASSERT1(objects >= 0, objects);

		if (objects == 0)
		{
			ASSERT(begin == end);
			return;
		}

		// Update the object count on the way down.
		node->setObjects(node->objects() + objects);

		if (node->leaf())
		{
			// Set the leaf nodes for each point.

			ObjectIterator iter = begin;
			while(iter != end)
			{
				iter->setLeaf(node);
				++iter;
			}

			// Splice the objects to this node.

			ObjectIterator last = end;
			--last;

			objectList_.splice(node->begin(), list, begin, end);

			node->setBegin(begin);

			if (node->last() == objectList_.end())
			{
				// If there are currently no
				// objects in the node, set the 'last' iterator.
				node->setLast(last);
			}

			// Propagate object ranges up in the tree.

			updateObjectRanges(node);
		}
		else
		{
			Node* left = node->left();
			Node* right = node->right();

			if (!bucketNode && node->objects() <= bucketSize_)
			{
				// This node is a bucket node for its subtree.
				bucketNode = node;				
			}

			Node* leftBucket = bucketNode;
			Node* rightBucket = bucketNode;

			// If we have already met the bucket node for this
			// subtree, and if the addition of these points causes 
			// the bucket threshold to be exceeded...
			if (bucketNode && node->objects() + objects > bucketSize_)
			{
				// ... then refine the bucket nodes for both
				// subtrees.

				setBucket(left, left);
				leftBucket = left;

				setBucket(right, right);
				rightBucket = right;
			}

			// This is a split node, so carry out a 
			// partitioning of the objects.

			const SplitPredicate splitPredicate(
				node->splitPosition(), node->splitAxis(), 
				objectPolicy_);

			const std::pair<
				std::pair<ObjectIterator, integer>,
				std::pair<ObjectIterator, integer> > result =
				partition(list, begin, end,
				splitPredicate);

			const ObjectIterator rightBegin = result.second.first;
			const integer rightObjects = result.second.second;
			const ObjectIterator leftBegin = result.first.first;
			const integer leftObjects = result.first.second;

			// Note that it is important to
			// splice the left objects first, because
			// rightBegin is part of the right object range.
			if (leftObjects > 0)
			{
				// If there are objects going to the left node,
				// recurse deeper.

				spliceInsert(
					left, list, 
					leftBegin, rightBegin, 
					leftObjects,
					leftBucket);
			}
			if (rightObjects > 0)
			{
				// If there are objects going to the right node,
				// recurse deeper.

				spliceInsert(
					right, list, 
					rightBegin, end, 
					rightObjects,
					rightBucket);
			}

			// Finally, we need to order the objects in the objectList_
			// so that the objects in the right child come right after
			// those in the left child. This is needed to represent
			// the object sets in each node by a simple iterator range.
			// We take care of this ordering by splicing.

			objectList_.splice(
				left->end(),
				objectList_,
				right->begin(),
				right->end());
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	template <typename SubdivisionRule>
	void PointKdTree<Real, N, ObjectPolicy>::refine(
		Node* node,
		integer maxDepth,
		const SubdivisionRule& subdivisionRule,
		integer depth,
		const Point<Real, N>& minBound,
		const Point<Real, N>& maxBound)
	{
		ASSERT(node);
		ASSERT_OP(depth, >=, 0);
		ASSERT_OP(maxDepth, >=, 0);

		Real splitPosition = 0;
		integer splitAxis = 0;

		if (node->leaf())
		{
			// There are two conditions for termination:
			// 1) The 'depth' gets greater than 'maxDepth'.
			// 2) The number of objects is less than 'bucketSize_'.

			if (depth < maxDepth && node->objects() > bucketSize_)
			{
				// If those conditions do not apply, 
				// apply the splitting rule.

				const std::pair<Real, integer> result = 
					subdivisionRule(
					*this,
					Cursor(node),
					minBound,
					maxBound);

				splitPosition = result.first;
				splitAxis = result.second;

				subdivide(node, splitPosition, splitAxis, 
					minBound[splitAxis], maxBound[splitAxis]);
			}
		}
		else
		{
			splitPosition = node->splitPosition();
			splitAxis = node->splitAxis();
		}

		// A leaf node might or might not have been turned
		// into an intermediate node.
		if (!node->leaf())
		{
			Point<Real, N> leftMax(maxBound);
			leftMax[splitAxis] = splitPosition;

			refine(
				node->left(), 
				maxDepth, 
				subdivisionRule,
				depth + 1,
				minBound,
				leftMax);

			Point<Real, N> rightMin(minBound);
			rightMin[splitAxis] = splitPosition;

			refine(
				node->right(), 
				maxDepth, 
				subdivisionRule,
				depth + 1, 
				rightMin,
				maxBound);
		}
	}

}

#endif
