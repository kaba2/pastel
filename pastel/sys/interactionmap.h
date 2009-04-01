#ifndef PASTELSYS_INTERACTIONMAP_H
#define PASTELSYS_INTERACTIONMAP_H

#include <pastel/sys/poolallocator.h>
#include <pastel/sys/unorderedmap.h>
#include <pastel/sys/unorderedset.h>

#include <boost/shared_ptr.hpp>

namespace Pastel
{

	template <typename First, typename Second>
	class InteractionMap
	{
	public:
		typedef UnorderedSet<First>
			FirstList;
		typedef typename FirstList::iterator
			FirstListIterator;
		typedef typename FirstList::const_iterator
			ConstFirstListIterator;
		typedef boost::shared_ptr<FirstList>
			FirstListPtr;
		typedef boost::shared_ptr<const FirstList>
			ConstFirstListPtr;

		typedef UnorderedSet<Second>
			SecondList;
		typedef typename SecondList::iterator
			SecondListIterator;
		typedef typename SecondList::const_iterator
			ConstSecondListIterator;
		typedef boost::shared_ptr<SecondList>
			SecondListPtr;
		typedef boost::shared_ptr<const SecondList>
			ConstSecondListPtr;

	private:
		typedef UnorderedMap<
			First, SecondListPtr>
			FirstContainer;
		typedef typename FirstContainer::iterator
			FirstIterator;

		typedef UnorderedMap<
			Second, FirstListPtr>
			SecondContainer;
		typedef typename SecondContainer::iterator
			SecondIterator;

	public:
		typedef typename FirstContainer::const_iterator
			ConstFirstIterator;

		typedef typename SecondContainer::const_iterator
			ConstSecondIterator;

		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		ConstFirstIterator firstBegin() const;
		ConstFirstIterator firstEnd() const;

		ConstSecondIterator secondBegin() const;
		ConstSecondIterator secondEnd() const;

		void addInteraction(
			const First& first,
			const Second& second);
		void removeInteraction(
			const First& first,
			const Second& second);
		void removeFirst(
			const First& first);
		void removeSecond(
			const Second& second);

	private:
		FirstContainer firsts_;
		SecondContainer seconds_;
	};

}

#include "pastel/sys/interactionmap.hpp"

#endif
