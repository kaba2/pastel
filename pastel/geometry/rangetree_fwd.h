// Description: Range tree

#ifndef PASTELGEOMETRY_RANGETREE_FWD_H
#define PASTELGEOMETRY_RANGETREE_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/range.h"

#include <vector>

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
		class Entry;

		using EntrySet = std::vector<Entry>;
		using Entry_Iterator = typename EntrySet::iterator;
		using Entry_ConstIterator = typename EntrySet::const_iterator;
		using Entry_Range = boost::iterator_range<Entry_Iterator>;
		using Entry_ConstRange = boost::iterator_range<Entry_ConstIterator>;

		using Node_Iterator = Node*;
		using Node_ConstIterator = const Node*;

	};

}

#endif
