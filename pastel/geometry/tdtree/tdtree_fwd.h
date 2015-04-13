// Description: Temporal kd-tree

#ifndef PASTELGEOMETRY_TDTREE_FWD_H
#define PASTELGEOMETRY_TDTREE_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/range.h"
#include "pastel/sys/point/temporal_point.h"

#include <vector>

namespace Pastel
{

	template <typename, template <typename> class>
	class TdTree;

	template <typename Settings>
	class TdTree_Fwd
	{
	public:
		using Fwd = Settings;
		PASTEL_FWD(Locator);

		//! The type of the contained points.
		using Point = typename Locator::Point;

		//! The type of the point-coordinates.
		using Real = typename Locator::Real;

		//! The compile-time dimension of the points.
		static PASTEL_CONSTEXPR integer N = Locator::N;

		//! Storage for the temporal points.
		/*!
		The temporal points are stored in an array at each node.
		The storage is an array, because we need indexing for the
		fractional cascading.
		*/
		using PointSet = 
			std::vector<Temporal_Point<Point, Real>>;
		using Point_Iterator = 
			typename PointSet::iterator;
		using Point_ConstIterator = 
			typename PointSet::const_iterator;

		using Iterator = Point_Iterator;
		using ConstIterator = Point_ConstIterator;

		//! A node.
		class Node;
		class Entry;
		class Cursor;

		using EntrySet = std::vector<Entry>;
		using Entry_Iterator = typename EntrySet::iterator;
		using Entry_ConstIterator = typename EntrySet::const_iterator;
		using Entry_Range = boost::iterator_range<Entry_Iterator>;
		using Entry_ConstRange = boost::iterator_range<Entry_ConstIterator>;
	};

}

namespace Pastel
{

	//! Returns whether Type is an instance of TdTree.
	template <typename Type>
	struct IsTdTree
	: std::false_type
	{};

	template <typename Settings,
		template <typename> class Customization>
	struct IsTdTree<TdTree<Settings, Customization>>
	: std::true_type
	{};

}

#endif
