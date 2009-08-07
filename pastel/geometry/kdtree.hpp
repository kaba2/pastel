#ifndef PASTEL_KDTREE_HPP
#define PASTEL_KDTREE_HPP

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/fastlist_tools.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy>
	class KdTree<Real, N, ObjectPolicy>::Node
	{
	public:
		explicit Node(pointer_integer unknown)
			: unknown_(unknown)
		{
		}

		bool leaf() const
		{
			return ((unknown_ & 1) != 0);
		}

		//protected:

		// The first bit of 'unknown_' tells
		// if the node is a LeafNode (1) or an
		// SplitNode (0).
		// If the node is a LeafNode, then
		// the number of objects is encoded in
		// (unknown_ >> 1).
		// Otherwise unknown_ ^ (unknown_ & 0x3)
		// contains a pointer to the positive child node.
		// This trick relies on 4 byte alignment of nodes.

		pointer_integer unknown_;
	};

	template <typename Real, int N, typename ObjectPolicy>
	class KdTree<Real, N, ObjectPolicy>::LeafNode
		: public Node
	{
	public:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

		LeafNode(
			const ObjectIterator& begin,
			const ObjectIterator& last,
			integer objects)
			: Node((objects << 1) + 1)
			, begin_(begin)
			, last_(last)
		{
		}

		void setBegin(const ObjectIterator& begin)
		{
			begin_ = begin;
		}

		ObjectIterator begin() const
		{
			return begin_;
		}

		void setLast(const ObjectIterator& last)
		{
			last_ = last;
		}

		ObjectIterator last() const
		{
			return last_;
		}

		void setObjects(integer objects)
		{
			unknown_ = (objects << 1) + 1;
		}

		integer objects() const
		{
			return unknown_ >> 1;
		}

	private:
		ObjectIterator begin_;
		ObjectIterator last_;
	};

	template <typename Real, int N, typename ObjectPolicy>
	class KdTree<Real, N, ObjectPolicy>::SplitNode_Low
		: public Node
	{
	private:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

	public:
		SplitNode_Low(
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis)
			: Node(0)
			, negative_(0)
			, splitPosition_(splitPosition)
		{
			unknown_ = encodePositive(positive, splitAxis);
			negative_ = encodeNegative(negative, splitAxis);
		}

		const Real& splitPosition() const
		{
			return splitPosition_;
		}

		integer splitAxis() const
		{
			const pointer_integer high =
				(unknown_ & 0x2) << 1;
			const pointer_integer low =
				(pointer_integer)(negative_) & 0x3;

			return (integer)(high + low);
		}

		Node* positive() const
		{
			return (Node*)(unknown_ ^ (unknown_ & 0x3));
		}

		Node* negative() const
		{
			const pointer_integer nodeId = (pointer_integer)negative_;
			return (Node*)(nodeId ^ (nodeId & 0x3));
		}

	private:
		Node* encodeNegative(Node* node, integer splitAxis)
		{
			pointer_integer nodeId = (pointer_integer)node;
			// We rely on that the pointer is aligned
			// to at least four-byte boundaries.
			ASSERT((nodeId & 0x3) == 0);
			// The two lowest bits of 'splitAxis' are stored
			// at the two lowest bits of 'nodeId'.
			nodeId += (splitAxis & 0x3);

			return (Node*)nodeId;
		}

		pointer_integer encodePositive(Node* node, integer splitAxis)
		{
			pointer_integer nodeId = (pointer_integer)node;
			// We rely on that the pointer is aligned
			// to at least four-byte boundaries.
			ASSERT((nodeId & 0x3) == 0);
			// The third bit of 'splitAxis' is stored
			// at the second bit of 'nodeId'.
			nodeId += (splitAxis & 0x4) >> 1;

			return nodeId;
		}

		/*
		It is assumed that the memory for the
		nodes is aligned by at least 4 bytes.
		Then we can store information to the
		lowest bits of the pointers.

		The 2 lowest bits of 'unknown_' are xy.
		The 2 lowest bits of 'negative_' are wz.

		For an intermediate node, y = 0 thus
		identifying it.
		The splitting plane is given by xwz.
		Thus this packing scheme can only be
		used for dimensions <= 8.
		The pointer to the positive child is
		obtained by zeroing the two lowest bits
		of unknown_.
		Likewise for negative child and negative_.
		*/

		Node* negative_;
		Real splitPosition_;
	};

	template <typename Real, int N, typename ObjectPolicy>
	class KdTree<Real, N, ObjectPolicy>::SplitNode_High
		: public Node
	{
	private:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

	public:
		SplitNode_High(
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis)
			: Node((pointer_integer)positive)
			, negative_(negative)
			, splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
		{
		}

		const Real& splitPosition() const
		{
			return splitPosition_;
		}

		integer splitAxis() const
		{
			return splitAxis_;
		}

		Node* positive() const
		{
			return (Node*)unknown_;
		}

		Node* negative() const
		{
			return negative_;
		}

	private:
		Node* negative_;
		Real splitPosition_;
		int32 splitAxis_;
	};

	template <typename Real, int N, typename ObjectPolicy>
	class KdTree<Real, N, ObjectPolicy>::Cursor
		: boost::less_than_comparable<Cursor
		, boost::equality_comparable<Cursor
		> >
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Cursor()
			: node_(0)
		{
		}

		bool operator<(const Cursor& that) const
		{
			return node_ < that.node_;
		}

		bool operator==(const Cursor& that) const
		{
			return node_ == that.node_;
		}

		bool empty() const
		{
			return node_ == 0;
		}

		ConstObjectIterator begin() const
		{
			PENSURE(node_);
			PENSURE(leaf());

			LeafNode* leafNode = (LeafNode*)node_;

			return leafNode->begin();
		}

		ConstObjectIterator end() const
		{
			PENSURE(node_);
			PENSURE(leaf());

			LeafNode* leafNode = (LeafNode*)node_;

			ObjectIterator iterEnd = leafNode->last();
			if (leafNode->objects() > 0)
			{
				++iterEnd;
			}

			return iterEnd;
		}

		integer objects() const
		{
			PENSURE(node_);
			PENSURE(leaf());

			LeafNode* leafNode = (LeafNode*)node_;

			return leafNode->objects();
		}

		Real splitPosition() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->splitPosition();
		}

		integer splitAxis() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->splitAxis();
		}

		bool leaf() const
		{
			PENSURE(node_);

			return node_->leaf();
		}

		Cursor positive() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return Cursor(((SplitNode*)node_)->positive());
		}

		Cursor negative() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return Cursor(((SplitNode*)node_)->negative());
		}

	private:
		friend class KdTree<Real, N, ObjectPolicy>;

		explicit Cursor(Node* node)
			: node_(node)
		{
		}

		Node* node_;
	};

	template <typename Real, int N, typename ObjectPolicy>
	KdTree<Real, N, ObjectPolicy>::KdTree()
		: objectList_()
		, nodeAllocator_(sizeof(SplitNode), 1024)
		, root_(0)
		, bound_(N)
		, leaves_(0)
		, objects_(0)
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

	template <typename Real, int N, typename ObjectPolicy>
	KdTree<Real, N, ObjectPolicy>::KdTree(
		integer dimension,
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(SplitNode), 1024)
		, root_(0)
		, bound_(dimension)
		, leaves_(0)
		, objects_(0)
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

	template <typename Real, int N, typename ObjectPolicy>
	KdTree<Real, N, ObjectPolicy>::KdTree(const KdTree& that)
		: objectList_()
		, nodeAllocator_(sizeof(SplitNode), 1024)
		, root_(0)
		, bound_(that.dimension_)
		, leaves_(0)
		, objects_(0)
		, objectPolicy_(that.objectPolicy_)
		, dimension_(0)
	{
		// TODO
		ENSURE(false);
	}

	template <typename Real, int N, typename ObjectPolicy>
	KdTree<Real, N, ObjectPolicy>::~KdTree()
	{
		// This is what we assume for memory allocation.
		BOOST_STATIC_ASSERT(sizeof(LeafNode) <= sizeof(SplitNode));
		BOOST_STATIC_ASSERT(N > 0 || N == Dynamic);

		nodeAllocator_.clear();
	}

	template <typename Real, int N, typename ObjectPolicy>
	KdTree<Real, N, ObjectPolicy>&
		KdTree<Real, N, ObjectPolicy>::operator=(
		const KdTree& that)
	{
		KdTree copy(that);
		swap(copy);
		return *this;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void KdTree<Real, N, ObjectPolicy>::swap(
		KdTree& that)
	{
		objectList_.swap(that.objectList_);
		nodeAllocator_.swap(that.nodeAllocator_);
		std::swap(root_, that.root_);
		bound_.swap(that.bound_);
		std::swap(leaves_, that.leaves_);
		std::swap(objects_, that.objects_);
		std::swap(objectPolicy_, that.objectPolicy_);
		std::swap(dimension_, that.dimension_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	const ObjectPolicy& KdTree<Real, N, ObjectPolicy>::objectPolicy() const
	{
		return objectPolicy_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void KdTree<Real, N, ObjectPolicy>::reserveBound(
		const AlignedBox<Real, N>& boxToCover)
	{
		extendToCover(
			boxToCover, bound_);

		//bound_ = boundingAlignedBox(bound_, boxToCover);
	}

	template <typename Real, int N, typename ObjectPolicy>
	const AlignedBox<Real, N>& KdTree<Real, N, ObjectPolicy>::bound() const
	{
		return bound_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	bool KdTree<Real, N, ObjectPolicy>::empty() const
	{
		return objectList_.empty();
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename KdTree<Real, N, ObjectPolicy>::Cursor
		KdTree<Real, N, ObjectPolicy>::root() const
	{
		return Cursor(root_);
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator
		KdTree<Real, N, ObjectPolicy>::begin() const
	{
		return objectList_.begin();
	}

	template <typename Real, int N, typename ObjectPolicy>
	typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator
		KdTree<Real, N, ObjectPolicy>::end() const
	{
		return objectList_.end();
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer KdTree<Real, N, ObjectPolicy>::nodes() const
	{
		return nodeAllocator_.allocated();
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer KdTree<Real, N, ObjectPolicy>::leaves() const
	{
		return leaves_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer KdTree<Real, N, ObjectPolicy>::objects() const
	{
		return objects_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer KdTree<Real, N, ObjectPolicy>::dimension() const
	{
		return dimension_;
	}
	
	template <typename Real, int N, typename ObjectPolicy>
	template <typename SubdivisionRule>
	void KdTree<Real, N, ObjectPolicy>::refine(
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

		refine(maxDepth, maxObjects,
			subdivisionRule, root(),
			0, bound().min(), bound().max());
	}

	template <typename Real, int N, typename ObjectPolicy>
	template <typename InputIterator>
	void KdTree<Real, N, ObjectPolicy>::insert(
		InputIterator begin, InputIterator end)
	{
		insert(root(), begin, end);
	}

	template <typename Real, int N, typename ObjectPolicy>
	void KdTree<Real, N, ObjectPolicy>::clear()
	{
		objectList_.clear();
		nodeAllocator_.clear();
		root_ = 0;
		bound_ = AlignedBox<Real, N>(dimension_);
		leaves_ = 0;
		objects_ = 0;

		root_ = (Node*)nodeAllocator_.allocate();
		new(root_) LeafNode(objectList_.end(), objectList_.end(), 0);
		++leaves_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void KdTree<Real, N, ObjectPolicy>::clearObjects()
	{
		clearObjects(root());

		objectList_.clear();
	}

	// Private

	template <typename Real, int N, typename ObjectPolicy>
	void KdTree<Real, N, ObjectPolicy>::subdivide(
		const Cursor& cursor,
		const Real& splitPosition, integer splitAxis)
	{
		ENSURE2(splitAxis >= 0 && splitAxis < dimension(), splitAxis, dimension());
		ENSURE(cursor.leaf());

		LeafNode* node = (LeafNode*)cursor.node_;

		ObjectIterator nodeEnd = node->last();
		if (node->objects() > 0)
		{
			++nodeEnd;
		}

		// Reorder the objects along the split position.

		SplitPredicate splitPredicate(
			splitPosition, splitAxis, objectPolicy_);

		const std::pair<std::pair<ObjectIterator, integer>,
			std::pair<ObjectIterator, integer> > result =
			fuzzyPartition(objectList_, node->begin(), nodeEnd,
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
		// sizeof(SplitNode) >= sizeof(LeafNode).

		StdExt::destruct(node);

		new(node) SplitNode(
			positiveLeaf,
			negativeLeaf,
			splitPosition,
			splitAxis);

		// One leaf node got splitted into two,
		// so it's only one up.
		++leaves_;
	}

	template <typename Real, int N, typename ObjectPolicy>
	template <typename InputIterator>
	void KdTree<Real, N, ObjectPolicy>::insert(
		const Cursor& cursor,
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

		ObjectIterator iter(list.begin());
		ObjectIterator iterEnd(list.end());
		while(iter != iterEnd)
		{
			reserveBound(objectPolicy_.bound(*iter));
			++objects;
			++iter;
		}

		// Use a combination of splicing and insertion to
		// get the objects to the leaf nodes.

		spliceInsert(cursor, list, list.begin(), list.end(), objects);

		objects_ += objects;
	}

	template <typename Real, int N, typename ObjectPolicy>
	void KdTree<Real, N, ObjectPolicy>::clearObjects(
		const Cursor& cursor)
	{
		if (cursor.leaf())
		{
			// Clear the object references.

			LeafNode* leafNode = (LeafNode*)cursor.node_;
			leafNode->setBegin(objectList_.end());
			leafNode->setLast(objectList_.end());
			leafNode->setObjects(0);
		}
		else
		{
			// Recurse deeper.

			clearObjects(cursor.positive());
			clearObjects(cursor.negative());
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	void KdTree<Real, N, ObjectPolicy>::spliceInsert(
		const Cursor& cursor,
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

		if (cursor.leaf())
		{
			// If this is a leaf node, splice the objects
			// to this node.

			ObjectIterator last = end;
			--last;

			LeafNode* node = (LeafNode*)cursor.node_;
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
			// Otherwise carry out a fuzzy partitioning
			// (those objects that overlap with the splitting
			// plane are placed to both sets).

			SplitPredicate splitPredicate(
				cursor.splitPosition(), cursor.splitAxis(), objectPolicy_);

			std::pair<
				std::pair<ObjectIterator, integer>,
				std::pair<ObjectIterator, integer> > result =
				fuzzyPartition(list, begin, end,
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

				spliceInsert(cursor.negative(),
					list, negativeBegin, positiveBegin, negativeObjects);
			}
			if (positiveObjects > 0)
			{
				// If there are objects going to the positive node,
				// recurse deeper.

				spliceInsert(cursor.positive(),
					list, positiveBegin, end, positiveObjects);
			}
		}
	}

	template <typename Real, int N, typename ObjectPolicy>
	template <typename SubdivisionRule>
	void KdTree<Real, N, ObjectPolicy>::refine(
		integer maxDepth,
		integer maxObjects,
		const SubdivisionRule& subdivisionRule,
		const Cursor& cursor,
		integer depth,
		const Point<Real, N>& minBound,
		const Point<Real, N>& maxBound)
	{
		Real splitPosition = 0;
		integer splitAxis = 0;

		if (cursor.leaf())
		{
			if (depth < maxDepth && cursor.objects() > maxObjects)
			{
				std::pair<Real, integer> result =
					subdivisionRule(
					minBound,
					maxBound,
					objectPolicy(),
					cursor.begin(),
					cursor.end());

				splitPosition = result.first;
				splitAxis = result.second;
				subdivide(cursor, splitPosition, splitAxis);
			}
		}
		else
		{
			splitPosition = cursor.splitPosition();
			splitAxis = cursor.splitAxis();
		}

		// A leaf node might or might not have been turned
		// into an intermediate node.
		if (!cursor.leaf())
		{
			Point<Real, N> negativeMax(maxBound);
			negativeMax[splitAxis] = splitPosition;

			refine(maxDepth, maxObjects, subdivisionRule,
				cursor.negative(), depth + 1,
				minBound,
				negativeMax.asTemporary());

			Point<Real, N> positiveMin(minBound);
			positiveMin[splitAxis] = splitPosition;

			refine(maxDepth, maxObjects, subdivisionRule,
				cursor.positive(), depth + 1, 
				positiveMin.asTemporary(),
				maxBound);
		}
	}

}

#endif
