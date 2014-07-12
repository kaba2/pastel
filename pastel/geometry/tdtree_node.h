// Description: Temporal kd-tree node
// Documentation: tdtree.txt

#ifndef PASTELGEOMETRY_TDTREE_NODE_H
#define PASTELGEOMETRY_TDTREE_NODE_H

#include "pastel/geometry/tdtree_fwd.h"

#include <array>

namespace Pastel
{

	//! Temporal kd-tree node
	template <typename Settings>
	class TdTree_Fwd<Settings>::Node
	{
	public:
		//! Returns a child node.
		Node*& child(bool right)
		{
			return childSet_[right];
		}

		//! Returns a child node.
		const Node* child(bool right) const
		{
			return childSet_[right];
		}

		//! Returns the split-point.
		const ConstIterator& split() const
		{
			return split_;
		}

		//! Returns the split-axis.
		integer splitAxis() const
		{
			return splitAxis_;
		}

		//! Returns whether the node is an end-node.
		bool isEnd() const
		{
			// The end-node is the only node
			// which is the child of itself.
			return child(false) == this;
		}

		//! Returns whether the node is a leaf.
		bool leaf() const
		{
			// A node is a leaf if and only if
			// it is not the end-node and its
			// children are equal.
			return !isEnd() &&
				child(false) == child(true);
		}

		//! Returns the stored points.
		Entry_ConstRange entryRange() const
		{
			return Pastel::range(
				entrySet_.begin(), 
				std::prev(entrySet_.end()));
		}

		//! Returns the number of stored points.
		integer points() const
		{
			return entrySet_.size() - 1;
		}

		const Real& min() const
		{
			return min_;
		}

		const Real& max() const
		{
			return max_;
		}

		const Real& prevMin() const
		{
			return prevMin_;
		}

		const Real& prevMax() const
		{
			return prevMax_;
		}

	private:
		template <typename, template <typename> class>
		friend class TdTree;

		Node()
		: childSet_()
		, split_()
		, splitAxis_(0)
		, entrySet_()
		, min_(0)
		, max_(0)
		, prevMin_(0)
		, prevMax_(0)
		{
			child(false) = this;
			child(true) = this;
			entrySet_.emplace_back(Iterator());
		}

		explicit Node(
			const std::vector<Iterator>& iteratorSet)
		: childSet_()
		, split_()
		, splitAxis_(0)
		, entrySet_()
		, min_(0)
		, max_(0)
		, prevMin_(0)
		, prevMax_(0)
		{
			entrySet_.reserve(iteratorSet.size() + 1);
			for (auto&& i : iteratorSet)
			{
				entrySet_.emplace_back(i);
			}
			entrySet_.emplace_back(Iterator());
		}

		void isolate(Node* end)
		{
			child(false) = end;
			child(true) = end;
		}

		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;

		//! The child-nodes.
		/*!
		The first (second) node contains the points that 
		are	< (>=) the split point on the splitting axis.
		*/
		std::array<Node*, 2> childSet_;

		//! The splitting position.
		/*!
		The splitting position is given by 
		locator(split_->point(), splitAxis).
		*/
		Iterator split_;

		//! The splitting axis.
		/*!
		This is the index of the standard basis 
		vector to use for the normal of the
		splitting plane normal.
		*/
		integer splitAxis_;

		//! The set of points.
		EntrySet entrySet_;

		//! Node bounds on the splitting axis of the parent.

		Real min_;
		Real max_;

		//! The previous node bounds on the splitting axis of the parent.
		/*!
		This information is important for the efficient 
		incremental computation of distance between the
		search point and the kd-tree nodes in the nearest 
		neighbor search algorithm.
		*/
		Real prevMin_;
		Real prevMax_;
	};
	
}

#endif
