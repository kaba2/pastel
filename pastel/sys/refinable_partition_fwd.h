#ifndef PASTEL_REFINABLE_PARTITION_FWD_H
#define PASTEL_REFINABLE_PARTITION_FWD_H

#include "pastel/sys/object_forwarding.h"

#include <list>

namespace Pastel
{
	
	template <typename ElementData, typename SetData>
	class RefinablePartition_Fwd
	{
	public:
		class Element;
		typedef std::list<Element>
			ElementSet;
		typedef typename ElementSet::iterator
			Element_Iterator;
		typedef typename ElementSet::const_iterator
			Element_ConstIterator;
		typedef typename AsClass<ElementData>::type
			ElementData_Class;

		class Set;
		typedef std::list<Set>
			SetSet;
		typedef typename SetSet::iterator
			Set_Iterator;
		typedef typename SetSet::const_iterator
			Set_ConstIterator;
		typedef typename AsClass<SetData>::type
			SetData_Class;

		typedef std::list<Element_Iterator>
			MemberSet;
		typedef typename MemberSet::iterator
			Member_Iterator;
		typedef typename MemberSet::const_iterator
			Member_ConstIterator;

		typedef std::list<Set_Iterator>
			SplitSet;
		typedef typename SplitSet::iterator
			Split_Iterator;
		typedef typename SplitSet::const_iterator
			Split_ConstIterator;
	};

}

#endif
