#ifndef PASTEL_POINTKDTREE_HPP
#define PASTEL_POINTKDTREE_HPP

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/intersect_alignedbox_plane.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/fastlist_tools.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::Node
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
	class PointKdTree<N, Real, ObjectPolicy>::LeafNode
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
	class PointKdTree<N, Real, ObjectPolicy>::IntermediateNode_BspTree
		: public Node
	{
	private:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

	public:
		IntermediateNode_BspTree(
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis,
			const Vector<N, Real>* splitDirection,
			const Real& min,
			const Real& max,
			const Real& positiveMin,
			const Real& negativeMax)
			: Node((pointer_integer)positive)
			, negative_(negative)
			, splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
			, splitDirection_(*splitDirection)
			, min_(min)
			, max_(max)
			, positiveMin_(positiveMin)
			, negativeMax_(negativeMax)
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

		const Real& min() const
		{
			return min_;
		}

		const Real& max() const
		{
			return max_;
		}

		Real positiveMin() const
		{
			return positiveMin_;
		}

		Real negativeMax() const
		{
			return negativeMax_;
		}

		const Vector<N, Real>* splitDirection() const
		{
			return &splitDirection_;
		}

		Real projectedPosition(
			const Point<N, Real>& point) const
		{
			return dot(asVector(point), splitDirection_);
		}

	private:
		Node* negative_;
		Real splitPosition_;
		int32 splitAxis_;
		Real min_;
		Real max_;
		Real positiveMin_;
		Real negativeMax_;
		Vector<N, Real> splitDirection_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::IntermediateNode_KdTree
		: public Node
	{
	private:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

	public:
		IntermediateNode_KdTree(
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis,
			const Vector<N, Real>* splitDirection,
			const Real& min,
			const Real& max,
			const Real& positiveMin,
			const Real& negativeMax)
			: Node((pointer_integer)positive)
			, negative_(negative)
			, splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
			, min_(min)
			, max_(max)
		{
			ASSERT(positiveMin == splitPosition);
			ASSERT(negativeMax == splitPosition);
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

		Real min() const
		{
			return min_;
		}

		Real max() const
		{
			return max_;
		}

		Real positiveMin() const
		{
			return splitPosition_;
		}

		Real negativeMax() const
		{
			return splitPosition_;
		}

		const Vector<N, Real>* splitDirection() const
		{
			return 0;
		}

		Real projectedPosition(
			const Point<N, Real>& point) const
		{
			return point[splitAxis_];
		}

	private:
		Node* negative_;
		Real splitPosition_;
		int32 splitAxis_;
		Real min_;
		Real max_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::Cursor
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

		Real positiveMin() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((IntermediateNode*)node_)->positiveMin();
		}

		Real negativeMax() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((IntermediateNode*)node_)->negativeMax();
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

		const Vector<N, Real>* splitDirection() const
		{
			PENSURE(node_);
			PENSURE(!leaf());
			
			return ((IntermediateNode*)node_)->splitDirection();
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

		Real projectedPosition(
			const Point<N, Real>& point) const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((IntermediateNode*)node_)->projectedPosition(point);
		}

	private:
		friend class PointKdTree<N, Real, ObjectPolicy>;

		explicit Cursor(Node* node)
			: node_(node)
		{
		}

		Node* node_;
	};

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
		, dimension_(0)
	{
		// TODO
		ENSURE(false);
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
		extendToCover(
			boxToCover, bound_);

		//bound_ = boundingAlignedBox(bound_, boxToCover);
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
	template <typename InputIterator>
	void PointKdTree<N, Real, ObjectPolicy>::insert(
		InputIterator begin, InputIterator end)
	{
		insert(root(), begin, end);
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::erase(
		const Object& object)
	{
		// FIX: TODO
		ENSURE(false);
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
		clearObjects(root());

		objectList_.clear();
	}

	// Private

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::subdivide(
		const Cursor& cursor,
		const Real& splitPosition, integer splitAxis,
		const Vector<N, Real>* splitDirection,
		const Real& boundMin, const Real& boundMax,
		const Real& positiveMin, const Real& negativeMax)
	{
		ASSERT2(splitAxis >= 0 && splitAxis < dimension(), splitAxis, dimension());
		ASSERT(cursor.leaf());

		LeafNode* node = (LeafNode*)cursor.node_;

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
	template <typename InputIterator>
	void PointKdTree<N, Real, ObjectPolicy>::insert(
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
			extendToCover(
				objectPolicy_.point(*iter), bound_);

			++objects;
			++iter;
		}

		// Use a combination of splicing and insertion to
		// get the objects to the leaf nodes.

		spliceInsert(cursor, list, list.begin(), list.end(), objects);
	}

	template <int N, typename Real, typename ObjectPolicy>
	void PointKdTree<N, Real, ObjectPolicy>::clearObjects(
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
	void PointKdTree<N, Real, ObjectPolicy>::spliceInsert(
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
			// Otherwise carry out a partitioning of the objects.

			const SplitPredicate splitPredicate(
				cursor.splitPosition(), cursor.splitAxis(), 
				cursor.splitDirection(), objectPolicy_);

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
	void PointKdTree<N, Real, ObjectPolicy>::refine(
		integer maxDepth,
		integer maxObjects,
		const SubdivisionRule& subdivisionRule,
		const Cursor& cursor,
		integer depth,
		const Point<N, Real>& minBound,
		const Point<N, Real>& maxBound)
	{
		Real negativeSplitMax = 0;
		Real positiveSplitMin = 0;
		Real splitPosition = 0;
		integer splitAxis = 0;

		if (cursor.leaf())
		{
			if (depth < maxDepth && cursor.objects() > maxObjects)
			{
				Vector<N, Real> splitDirection(ofDimension(
					UseArbitrarySplits::value ? dimension_ : 0));

				Vector<N, Real>* splitDirectionPtr =
					UseArbitrarySplits::value ? &splitDirection : 0;

				const std::pair<Real, integer> result = 
					subdivisionRule(
					minBound,
					maxBound,
					objectPolicy(),
					cursor.begin(),
					cursor.end(),
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

				subdivide(cursor, splitPosition, splitAxis, 
					splitDirectionPtr,
					minBound[splitAxis], maxBound[splitAxis],
					positiveSplitMin, negativeSplitMax);
			}
		}
		else
		{
			splitPosition = cursor.splitPosition();
			splitAxis = cursor.splitAxis();
			negativeSplitMax = cursor.negativeMax();
			positiveSplitMin = cursor.positiveMin();
		}

		// A leaf node might or might not have been turned
		// into an intermediate node.
		if (!cursor.leaf())
		{
			Point<N, Real> negativeMax(maxBound);
			negativeMax[splitAxis] = negativeSplitMax;

			refine(maxDepth, maxObjects, subdivisionRule,
				cursor.negative(), depth + 1,
				minBound,
				negativeMax);

			Point<N, Real> positiveMin(minBound);
			positiveMin[splitAxis] = positiveSplitMin;

			refine(maxDepth, maxObjects, subdivisionRule,
				cursor.positive(), depth + 1, 
				positiveMin,
				maxBound);
		}
	}

}

#endif
