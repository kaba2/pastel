// Description: Map_RbtPolicy class
// Detail: Stores value data in each node.
// Documentation: rbtpolicy.txt

#ifndef PASTEL_MAP_RBTPOLICY_H
#define PASTEL_MAP_RBTPOLICY_H

#include "pastel/sys/rbtpolicy_concept.h"

namespace Pastel
{

	template <typename Value>
	class Map_RbtPolicy
	{
	public:
		typedef Value ValueType;

		void swap(Map_RbtPolicy& that)
		{
		}

		template <typename Iterator>
		void updateInsertDown(const Iterator& iter)
		{
		}

		template <typename Iterator>
		void updateHierarchical(const Iterator& iter)
		{
		}
	};

}

#endif
