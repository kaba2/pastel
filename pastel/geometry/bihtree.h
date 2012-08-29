// Description: BihTree class
// Detail: Bounding Interval Hierarchy tree

#ifndef PASTELGEOMETRY_BIHTREE_H
#define PASTELGEOMETRY_BIHTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/arena_allocator.h"

#include "pastel/geometry/concept_shapequery.h"

#include <vector>

namespace Pastel
{

	//! A bounding interval hierarchy tree

	/*!
	"Instant ray tracing: The bounding interval hierarchy"
	Carsten Waechter and Alexander Keller, 2006.

	This is an implementation of the bounding interval
	hierarchy tree. It uses a construct-at-once philosophy:
	no incremental building is possible. It is possible to
	traverse through the leaf nodes by using the leafBegin
	and leafEnd functions. The usual traversal over the
	binary tree is handled by cursors. Cursors traverse the
	tree from the top and at each node can choose either
	the negative or the positive child to continue to.
	*/

	template <typename Real, int N, typename Shape>
	class BihTree
	{
	public:
		class Leaf;

	private:
		typedef std::vector<Leaf> LeafContainer;
		typedef std::vector<Shape> ShapeContainer;

	public:
		class ConstCursor;
		typedef typename LeafContainer::const_iterator
			ConstLeafIterator;

		//! Constructs an empty tree.
		BihTree();

		//! Constructs a copy of an existing tree.
		BihTree(const BihTree<Real, N, Shape>& that);

		//! Destructs the tree.
		virtual ~BihTree();

		//! Copies another tree.
		BihTree<Real, N, Shape>& operator=(
			const BihTree<Real, N, Shape>& that);

		//! Swaps two trees.
		void swap(BihTree& that);

		//! Returns true if the BihTree does not contain any shape.
		bool empty() const;

		//! Removes all contained shapes and inner structures.
		void clear();

		//! Returns a cursor to the root of the tree.
		ConstCursor root() const;

		//! Returns the depth of the tree.
		integer depth() const;

		//! Returns the number of leaf nodes.
		integer leaves() const;

		//! Returns the number of non-leaf nodes.
		integer nodes() const;

		//! Returns the maxShapesPerNode limit criterion.
		integer maxShapesPerNode() const;

		//! Returns an iterator to the beginning of the leaf node list.
		ConstLeafIterator leafBegin() const;

		//! Returns an iterator to the end of the leaf node list.
		ConstLeafIterator leafEnd() const;

		//! Constructs the tree given the shapes and parameters.

		/*!
		'minDepth' is the minimum subdivision depth of the tree.
		'maxDepth' is the maximum subdivision depth of the tree.
		'maxShapesPerNode' is the maximum number of shapes that
		does not cause subdivision to occur in a node.
		*/

		template <typename ShapeBoundQuery>
		void construct(
			const std::vector<Shape>& that,
			integer minDepth,
			integer maxDepth,
			integer maxShapesPerNode,
			const ShapeBoundQuery& shapeQuery);

		//! Returns an ALIGNEDBOX that bounds all contained shapes.

		AlignedBox<Real, N> boundingAlignedBox() const;

		//! Given a leaf cursor, returns the associated leaf iterator.

		ConstLeafIterator leaf(
			const ConstCursor& leafNode) const;

		//! Given a leaf iterator, returns the contained shapes.

		ConstMemoryView<Shape> shapes(
			const ConstLeafIterator& leafNode) const;

		//! Returns all of the contained shapes.

		ConstMemoryView<Shape> shapes() const;

	private:
		class Node;

		Node* allocateNodes(integer count);
		integer allocateLeaf(integer shapeIndexFrom,
			integer shapeIndexTo);

		template <typename ShapeBoundQuery>
		void construct(
			Node* node,
			const AlignedBox<Real, N>& bound,
			integer objectIndexFrom,
			integer objectIndexTo,
			integer currentDepth,
			integer minDepth,
			integer maxDepth,
			integer maxShapesPerNode,
			const ShapeBoundQuery& shapeQuery);

		ArenaAllocator nodeAllocator_;
		Node* root_;
		AlignedBox<Real, N> rootBound_;
		ShapeContainer shapeArray_;
		LeafContainer leafArray_;
		integer leavesAllocated_;
		integer depth_;
		integer nodes_;
		integer actualMaxShapesPerNode_;
	};

}

#include "pastel/geometry/bihtree_more.h"

#include "pastel/geometry/bihtree.hpp"

#endif
