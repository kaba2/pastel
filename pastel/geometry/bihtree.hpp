#ifndef PASTEL_BIHTREE_HPP
#define PASTEL_BIHTREE_HPP

#include "pastel/geometry/bihtree.h"

#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"
#include "pastel/geometry/alignedplane.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/countedptr.h"

namespace Pastel
{

	// BihTree

	template <typename Real, int N, typename Shape>
	BihTree<Real, N, Shape>::BihTree()
		: nodeAllocator_()
		, root_(0)
		, rootBound_()
		, shapeArray_()
		, leafArray_()
		, leavesAllocated_(0)
		, depth_(0)
		, nodes_(0)
		, actualMaxShapesPerNode_(0)
	{
	}

	template <typename Real, int N, typename Shape>
	BihTree<Real, N, Shape>::BihTree(
		const BihTree<Real, N, Shape>& that)
		: nodeAllocator_()
		, root_(0)
		, rootBound_()
		, shapeArray_()
		, leafArray_()
		, leavesAllocated_(0)
		, depth_(0)
		, nodes_(0)
		, actualMaxShapesPerNode_(0)
	{
		// TODO
		ASSERT(false);
	}

	template <typename Real, int N, typename Shape>
	BihTree<Real, N, Shape>::~BihTree()
	{
		clear();
	}

	template <typename Real, int N, typename Shape>
	BihTree<Real, N, Shape>& BihTree<Real, N, Shape>::operator=(
		const BihTree<Real, N, Shape>& that)
	{
		BihTree copy(that);
		swap(copy);
		return *this;
	}

	template <typename Real, int N, typename Shape>
	void BihTree<Real, N, Shape>::swap(BihTree<Real, N, Shape>& that)
	{
		using std::swap;
		using std::swap;

		nodeAllocator_.swap(that.nodeAllocator_);
		swap(root_, that.root_);
		swap(rootBound_, that.rootBound_);
		shapeArray_.swap(that.shapeArray_);
		leafArray_.swap(that.leafArray_);
		swap(leavesAllocated_, that.leavesAllocated_);
		swap(depth_, that.depth_);
		swap(nodes_, that.nodes_);
		swap(actualMaxShapesPerNode_, that.actualMaxShapesPerNode_);
	}

	template <typename Real, int N, typename Shape>
	bool BihTree<Real, N, Shape>::empty() const
	{
		return (root_ == 0);
	}

	template <typename Real, int N, typename Shape>
	void BihTree<Real, N, Shape>::clear()
	{
		nodeAllocator_.clear();
		root_ = 0;
		rootBound_.set(Vector<Real, N>(0),
			Vector<Real, N>(0));
		shapeArray_.clear();
		leafArray_.clear();
		leavesAllocated_ = 0;
		depth_ = 0;
		nodes_ = 0;
		actualMaxShapesPerNode_ = 0;
	}

	template <typename Real, int N, typename Shape>
	typename BihTree<Real, N, Shape>::ConstCursor
		BihTree<Real, N, Shape>::root() const
	{
		return ConstCursor(root_);
	}

	template <typename Real, int N, typename Shape>
	integer BihTree<Real, N, Shape>::depth() const
	{
		return depth_;
	}

	template <typename Real, int N, typename Shape>
	integer BihTree<Real, N, Shape>::leaves() const
	{
		return leafArray_.size();
	}

	template <typename Real, int N, typename Shape>
	integer BihTree<Real, N, Shape>::nodes() const
	{
		return nodes_;
	}

	template <typename Real, int N, typename Shape>
	integer BihTree<Real, N, Shape>::maxShapesPerNode() const
	{
		return actualMaxShapesPerNode_;
	}

	template <typename Real, int N, typename Shape>
	typename BihTree<Real, N, Shape>::ConstLeafIterator
		BihTree<Real, N, Shape>::leafBegin() const
	{
		return leafArray_.begin();
	}

	template <typename Real, int N, typename Shape>
	typename BihTree<Real, N, Shape>::ConstLeafIterator
		BihTree<Real, N, Shape>::leafEnd() const
	{
		return leafArray_.end();
	}

