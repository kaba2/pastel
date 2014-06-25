// Description: Range tree

#ifndef PASTELGEOMETRY_RANGETREE_FWD_H
#define PASTELGEOMETRY_RANGETREE_FWD_H

#include "pastel/sys/mytypes.h"

#include <vector>
#include <array>

namespace Pastel
{

	template <typename Settings>
	class RangeTree_Fwd
	{
	public:
		using Fwd = Settings;
		PASTEL_FWD(Point);
		PASTEL_FWD(Real);

		using PointSet = 
			std::vector<Point>;
		using Point_Iterator = 
			typename PointSet::iterator;
		using Point_ConstIterator = 
			typename PointSet::const_iterator;

		class Entry
		{
		public:
			Point_Iterator element_;
			std::array<integer, 2> cascadeSet_;
		};

		class Node
		{
		public:
			Node()
			: entrySet_()
			, childSet_()
			, split_(0)
			{
				child(false) = this;
				child(true) = this;
			}

			Node*& child(bool right)
			{
				return childSet_[right];
			}

			Node* child(bool right) const
			{
				return childSet_[right];
			}

			std::vector<Entry> entrySet_;
			std::array<Node*, 2> childSet_;
			Real split_;
		};
	};

}

#endif
