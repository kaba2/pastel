// Description: Temporal kd-tree

#ifndef PASTELGEOMETRY_TDTREE_FWD_H
#define PASTELGEOMETRY_TDTREE_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/range.h"

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
		using Point = typename Locator::Point;
		using Real = typename Locator::Real;
		static PASTEL_CONSTEXPR integer N = Locator::N;

		class Point_Entry
		{
		public:
			Point_Entry(
				const Point& point,
				const Real& time)
			: point_(point)
			, time_(time)
			{
			}

			const Point& point() const
			{
				return point_;
			}

			const Real& time() const
			{
				return time_;
			}

		private:
			Point point_;
			Real time_;
		};

		using PointSet = 
			std::vector<Point_Entry>;
		using Point_Iterator = 
			typename PointSet::iterator;
		using Point_ConstIterator = 
			typename PointSet::const_iterator;

		using Iterator = Point_Iterator;
		using ConstIterator = Point_ConstIterator;

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