	template <typename Real, int N, typename Shape>
	template <typename ShapeBoundQuery>
	void BihTree<Real, N, Shape>::construct(
		const std::vector<Shape>& shapes,
		integer minDepth,
		integer maxDepth,
		integer maxShapesPerNode,
		const ShapeBoundQuery& shapeQuery)
	{
		ENSURE_OP(minDepth, >=, 0);
		ENSURE_OP(maxDepth, >=, 0);
		ENSURE_OP(minDepth, <=, maxDepth);
		ENSURE_OP(maxShapesPerNode, >, 0);

		clear();

		if (shapes.empty())
		{
			return;
		}

		shapeArray_ = shapes;

		// Find out a bounding aligned box for the shapes

		AlignedBox<Real, N> treeBound(
			shapeQuery.boundingAlignedBox(shapes[0]));

		for (integer i = 1;i < shapes.size();++i)
		{
			treeBound = Pastel::boundingAlignedBox(treeBound,
				shapeQuery.boundingAlignedBox(shapes[i]));
		}

		// Allocate root node..

		root_ = allocateNodes(1);
		rootBound_ = treeBound;

		// ..And start constructing it.

		construct(
			root_,
			treeBound,
			0,
			shapeArray_.size(),
			0,
			minDepth,
			maxDepth,
			maxShapesPerNode,
			shapeQuery);
	}

	template <typename Real, int N, typename Shape>
	AlignedBox<Real, N> BihTree<Real, N, Shape>::boundingAlignedBox() const
	{
		return rootBound_;
	}

	template <typename Real, int N, typename Shape>
	typename BihTree<Real, N, Shape>::ConstLeafIterator
		BihTree<Real, N, Shape>::leaf(
		const ConstCursor& leafNode) const
	{
		if (!leafNode.isLeaf())
		{
			return leafArray_.end();
		}

		integer leafIndex = leafNode.splitAxis();

		ASSERT2(leafIndex >= 0 && leafIndex < leafArray_.size(),
			leafIndex, leafArray_.size());

		return leafArray_.begin() + leafIndex;
	}

	template <typename Real, int N, typename Shape>
	ConstMemoryView<Shape> BihTree<Real, N, Shape>::shapes(
		const ConstLeafIterator& leaf) const
	{
		ConstMemoryView<Shape> shapes;

		if (leaf != leafArray_.end() &&
			leaf->shapeIndex() < shapeArray_.size())
		{
			shapes.set(
				&shapeArray_[leaf->shapeIndex()],
				leaf->shapes());
		}

		return shapes;
	}

	template <typename Real, int N, typename Shape>
	ConstMemoryView<Shape>
		BihTree<Real, N, Shape>::shapes() const
	{
		return ConstMemoryView<Shape>(
			&shapeArray_[0], shapeArray_.size());
	}

	// Private

	template <typename Real, int N, typename Shape>
	typename BihTree<Real, N, Shape>::Node*
		BihTree<Real, N, Shape>::allocateNodes(integer count)
	{
		return (Node*)nodeAllocator_.allocate(sizeof(Node) * count);
	}

	template <typename Real, int N, typename Shape>
	integer
		BihTree<Real, N, Shape>::allocateLeaf(
		integer shapeIndexFrom,
		integer shapeIndexTo)
	{
		integer leafIndex = leavesAllocated_;
		leafArray_.push_back(Leaf(shapeIndexFrom, shapeIndexTo));
		++leavesAllocated_;
		return leafIndex;
	}

