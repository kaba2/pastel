#ifndef PASTELGEOMETRY_KDTREE_HPP
#define PASTELGEOMETRY_KDTREE_HPP

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/fastlist_tools.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	class KdTree<N, Real, ObjectPolicy>::Node
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
		// IntermediateNode (0).
		// If the node is a LeafNode, then
		// the number of objects is encoded in
		// (unknown_ >> 1).
		// Otherwise unknown_ ^ (unknown_ & 0x3)
		// contains a pointer to the positive child node.
		// This trick relies on 4 byte alignment of nodes.

		pointer_integer unknown_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class KdTree<N, Real, ObjectPolicy>::LeafNode
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

	template <int N, typename Real, typename ObjectPolicy>
	class KdTree<N, Real, ObjectPolicy>::Bounds
		: public Node
	{
	public:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

		Bounds(
			pointer_integer unknown,
			const Real& min,
			const Real& max)
			: Node(unknown)
			, min_(min)
			, max_(max)
		{
		}

		const Real& min() const
		{
			return min_;
		}

		const Real& max() const
		{
			return max_;
		}

	private:
		Real min_;
		Real max_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class KdTree<N, Real, ObjectPolicy>::Bounds_None
		: public Node
	{
	public:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

		Bounds_None(
			pointer_integer unknown,
			const Real& min,
			const Real& max)
			: Node(unknown)
		{
		}

		Real min() const
		{
			ENSURE(false);
			return nan<Real>();
		}

		Real max() const
		{
			ENSURE(false);
			return nan<Real>();
		}
	};

	template <int N, typename Real, typename ObjectPolicy>
	class KdTree<N, Real, ObjectPolicy>::IntermediateNode_Low
		: public IntermediateBase
	{
	private:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

	public:
		IntermediateNode_Low(
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis,
			const Real& min,
			const Real& max)
			: IntermediateBase(0, min, max)
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

	template <int N, typename Real, typename ObjectPolicy>
	class KdTree<N, Real, ObjectPolicy>::IntermediateNode_High
		: public IntermediateBase
	{
	private:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

	public:
		IntermediateNode_High(
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis,
			const Real& min,
			const Real& max)
			: IntermediateBase((pointer_integer)positive, min, max)
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

	template <int N, typename Real, typename ObjectPolicy>
	class KdTree<N, Real, ObjectPolicy>::Cursor
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

		Real min() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((IntermediateNode*)node_)->min();
		}

		Real max() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((IntermediateNode*)node_)->max();
		}

		Real splitPosition() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((IntermediateNode*)node_)->splitPosition();
		}

		integer splitAxis() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((IntermediateNode*)node_)->splitAxis();
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

			return Cursor(((IntermediateNode*)node_)->positive());
		}

		Cursor negative() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return Cursor(((IntermediateNode*)node_)->negative());
		}

	private:
		friend class KdTree<N, Real, ObjectPolicy>;

		explicit Cursor(Node* node)
			: node_(node)
		{
		}

		Node* node_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	KdTree<N, Real, ObjectPolicy>::KdTree()
		: objectList_()
		, nodeAllocator_(sizeof(IntermediateNode), 1024)
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

	template <int N, typename Real, typename ObjectPolicy>
	KdTree<N, Real, ObjectPolicy>::KdTree(
		integer dimension,
		const ObjectPolicy& objectPolicy)
		: objectList_()
		, nodeAllocator_(sizeof(IntermediateNode), 1024)
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

	template <int N, typename Real, typename ObjectPolicy>
	KdTree<N, Real, ObjectPolicy>::KdTree(const KdTree& that)
		: objectList_()
		, nodeAllocator_(sizeof(IntermediateNode), 1024)
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

	template <int N, typename Real, typename ObjectPolicy>
	KdTree<N, Real, ObjectPolicy>::~KdTree()
	{
		// This is what we assume for memory allocation.
		BOOST_STATIC_ASSERT(sizeof(LeafNode) <= sizeof(IntermediateNode));
		BOOST_STATIC_ASSERT(N > 0 || N == Dynamic);

		nodeAllocator_.clear();
	}

	template <int N, typename Real, typename ObjectPolicy>
	KdTree<N, Real, ObjectPolicy>&
		KdTree<N, Real, ObjectPolicy>::operator=(
		const KdTree& that)
	{
		KdTree copy(that);
		swap(copy);
		return *this;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::swap(
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

	template <int N, typename Real, typename ObjectPolicy>
	const ObjectPolicy& KdTree<N, Real, ObjectPolicy>::objectPolicy() const
	{
		return objectPolicy_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::reserveBound(
		const AlignedBox<N, Real>& boxToCover)
	{
		extendToCover(
			boxToCover, bound_);

		//bound_ = boundingAlignedBox(bound_, boxToCover);
	}

	template <int N, typename Real, typename ObjectPolicy>
	const AlignedBox<N, Real>& KdTree<N, Real, ObjectPolicy>::bound() const
	{
		return bound_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	bool KdTree<N, Real, ObjectPolicy>::empty() const
	{
		return objectList_.empty();
	}

	template <int N, typename Real, typename ObjectPolicy>
	typename KdTree<N, Real, ObjectPolicy>::Cursor
		KdTree<N, Real, ObjectPolicy>::root() const
	{
		return Cursor(root_);
	}

	template <int N, typename Real, typename ObjectPolicy>
	typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		KdTree<N, Real, ObjectPolicy>::begin() const
	{
		return objectList_.begin();
	}

	template <int N, typename Real, typename ObjectPolicy>
	typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		KdTree<N, Real, ObjectPolicy>::end() const
	{
		return objectList_.end();
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer KdTree<N, Real, ObjectPolicy>::nodes() const
	{
		return nodeAllocator_.allocated();
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer KdTree<N, Real, ObjectPolicy>::leaves() const
	{
		return leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer KdTree<N, Real, ObjectPolicy>::objects() const
	{
		return objects_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer KdTree<N, Real, ObjectPolicy>::dimension() const
	{
		return dimension_;
	}
	
	template <int N, typename Real, typename ObjectPolicy>
	template <typename SubdivisionRule>
	void KdTree<N, Real, ObjectPolicy>::refine(
		integer maxDepth,
		integer maxObjects,
		const SubdivisionRule& subdivisionRule)
	{
		ENSURE1(maxDepth >= 0, maxDepth);
		ENSURE1(maxObjects > 0, maxObjects);

		if (maxDepth == 0)
		{
			// Nothing to be done.
			return;
		}

		refine(maxDepth, maxObjects,
			subdivisionRule, root(),
			0, bound().min(), bound().max());
	}

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::subdivide(
		const Cursor& cursor,
		const Real& splitPosition, 
		integer splitAxis)
	{
		ENSURE1(splitAxis >= 0, splitAxis);
		ENSURE2(splitAxis < dimension_, splitAxis, dimension_);

		subdivide(cursor, splitPosition, splitAxis,
			0, 0);
	}

	template <int N, typename Real, typename ObjectPolicy>
	template <typename InputIterator>
	void KdTree<N, Real, ObjectPolicy>::insert(
		InputIterator begin, InputIterator end)
	{
		insert(root(), begin, end);
	}

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::erase(
		const Object& object)
	{
		// FIX: TODO
		ENSURE(false);
	}

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::clear()
	{
		objectList_.clear();
		nodeAllocator_.clear();
		root_ = 0;
		bound_ = AlignedBox<N, Real>(dimension_);
		leaves_ = 0;
		objects_ = 0;

		root_ = (Node*)nodeAllocator_.allocate();
		new(root_) LeafNode(objectList_.end(), objectList_.end(), 0);
		++leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::clearObjects()
	{
		clearObjects(root());

		objectList_.clear();
	}

	// Private

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::subdivide(
		const Cursor& cursor,
		const Real& splitPosition, integer splitAxis,
		const Real& boundMin, const Real& boundMax)
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
		// sizeof(IntermediateNode) >= sizeof(LeafNode).

		node->~LeafNode();

		new(node) IntermediateNode(
			positiveLeaf,
			negativeLeaf,
			splitPosition,
			splitAxis,
			boundMin,
			boundMax);

		// One leaf node got splitted into two,
		// so it's only one up.
		++leaves_;
	}

	template <int N, typename Real, typename ObjectPolicy>
	template <typename InputIterator>
	void KdTree<N, Real, ObjectPolicy>::insert(
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

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::clearObjects(
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

	template <int N, typename Real, typename ObjectPolicy>
	void KdTree<N, Real, ObjectPolicy>::spliceInsert(
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

	template <int N, typename Real, typename ObjectPolicy>
	template <typename SubdivisionRule>
	void KdTree<N, Real, ObjectPolicy>::refine(
		integer maxDepth,
		integer maxObjects,
		const SubdivisionRule& subdivisionRule,
		const Cursor& cursor,
		integer depth,
		const Point<N, Real>& minBound,
		const Point<N, Real>& maxBound)
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
				subdivide(cursor, splitPosition, splitAxis, 
					minBound[splitAxis], maxBound[splitAxis]);
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
			Point<N, Real> negativeMax(maxBound);
			negativeMax[splitAxis] = splitPosition;

			refine(maxDepth, maxObjects, subdivisionRule,
				cursor.negative(), depth + 1,
				minBound,
				negativeMax.asTemporary());

			Point<N, Real> positiveMin(minBound);
			positiveMin[splitAxis] = splitPosition;

			refine(maxDepth, maxObjects, subdivisionRule,
				cursor.positive(), depth + 1, 
				positiveMin.asTemporary(),
				maxBound);
		}
	}

}

#endif
