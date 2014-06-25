// Description: Range tree

#ifndef PASTELGEOMETRY_RANGETREE_FWD_H
#define PASTELGEOMETRY_RANGETREE_FWD_H

#include "pastel/sys/mytypes.h"

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

		class Entry
		{
		public:
			explicit Entry(const Point_Iterator& point)
			: point_(point)
			, cascadeSet_()
			{
			}

			Point_Iterator point_;
			std::array<integer, 2> cascadeSet_;
		};

		class Node
		{
		public:
			template <typename, template <typename> class>
			friend class RangeTree;

			Node*& child(bool right)
			{
				return childSet_[right];
			}

			Node* child(bool right) const
			{
				return childSet_[right];
			}

		private:
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
				for (auto&& i : iteratorSet)
				{
					entrySet_.emplace_back(i);
				}
				split_ = split;
			}

			Node(const Node&) = delete;
			Node(Node&&) = delete;
			Node& operator=(const Node&) = delete;

			std::vector<Entry> entrySet_;
			std::array<Node*, 2> childSet_;
			Point_Iterator split_;
		};

		using Node_Iterator = Node*;
		using Node_ConstIterator = const Node*;
	};

}

#endif