	template <typename Real, int N, typename Shape>
	template <typename ShapeBoundQuery>
	void BihTree<Real, N, Shape>::construct(
		Node* node,
		const AlignedBox<Real, N>& nodeBound,
		integer shapeIndexFrom,
		integer shapeIndexTo,
		integer currentDepth,
		integer minDepth,
		integer maxDepth,
		integer maxShapesPerNode,
		const ShapeBoundQuery& shapeQuery)
	{
		++nodes_;

		using std::swap;
		using std::swap;

		ASSERT2(shapeIndexFrom <= shapeIndexTo,
			shapeIndexFrom, shapeIndexTo);
		ASSERT1(minDepth >= 0, minDepth);
		ASSERT1(maxDepth >= 0, maxDepth);
		ASSERT2(minDepth < maxDepth, minDepth,
			maxDepth);

		ASSERT(node);
		ASSERT1(currentDepth >= 0, currentDepth);
		ASSERT2(currentDepth <= maxDepth, currentDepth,
			maxDepth);
		ASSERT1(maxShapesPerNode > 0, maxShapesPerNode);

		const integer shapes = shapeIndexTo - shapeIndexFrom;

		if (currentDepth < maxDepth &&
			shapes > 0 &&
			(shapes > maxShapesPerNode ||
			currentDepth < minDepth))
		{
			// Create an intermediate node by
			// subdividing space into two.

			// By assuming that the child nodes
			// are stored one after another,
			// we enable a space optimization
			// in Node storage.

			Node* children = allocateNodes(2);

			Node* negativeChild = children;
			Node* positiveChild = children + 1;

			// Find out the largest dimension of
			// the bounding aligned box.
			const Vector<Real, N> width(
				nodeBound.max() - nodeBound.min());

			integer largestDimension = 0;
			Real largestWidth(-infinity<Real>());
			for (integer i = 0;i < N;++i)
			{
				ASSERT(width[i] >= 0);
				if (width[i] > largestWidth)
				{
					largestDimension = i;
					largestWidth = width[i];
				}
			}

			const integer splitAxis = largestDimension;

			// Calculate the premilinary split position

			const Real splitPosition(
				0.5 * nodeBound.min()[splitAxis] +
				0.5 * nodeBound.max()[splitAxis]);

			// Sort the shapes w.r.t to this plane
			// to the side it is most on.
			// This is like a one application
			// of quicksort with the difference
			// that there is no explicit pivot point:
			// the pivot will be known after
			// this, and the ordering becomes
			// from the geometric positions of the
			// shapes w.r.t to the splitting plane.

			Real negativeSplit(nodeBound.min()[splitAxis]);
			Real positiveSplit(nodeBound.max()[splitAxis]);
			integer storeIndex = shapeIndexFrom;

			for (integer i = shapeIndexFrom;
				i < shapeIndexTo;++i)
			{
				AlignedBox<Real, N> alignedBox(
					shapeQuery.boundingAlignedBox(
					shapeArray_[i]));
				Real pivot(alignedBox.min()[splitAxis]);

				if (pivot < splitPosition)
				{
					// The shape belongs to the negative
					// side of the plane.

					swap(shapeArray_[i], shapeArray_[storeIndex]);
					++storeIndex;

					// Update the split position of
					// the negative child.

					if (alignedBox.max()[splitAxis] > negativeSplit)
					{
						negativeSplit = alignedBox.max()[splitAxis];
					}
				}
				else
				{
					// Update the split position of
					// the positive child.

					if (alignedBox.min()[splitAxis] < positiveSplit)
					{
						positiveSplit = alignedBox.min()[splitAxis];
					}
				}
			}

			if (storeIndex == shapeIndexFrom)
			{
				negativeSplit = splitPosition;
			}

			if (storeIndex == shapeIndexTo)
			{
				positiveSplit = splitPosition;
			}

			const integer positiveIndexFrom = storeIndex;

			new(node) Node(
				children,
				splitAxis,
				negativeSplit,
				positiveSplit);

			// Recurse deeper to the negative child.

			AlignedBox<Real, N> negativeChildBound(nodeBound);
			Vector<Real, N> maxTmp(negativeChildBound.max());
			maxTmp[splitAxis] = negativeSplit;
			negativeChildBound.setMax(maxTmp);

			construct(
				negativeChild,
				negativeChildBound,
				shapeIndexFrom,
				positiveIndexFrom,
				currentDepth + 1,
				minDepth,
				maxDepth,
				maxShapesPerNode,
				shapeQuery);

			// Recurse deeper to the positive child.

			AlignedBox<Real, N> positiveChildBound(nodeBound);
			Vector<Real, N> minTmp(positiveChildBound.min());
			minTmp[splitAxis] = positiveSplit;
			positiveChildBound.setMin(minTmp);

			construct(
				positiveChild,
				positiveChildBound,
				positiveIndexFrom,
				shapeIndexTo,
				currentDepth + 1,
				minDepth,
				maxDepth,
				maxShapesPerNode,
				shapeQuery);
		}
		else
		{
			// Initialize this node as a leaf node.

			if (currentDepth > depth_)
			{
				depth_ = currentDepth;
			}

			if (shapes > actualMaxShapesPerNode_)
			{
				actualMaxShapesPerNode_ = shapes;
			}

			const integer newLeafIndex =
				allocateLeaf(
				shapeIndexFrom,
				shapes);

			new(node) Node
				(0,
				newLeafIndex,
				Real(0),
				Real(0));
		}
	}

}

#endif
