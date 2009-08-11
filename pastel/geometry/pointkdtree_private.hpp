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
		const ConstObjectIterator& first,
		const ConstObjectIterator& last,
		integer objects)
	{
		// In case of a leaf node, the 'right' node
		// acts as the bucket pointer, and a null pointer
		// in 'left' is used to identify a leaf 
		// node. In creation, the bucket node of a leaf node 
		// is set to itself. 

		Node* node = (Node*)nodeAllocator_.allocate();

		new(node) Node(
			parent,
			node,
			0,
			first,
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
		PointKdTree<Real, N, ObjectPolicy>::findBucketUpwards(
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
		// node.
		//
		// 2) The parent of the node, if it
		// exists, contains greater than 'limitSize'
		// number of points.
		//
		// 3) The node contains less than or equal to 
		// 'limitSize' number of points.

		if (node->empty())
		{
			ASSERT(node->leaf());
			return node;
		}

		const integer limitSize = 
			std::max(bucketSize_, node->objects());

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
		// Since empty leaf nodes are their own bucket
		// nodes, they need not (and should not) be updated.
		//
		// First assume that the subtree has less than
		// or equal to 'bucketSize_' number of objects.
		// Then this task can be carried out by
		// assigning to the bucket node pointers of the 
		// leaf nodes of the objects in the subtree.
		// Because 'bucketSize_' is a constant, this
		// is a constant-time operation and in particular
		// independent of the tree depth.
		//
		// Now assume that the subtree has greater than
		// 'bucketSize_' number of objects.
		// Then by the definition of a bucket node
		// all of the objects are in the same leaf node.
		// This leaf node can be accessed by the 
		// the leaf node of the first object in the
		// subtree. Thus also in this case the operation
		// is constant time.

		if (subtree->objects() > bucketSize_)
		{
			Node* leaf = subtree->first()->leaf().node_;
			leaf->setBucket(bucket);
		}
		else
		{
			ConstObjectIterator iter = subtree->first();
			const ConstObjectIterator iterEnd = subtree->end();
			while(iter != iterEnd)
			{
				Node* leaf = iter->leaf().node_;
				leaf->setBucket(bucket);
				++iter;
			}
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

		Node* left = node->left();
		Node* right = node->right();

		node->setObjects(
			left->objects() + right->objects());

		ConstObjectIterator first = left->first();
		if (first == objectList_.end())
		{
			first = right->first();
		}

		ConstObjectIterator last = right->last();
		if (last == objectList_.end())
		{
			last = left->last();
		}

		node->setFirst(first);
		node->setLast(last);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::updateObjectsUpwards(
		Node* node)
	{
		ASSERT(node);

		node = node->parent();
		while (node)
		{
			updateObjects(node);
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

				node->setBucket(findBucketUpwards(node));
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

		updateObjectsUpwards(node);

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

		Node* right = allocateLeaf(
			node, 
			rightFirst, 
			rightLast, 
			rightObjects);

		// Assign correct leaf information to objects.

		setLeaf(left->first(), left->end(), left);
		setLeaf(right->first(), right->end(), right);

		// Store the current bucket node because
		// it is going to be overwritten.

		Node* bucket = node->bucket();

		// Turn the subdivided node into a split node.

		node->setLeft(left);
		node->setRight(right);
		node->setSplitPosition(splitPosition);
		node->setSplitAxis(splitAxis);
		node->setMin(boundMin);
		node->setMax(boundMax);

		updateObjects(node);

		// Propagate object information upwards.

		updateObjectsUpwards(node);

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
		Node* newBucketNode)
	{
		ASSERT(node);
		ASSERT(newBucketNode);
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

			// Set the bucket node.
			node->setBucket(newBucketNode);
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

			const ObjectIterator newRightFirst = result.second.first;
			const integer newRightObjects = result.second.second;
			ObjectIterator newRightLast = end;
			--newRightLast;

			const ObjectIterator newLeftFirst = result.first.first;
			const integer newLeftObjects = result.first.second;
			ObjectIterator newLeftLast = newRightFirst;
			--newLeftLast;

			Node* left = node->left();
			Node* right = node->right();

			if (!bucketNode && node->isBucket())
			{
				bucketNode = node;
			}

			Node* leftBucket = bucketNode;
			Node* rightBucket = bucketNode;
			Node* newLeftBucket = newBucketNode;
			Node* newRightBucket = newBucketNode;

			const bool oneSided = 
				(newLeftObjects + left->objects() != 0) ^ 
				(newRightObjects + right->objects() != 0);

			// This is a non-empty bucket node.
			// If the addition of these points causes 
			// the bucket threshold to be exceeded and the
			// node is not one-sided...
			if (node->objects() + objects > bucketSize_ && !oneSided)
			{
				// ... then refine the bucket nodes to
				// the subtrees.

				if (bucketNode)
				{
					setBucket(left, left);
					setBucket(right, right);
					leftBucket = left;
					rightBucket = right;
				}

				newLeftBucket = left;
				newRightBucket = right;
			}

			if (newLeftObjects > 0)
			{
				// If there are objects going to the left node,
				// recurse deeper.

				spliceInsert(
					left, 
					newLeftFirst, newLeftLast, 
					newLeftObjects,
					leftBucket,
					newLeftBucket);
			}
			if (newRightObjects > 0)
			{
				// If there are objects going to the right node,
				// recurse deeper.

				spliceInsert(
					right, 
					newRightFirst, newRightLast, 
					newRightObjects,
					rightBucket,
					newRightBucket);
			}

			// Update the object information.

			updateObjects(node);
			
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
