#ifndef PASTELSYS_REFINABLE_PARTITION_FWD_H
#define PASTELSYS_REFINABLE_PARTITION_FWD_H

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
	#ifdef __GNUC__
		typedef typename ElementSet::iterator
			Element_ConstIterator;
	#else
		typedef typename ElementSet::const_iterator
			Element_ConstIterator;
	#endif
		typedef Class<ElementData>
			ElementData_Class;

		class Set;
		typedef std::list<Set>
			SetSet;
		typedef typename SetSet::iterator
			Set_Iterator;
	#ifdef __GNUC__
		typedef typename SetSet::iterator
			Set_ConstIterator;
	#else
		typedef typename SetSet::const_iterator
			Set_ConstIterator;
	#endif
		typedef Class<SetData>
			SetData_Class;

		typedef std::list<Element_Iterator>
			MemberSet;
		typedef typename MemberSet::iterator
			Member_Iterator;
	#ifdef __GNUC__
		typedef typename MemberSet::iterator
			Member_ConstIterator;
	#else
		typedef typename MemberSet::const_iterator
			Member_ConstIterator;
	#endif

		typedef std::list<Set_Iterator>
			SplitSet;
		typedef typename SplitSet::iterator
			Split_Iterator;
	#ifdef __GNUC__
		typedef typename SplitSet::iterator
			Split_ConstIterator;
	#else
		typedef typename SplitSet::const_iterator
			Split_ConstIterator;
	#endif
	};

}

#endif
