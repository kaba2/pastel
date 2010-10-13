// Description: Set_RbtPolicy class
// Detail: Plain red-black tree with no value data.
// Documentation: rbtpolicy.txt

#ifndef PASTEL_SET_RBTPOLICY_H
#define PASTEL_SET_RBTPOLICY_H

#include "pastel/sys/rbtpolicy_concept.h"

namespace Pastel
{

	class Set_RbtPolicy
	{
	public:
		typedef EmptyClass ValueType;

		void swap(Set_RbtPolicy& that)
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
