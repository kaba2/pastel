// Description: Pairing of a point and a locator
// Documentation: point.txt

#ifndef PASTELSYS_LOCATION_H
#define PASTELSYS_LOCATION_H

#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	//! Pairing of a locator and a variable.
	/*!
	This class is used to explicitly assign a locator to a 
	variable. An alternative is to assign a default locator for
	the type by specializing the Default_Locator class template.
	*/
	template <
		typename Type,
		typename Locator,
		// We have to use typename = constraint here, because
		// otherwise IsTemplateInstance does not work
		// with Location.
		typename = 
			Requires<
				Models<Locator, Locator_Concept(Type)>
			>
	>
	class Location
	{
	public:
		Location(
			const Type& point,
			const Locator& locator)
			: point_(point)
			, locator_(locator)
		{
		}

		const Type& point() const
		{
			return point_;
		}

		const Locator& locator() const
		{
			return locator_;
		}

	private:
		Type point_;
		Locator locator_;
	};

	//! Constructs a location from a variable and a locator.
	template <
		typename Type, 
		typename Locator,
		Requires<
			Models<Locator, Locator_Concept(Type)>
		> = 0
	>
	Location<Type, Locator> location(
		const Type& point,
		const Locator& locator)
	{
		return Location<Type, Locator>(point, locator);
	}

}

#endif
