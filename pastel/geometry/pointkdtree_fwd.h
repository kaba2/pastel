// Description: Types for the point kd-tree

#ifndef PASTELGEOMETRY_POINTKDTREE_FWD_H
#define PASTELGEOMETRY_POINTKDTREE_FWD_H

#include "pastel/sys/pool_allocator.h"
#include "pastel/sys/list.h"
#include "pastel/sys/range.h"

#include <boost/iterator/indirect_iterator.hpp>

namespace Pastel
{

	template <typename, template <typename> class>
	class PointKdTree;

	template <typename Settings>
	class PointKdTree_Fwd
	{
	public:
		using Fwd = Settings;
		PASTEL_FWD(Real);
		PASTEL_FWD(PointPolicy);
		static PASTEL_CONSTEXPR integer N = Settings::N;

		using Point = typename PointPolicy::Point;
		class Cursor;
		class SplitPredicate;

		using NodeAllocator = PoolAllocator;
		using PointAllocator = PoolAllocator;
		using BoundAllocator = PoolAllocator;

		class Node;

		class PointInfo
		{
		public:
			template <typename, template <typename> class>
			friend class PointKdTree;

			// Implicit conversion allowed.
			PointInfo(
				const Point& point,
				Node* leafNode = 0,
				bool hidden = false)
				: point_(point)
				, leafNode_(leafNode)
				, hidden_(hidden)
			{
			}

			const Point& operator*() const
			{
				return point_;
			}

			const Point& point() const
			{
				return point_;
			}

			Cursor leaf() const
			{
				return Cursor((Node*)leafNode_);
			}

			bool hidden() const
			{
				return hidden_;
			}
		
		private:
			void setLeaf(
				const Node* leafNode) const
			{
				leafNode_ = leafNode;
			}

			void setHidden(bool hidden) const
			{
				hidden_ = hidden;
			}

			Point point_;
			mutable const Node* leafNode_;
			mutable bool hidden_;
		};

		using PointSet = List_Set<PointInfo>;
		using Point_Iterator = typename PointSet::iterator;

		using Point_ConstIterator = 
			typename PointSet::const_iterator;
		using Point_ConstRange = 
			boost::iterator_range<Point_ConstIterator>;

		using PointData_ConstIterator = 
			boost::indirect_iterator<Point_ConstIterator, const Point>;
		using PointData_ConstRange = 
			boost::iterator_range<PointData_ConstIterator>;
	};	

}

#endif
