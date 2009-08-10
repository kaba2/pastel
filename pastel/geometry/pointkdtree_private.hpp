#ifndef PASTEL_POINTKDTREE_PRIVATE_HPP
#define PASTEL_POINTKDTREE_PRIVATE_HPP

#include "pastel/geometry/pointkdtree.h"

#include <boost/type_traits/has_trivial_destructor.hpp>

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
		// it is correct in case the leaf node is either empty.

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
	void PointKdTree<Real, N, ObjectPolicy>::destructSubtree(
		Node* node)
	{
		if (!boost::has_trivial_destructor<Real>())
		{
			if (!node->leaf())
			{
				destructSubtree(node->left());
				destructSubtree(node->right());
			}

			StdExt::destruct(node);
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename PointKdTree<Real, N, ObjectPolicy>::Node* 
		PointKdTree<Real, N, ObjectPolicy>::findBucket(
		Node* node)
	{
		ASSERT(node);
		
		// Let limitSize = max(bucketSize_, leafNode->objects()).
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
		// exists, contains greater than 'limitSize'
		// number of points.
		//
		// 3) The children of the bucket node, if
		// they exist, contain less than or equal to 
		// 'limitSize' number of points.

		if (node->objects() == 0)
		{
			ASSERT(node->leaf());
			return node;
		}

		const integer limitSize = 
			std::max(bucketSize_, node->objects());

		// Search for the new bucket node upwards.

		Node* bucket = node;
		Node* parent = bucket->parent();
		while(parent)
		{
			if (parent->objects() > limitSize)
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

		ConstObjectIterator iter = subtree->first();
		const ConstObjectIterator iterEnd = subtree->end();
		while(iter != iterEnd)
		{
			Node* leaf = iter->leaf().node_;
			leaf->setBucket(bucket);
			++iter;
		}
	}


	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::setLeaf(
		const ConstObjectIterator& begin,
		const ConstObjectIterator& end,
		Node* node)
	{
		ConstObjectIterator iter = begin;
		while(iter != end)
		{
			iter->setLeaf(node);
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

			ConstObjectIterator begin = left->first();
			if (begin == objectList_.end())
			{
				begin = right->first();
			}

			ConstObjectIterator last = right->last();
			if (last == objectList_.end())
			{
				last = left->last();
			}

			node->setFirst(begin);
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
			if (node == root_)
			{
				merge();
			}
			else
			{
				erase(node->left());
				erase(node->right());

				node->setLeft(0);
				node->setRight(0);

				// The node is now a leaf node.

				node->setBucket(findBucket(node));
			}
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

		if (!boost::has_trivial_destructor<Real>())
		{
			StdExt::destruct(node);
		}

		nodeAllocator_.deallocate(node);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::eraseObjects(
		Node* node)
	{
		ASSERT(node);

		// Actually remove the objects.

		ConstObjectIterator iter = node->first();
		const ConstObjectIterator iterEnd = node->end();
		while(iter != iterEnd)
		{
			iter = objectList_.erase(iter);
		}

		// Update the state for this node.

		node->setFirst(objectList_.end());
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

		node->setFirst(objectList_.end());
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

		const integer objects = node->objects();
		const ConstObjectIterator nodeEnd = node->end();

		// Reorder the objects along the split position.

		const SplitPredicate splitPredicate(
			splitPosition, splitAxis, 
			objectPolicy_);

		const std::pair<std::pair<ObjectIterator, integer>,
			std::pair<ObjectIterator, integer> > result =
			partition(objectList_, node->first(), nodeEnd,
			splitPredicate);

		ConstObjectIterator leftFirst = objectList_.end();
		ConstObjectIterator leftLast = objectList_.end();

		const integer leftObjects = result.first.second;
		if (leftObjects > 0)
		{
			leftFirst = result.first.first;
			leftLast = result.second.first;
			--leftLast;
		}

		ConstObjectIterator rightFirst = objectList_.end();
		ConstObjectIterator rightLast = objectList_.end();

		const integer rightObjects = result.second.second;
		if (rightObjects > 0)
		{
			rightFirst = result.second.first;
			rightLast = nodeEnd;
			--rightLast;
		}

		// Allocate the new leaf nodes.

		Node* left = allocateLeaf(
			node,
			leftFirst, 
			leftLast, 
			leftObjects);

		// Assign correct leaf information to objects.
		setLeaf(left->first(), left->end(), left);

		Node* right = allocateLeaf(
			node, 
			rightFirst, 
			rightLast, 
			rightObjects);

		// Assign correct leaf information to objects.
		setLeaf(right->first(), right->end(), right);

		Node* bucket = node->bucket();

		// Convert the subdivided leaf node into
		// a split node by adding details.

		if (leftFirst != objectList_.end())
		{
			node->setFirst(leftFirst);
		}
		else
		{
			node->setFirst(rightFirst);
		}

		if (rightLast != objectList_.end())
		{
			node->setLast(rightLast);
		}
		else
		{
			node->setLast(leftLast);
		}

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

		const bool oneSided = 
			(leftObjects != 0) ^ (rightObjects != 0);

		if (bucket->objects() <= bucketSize_ || oneSided)
		{
			if (leftObjects > 0)
			{
				left->setBucket(bucket);
			}
			if (rightObjects > 0)
			{
				right->setBucket(bucket);
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
		const ObjectIterator& first, 
		const ObjectIterator& last,
		integer objects,
		Node* bucketNode,
		Node* pseudoBucketNode)
	{
		ASSERT(node);
		ASSERT_OP(objects, >, 0);

		const ObjectIterator begin = first;
		ObjectIterator end = last;
		++end;

		if (node->leaf())
		{
			// Set the leaf nodes for each object.
			setLeaf(begin, end, node);

			// Splice the objects to this node.
			objectList_.splice(node->first(), objectList_, begin, end);

			// Update the node's object range.
			node->insert(begin, last, objects, objectList_.end());

			// Propagate object information up the tree.
			updateObjects(node);

			node->setBucket(pseudoBucketNode);
		}
		else
		{
			// This is a split node, so carry out a 
			// partitioning of the objects.

			const SplitPredicate splitPredicate(
				node->splitPosition(), node->splitAxis(), 
				objectPolicy_);

			const std::pair<
				std::pair<ObjectIterator, integer>,
				std::pair<ObjectIterator, integer> > result =
				partition(objectList_, begin, end,
				splitPredicate);

			const ObjectIterator rightFirst = result.second.first;
			const integer rightObjects = result.second.second;
			ObjectIterator rightLast = end;
			--rightLast;

			const ObjectIterator leftFirst = result.first.first;
			const integer leftObjects = result.first.second;
			ObjectIterator leftLast = rightFirst;
			--leftLast;

			Node* left = node->left();
			Node* right = node->right();

			if (!bucketNode && node->nonEmptyBucket())
			{
				bucketNode = node;
			}

			Node* leftBucket = bucketNode;
			Node* rightBucket = bucketNode;
			Node* leftPseudoBucket = pseudoBucketNode;
			Node* rightPseudoBucket = pseudoBucketNode;

			const bool oneSided = 
				(leftObjects + left->objects() != 0) ^ 
				(rightObjects + right->objects() != 0);

			// This is a non-empty bucket node.
			// If the addition of these points causes 
			// the bucket threshold to be exceeded and the
			// node is not one-sided...
			if (node->objects() + objects > bucketSize_ && !oneSided)
			{
				// ... then refine the bucket nodes to
				// the subtrees.

				if (bucketNode == node)
				{
					setBucket(left, left);
					setBucket(right, right);
					leftBucket = left;
					rightBucket = right;
				}

				leftPseudoBucket = left;
				rightPseudoBucket = right;
			}

			if (leftObjects > 0)
			{
				// If there are objects going to the left node,
				// recurse deeper.

				spliceInsert(
					left, 
					leftFirst, leftLast, 
					leftObjects,
					leftBucket,
					leftPseudoBucket);
			}
			if (rightObjects > 0)
			{
				// If there are objects going to the right node,
				// recurse deeper.

				spliceInsert(
					right, 
					rightFirst, rightLast, 
					rightObjects,
					rightBucket,
					rightPseudoBucket);
			}

			// Finally, we need to order the objects in the objectList_
			// so that the objects in the right child come right after
			// those in the left child. This is needed to represent
			// the object sets in each node by a simple iterator range.
			// We take care of this ordering by splicing.

			objectList_.splice(
				left->end(),
				objectList_,
				right->first(),
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
