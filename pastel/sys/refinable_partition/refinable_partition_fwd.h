#ifndef PASTELSYS_REFINABLE_PARTITION_FWD_H
#define PASTELSYS_REFINABLE_PARTITION_FWD_H

#include "pastel/sys/class.h"

#include <list>

namespace Pastel
{
	
	template <typename ElementData, typename SetData>
	class RefinablePartition_Fwd
	{
	public:
		class Element;
		using ElementSet = std::list<Element>;

		typedef typename ElementSet::iterator
			Element_Iterator;
	#ifdef __GNUC__
		typedef typename ElementSet::iterator
			Element_ConstIterator;
	#else
		typedef typename ElementSet::const_iterator
			Element_ConstIterator;
	#endif

		struct Element_Tag;
		using ElementData_Class = Class<ElementData, Element_Tag>;

		class Set;
		using SetSet = std::list<Set>;

		typedef typename SetSet::iterator
			Set_Iterator;
	#ifdef __GNUC__
		typedef typename SetSet::iterator
			Set_ConstIterator;
	#else
		typedef typename SetSet::const_iterator
			Set_ConstIterator;
	#endif

		struct Set_Tag;
		using SetData_Class = Class<SetData, Set_Tag>;

		using MemberSet = std::list<Element_Iterator>;

		typedef typename MemberSet::iterator
			Member_Iterator;
	#ifdef __GNUC__
		typedef typename MemberSet::iterator
			Member_ConstIterator;
	#else
		typedef typename MemberSet::const_iterator
			Member_ConstIterator;
	#endif

		using SplitSet = std::list<Set_Iterator>;
		
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
