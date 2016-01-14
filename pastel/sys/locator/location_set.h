// Description: A set with a locator; a point-set.
// Documentation: pointset.txt

#ifndef PASTELSYS_LOCATION_SET_H
#define PASTELSYS_LOCATION_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	//! A set with a locator; a point-set.
	template <
		typename Set,
		typename Locator
		// This check triggers an internal compiler
		// error in Visual Studio 2015 RC.
		/*,
		Requires<
			Models<Set, Set_Concept>,
			Models<Locator, Locator_Concept>
		> = 0*/
	>
	class LocationSet
	{
	public:
		using Element = Location<Set_Element<Set>, Locator>;
		using Index = Set_Index<Set>;

		LocationSet(
			const Set& set,
			const Locator& locator)
			: set_(set)
			, locator_(locator)
		{
		}

		integer n() const
		{
			return set_.n();
		}

		Index begin() const
		{
			return set_.begin();
		}

		Element operator[](const Index& index) const
		{
			return {set_[index], locator_};
		}

		bool empty() const
		{
			return set_.empty();
		}

		bool empty(const Index& index) const
		{
			return set_.empty(index);
		}

		integer next(Index& index, integer steps = 1) const
		{
			return set_.next(index, steps);
		}
		
		Set& pointSet()
		{
			return set_;
		}
		
		const Set& pointSet() const
		{
			return set_;
		}

		const Locator& locator() const
		{
			return locator_;
		}

	private:
		Set set_;
		Locator locator_;
	};

	//! Constructs a location-set from a point-set and a locator.
	template <typename Set, typename Locator>
	LocationSet<Set, Locator> locationSet(
		const Set& set,
		const Locator& locator)
	{
		return {set, locator};
	}

}

#endif
