#ifndef PASTEL_POINTKDTREE_PRIVATE_HPP
#define PASTEL_POINTKDTREE_PRIVATE_HPP

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/longestmedian_splitrule_pointkdtree.h"

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
				thatNode->splitAxis());

			copyConstruct(
				thisNode->left(),
				thatNode->left());

			copyConstruct(
				thisNode->right(),
				thatNode->right());

			thisNode->setMin(
				thatNode->min());
			thisNode->setMax(
				thatNode->max());
			updateObjects(thisNode);
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
		// A null pointer in 'left' is used to identify a leaf 
		// node.

		Node* node = (Node*)nodeAllocator_.allocate();

		new(node) Node(
			parent,
			0,
			0,
			first,
			last,
			objects, 
			0, 0);

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
	AlignedBox<Real, N> PointKdTree<Real, N, ObjectPolicy>::computeBound(
		const ConstObjectIterator& begin, 
		const ConstObjectIterator& end) const
	{
		AlignedBox<Real, N> bound(ofDimension(dimension_));

		ConstObjectIterator iter = begin;
		const ConstObjectIterator iterEnd = end;
		while(iter != iterEnd)
		{
			extendToCover(
				Vector<Real, N>(ofDimension(dimension_),
				withAliasing((Real*)objectPolicy_.point(iter->object()))), 
				bound);
			++iter;
		}
		
		return bound;
	}

	template <typename Real, int N, typename ObjectPolicy>
	std::pair<Real, Real> PointKdTree<Real, N, ObjectPolicy>::computeBound(
		const ConstObjectIterator& begin, 
		const ConstObjectIterator& end,
		integer axis) const
	{
		std::pair<Real, Real> bound(
			infinity<Real>(), -infinity<Real>());

		ConstObjectIterator iter = begin;
		const ConstObjectIterator iterEnd = end;
		while(iter != iterEnd)
		{
			const Real position = 
				objectPolicy_.point(iter->object(), axis);
			if (position < bound.first)
			{
				bound.first = position;
			}
			if (position > bound.second)
			{
				bound.second = position;
			}

			++iter;
		}
		
		return bound;
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
		ASSERT(!node->leaf());

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
	void PointKdTree<Real, N, ObjectPolicy>::updateBounds(
		Node* node, const AlignedBox<Real, N>& bound)
	{
		// We only allow to expand the
		// object bound.
		if (node->parent())
		{
			const integer splitAxis = node->parent()->splitAxis();
			node->setMin(std::min(bound.min()[splitAxis], node->min()));
			node->setMax(std::max(bound.max()[splitAxis], node->max()));
		}
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

		// Clear the object ranges in the subtree.
		clearObjects(node);

		// Update hierarchical information.
		updateObjectsUpwards(node);
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
		node->setMin(infinity<Real>());
		node->setMax(-infinity<Real>());

		if (!node->leaf())
		{
			// Recurse deeper.

			clearObjects(node->right());
			clearObjects(node->left());
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::subdivide(
		Node* node,
		const Real& splitPosition, 
		integer splitAxis,
		const Real& prevMin,
		const Real& prevMax)
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

		// Turn the subdivided node into a split node.

		node->setLeft(left);
		node->setRight(right);
		node->setSplitPosition(splitPosition);
		node->setSplitAxis(splitAxis);

		// Create new cuts for the child nodes.

		Real leftMin;
		Real leftMax;
		if (simulateKdTree_)
		{
			leftMin = prevMin;
			leftMax = splitPosition;
		}
		else
		{
			const std::pair<Real, Real> leftBound = 
				computeBound(left->first(), left->end(), splitAxis);
			leftMin = leftBound.first;
			leftMax = leftBound.second;
		}

		left->setMin(leftMin);
		left->setMax(leftMax);
		left->setPrevMin(prevMin);
		left->setPrevMax(prevMax);

		Real rightMin;
		Real rightMax;
		if (simulateKdTree_)
		{
			rightMin = splitPosition;
			rightMax = prevMax;
		}
		else
		{
			const std::pair<Real, Real> rightBound = 
				computeBound(right->first(), right->end(), splitAxis);
			rightMin = rightBound.first;
			rightMax = rightBound.second;
		}

		right->setMin(rightMin);
		right->setMax(rightMax);
		right->setPrevMin(prevMin);
		right->setPrevMax(prevMax);

		// One leaf node got splitted into two,
		// so it's only one up.

		++leaves_;

		// Note: we didn't update hierarchical information
		// nor set the leaf node pointers for objects.
		// This is intentional: these things are done in refine().
		// Doing them here would affect the complexity of
		// recursive subdivision.
	}

	template <typename Real, int N, typename ObjectPolicy>
	template <typename InputIterator>
	typename PointKdTree<Real, N, ObjectPolicy>::ObjectIterator
	PointKdTree<Real, N, ObjectPolicy>::insertPrepare(
		const InputIterator& begin, 
		const InputIterator& end)
	{
		// Copy the objects to the end of objectList_.

		ObjectIterator oldLast = objectList_.end();
		if (!objectList_.empty())
		{
			--oldLast;
		}

		// We can't use the return value of std::copy because
		// it is of type std::back_inserter_iterator.
		std::copy(begin, end, std::back_inserter(objectList_));

		ObjectIterator first;
		if (oldLast == objectList_.end())
		{
			first = objectList_.begin();
		}
		else
		{
			first = oldLast;
			++first;
		}

		return first;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void PointKdTree<Real, N, ObjectPolicy>::insert(
		Node* node,
		const ObjectIterator& first, 
		const ObjectIterator& last,
		integer objects,
		AlignedBox<Real, N>& bound)
	{
		ASSERT(node);
		ASSERT_OP(objects, >, 0);

		const ObjectIterator begin = first;
		ObjectIterator end = last;
		++end;

		if (node->leaf())
		{
			// Compute the bounding box of the inserted points.
			bound = computeBound(begin, end);

			// Set the bounds.
			updateBounds(node, bound);

			// Set the leaf nodes for each object.
			setLeaf(begin, end, node);

			// Splice the objects to this node.
			objectList_.splice(node->end(), objectList_, begin, end);

			// Update the node's object range.
			node->insert(first, last, objects, objectList_.end());
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

			if (newLeftObjects > 0)
			{
				// If there are objects going to the left node,
				// recurse deeper.

				insert(
					left, 
					newLeftFirst, newLeftLast, 
					newLeftObjects,
					bound);
			}

			if (newRightObjects > 0)
			{
				// If there are objects going to the right node,
				// recurse deeper.

				AlignedBox<Real, N> rightBound(ofDimension(dimension_));
				insert(
					right, 
					newRightFirst, newRightLast, 
					newRightObjects,
					rightBound);

				// Compute the combined bounding box of the
				// inserted points in both the left and right child.
				extendToCover(rightBound, bound);
			}

			// Update hierarchical information.
			updateObjects(node);
			updateBounds(node, bound);
			
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
	template <typename SplitRule_PointKdTree>
	void PointKdTree<Real, N, ObjectPolicy>::refine(
		Node* node,
		Vector<Real, N>& minBound,
		Vector<Real, N>& maxBound,
		const SplitRule_PointKdTree& splitRule,
		integer depth,
		integer bucketSize)
	{
		ASSERT(node);

		if (node->leaf())
		{
			if (node->objects() <= bucketSize)
			{
				setLeaf(node->first(), node->end(), node);
			}
			else
			{
				const std::pair<Real, integer> split = 
					splitRule(*this, Cursor(node), 
					minBound, maxBound, depth);
				const Real splitPosition = split.first;
				const integer splitAxis = split.second;

				subdivide(node, splitPosition, splitAxis,
					minBound[splitAxis], maxBound[splitAxis]);
			}
		}

		if (!node->leaf())
		{
			// Refine recursively.

			const integer splitAxis = node->splitAxis();
			
			const Real oldMinBound = minBound[splitAxis];
			const Real oldMaxBound = maxBound[splitAxis];

			minBound[splitAxis] = node->left()->min();
			maxBound[splitAxis] = node->left()->max();

			refine(
				node->left(), 
				minBound,
				maxBound,
				splitRule,
				depth + 1,
				bucketSize);

			minBound[splitAxis] = node->right()->min();
			maxBound[splitAxis] = node->right()->max();

			refine(
				node->right(), 
				minBound,
				maxBound,
				splitRule,
				depth + 1,
				bucketSize);

			minBound[splitAxis] = oldMinBound;
			maxBound[splitAxis] = oldMaxBound;

			// Update object information.
			// The bound information has already
			// been handled top-down.
			updateObjects(node);
		}
	}

}

#endif
