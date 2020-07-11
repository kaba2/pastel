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
		typename Locator>
	requires Locator_Concept<Locator>
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

		decltype(auto) pointAxis(integer axis) const
		{
			return locator_(point_, axis);
		}

		integer pointDimension() const
		{
			return locator_.n(point_);
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
		typename Locator>
	decltype(auto) location(
		const Type& point,
		const Locator& locator)
	{
		return Location<Type, Locator>(point, locator);
	}

	template <
		typename Type,
		typename Locator>
	decltype(auto) pointPointId(const Location<Type, Locator>& location)
	{
		return location.point();
	}


}

#endif
