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
			Node_Iterator& child(bool right)
			{
				return childSet_[right];
			}

			Node_ConstIterator child(bool right) const
			{
				return childSet_[right];
			}

			const Point_ConstIterator& split() const
			{
				return split_;
			}

			Entry_ConstRange entryRange() const
			{
				return Pastel::range(entrySet_.begin(), entrySet_.end());
			}

			integer entries() const
			{
				return entrySet_.size();
			}

			bool isEnd() const
			{
				return child(false) == this;
			}

		private:
			template <typename, template <typename> class>
			friend class RangeTree;

			Node()
			: entrySet_()
			, childSet_()
			, split_()
			{
				child(false) = this;
				child(true) = this;
			}

			Node(
				const Point_Iterator& split,
				const std::vector<Point_Iterator>& iteratorSet)
			: Node()
			{
				for (auto i : iteratorSet)
				{
					entrySet_.emplace_back(i);
				}
				split_ = split;
			}

			Node(const Node&) = delete;
			Node(Node&&) = delete;
			Node& operator=(const Node&) = delete;

			EntrySet entrySet_;
			std::array<Node_Iterator, 2> childSet_;
			Point_Iterator split_;
		};
	};

}

#endif
