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
		Node_Iterator& child(bool right)
		{
			return childSet_[right];
		}

		//! Returns a child node.
		Node_ConstIterator child(bool right) const
		{
			return childSet_[right];
		}

		//! Returns the down node.
		Node_Iterator& down()
		{
			return down_;
		}

		//! Returns the down node.
		Node_ConstIterator down() const
		{
			return down_;
		}

		//! Returns the split-point.
		const Point_ConstIterator& split() const
		{
			return split_;
		}

		//! Returns whether the node is an end-node.
		bool isEnd() const
		{
			// The end-node is the only node
			// which is the child of itself.
			return child(false) == this;
		}

		//! Returns whether the node is a bottom node.
		bool isBottom() const
		{
			ASSERT(down_);

			// A node is a bottom node if
			// it has no down-link.
			return down_->isEnd();
		}

		//! Returns whether the node is a leaf.
		bool isLeaf() const
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
				entrySet_.end());
		}

		//! Returns the number of stored points.
		integer entries() const
		{
			return entrySet_.size() - 1;
		}

	private:
		template <typename, template <typename> class>
		friend class TdTree;

		Node()
		: childSet_()
		, split_()
		, down_(nullptr)
		, entrySet_()
		{
			child(false) = this;
			child(true) = this;
			down_ = this;
			entrySet_.emplace_back(Point_Iterator());
		}

		explicit Node(
			const std::vector<Point_Iterator>& iteratorSet)
		: childSet_()
		, split_()
		, down_(nullptr)
		, entrySet_()
		{
			entrySet_.reserve(iteratorSet.size() + 1);
			for (auto i : iteratorSet)
			{
				entrySet_.emplace_back(i);
			}
			entrySet_.emplace_back(Point_Iterator());
		}

		void isolate(Node* end)
		{
			down_ = end;
			child(false) = end;
			child(true) = end;
		}

		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;

		std::array<Node_Iterator, 2> childSet_;
		Point_Iterator split_;
		Node_Iterator down_;
		EntrySet entrySet_;
	};
	
}

#endif
