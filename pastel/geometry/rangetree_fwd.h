// Description: Range tree

#ifndef PASTELGEOMETRY_RANGETREE_FWD_H
#define PASTELGEOMETRY_RANGETREE_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/range.h"

#include <vector>
#include <array>

namespace Pastel
{

	template <typename, template <typename> class>
	class RangeTree;

	template <typename Settings>
	class RangeTree_Fwd
	{
	public:
		using Fwd = Settings;
		PASTEL_FWD(Point);
		PASTEL_FWD(MultiLess);

		using PointSet = 
			std::vector<Point>;
		using Point_Iterator = 
			typename PointSet::iterator;
		using Point_ConstIterator = 
			typename PointSet::const_iterator;

		class Node;

		class Entry
		{
		public:
			Entry(const Point_Iterator& point)
				: point_(point)
				, cascadeSet_()
			{
				cascadeSet_[0] = 0;
				cascadeSet_[1] = 0;
			}

			Entry& operator=(const Entry&) = delete;

			integer& cascade(bool right)
			{
				return cascadeSet_[right];
			}

			integer cascade(bool right) const
			{
				return cascadeSet_[right];
			}

			Point_ConstIterator point() const
			{
				return point_;
			}

		private:
			template <typename, template <typename> class>
			friend class RangeTree;

			Point_Iterator point_;
			std::array<integer, 2> cascadeSet_;
		};

		using EntrySet = std::vector<Entry>;
		using Entry_Iterator = typename EntrySet::iterator;
		using Entry_ConstIterator = typename EntrySet::const_iterator;
		using Entry_Range = boost::iterator_range<Entry_Iterator>;
		using Entry_ConstRange = boost::iterator_range<Entry_ConstIterator>;

		using Node_Iterator = Node*;
		using Node_ConstIterator = const Node*;

		class Node
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
			friend class RangeTree;

			Node()
			: childSet_()
			, split_()
			, down_(nullptr)
			, entrySet_()
			{
				child(false) = this;
				child(true) = this;
				down_ = this;
			}

			explicit Node(
				const std::vector<Point_Iterator>& iteratorSet)
			: Node()
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
	};

}

#endif
