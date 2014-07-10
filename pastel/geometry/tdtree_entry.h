// Description: Temporal kd-tree entry
// Documentation: tdtree.txt

#ifndef PASTELGEOMETRY_TDTREE_ENTRY_H
#define PASTELGEOMETRY_TDTREE_ENTRY_H

#include <pastel/geometry/rangetree_fwd.h>

#include <array>

namespace Pastel
{

	template <typename Settings>
	class TdTree_Fwd<Settings>::Entry
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
		friend class TdTree;

		Point_Iterator point_;
		std::array<integer, 2> cascadeSet_;
	};


}

#endif
