// Description: Types for the point kd-tree

#ifndef PASTELGEOMETRY_POINTKDTREE_FWD_H
#define PASTELGEOMETRY_POINTKDTREE_FWD_H

#include "pastel/sys/allocator/pool_allocator.h"
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

		PASTEL_FWD(Locator);
		using Point = typename Locator::Point;
		using Real = typename Locator::Real;
		static constexpr integer N = Locator::N;

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
			ranges::subrange<Point_ConstIterator>;

		using PointData_ConstIterator = 
			boost::indirect_iterator<Point_ConstIterator, const Point>;
		using PointData_ConstRange = 
			ranges::subrange<PointData_ConstIterator>;
	};	

}

namespace Pastel
{

	namespace PointKdTree_
	{

		//! Returns whether Type is an instance of PointKdTree.
		template <typename Type>
		struct IsPointKdTree
		: std::false_type
		{};

		template <
			typename Settings,
			template <typename> class Customization>
		struct IsPointKdTree<PointKdTree<Settings, Customization>>
		: std::true_type
		{};

	}

	template <typename Type>
	using IsPointKdTree =
		PointKdTree_::IsPointKdTree<RemoveCvRef<Type>>;

}

#endif
