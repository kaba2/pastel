#ifndef PASTELSYS_LOCATION_H
#define PASTELSYS_LOCATION_H

#include "pastel/sys/locator_concept.h"

namespace Pastel
{

	//! A point and a locator.
	/*!
	This class is used to explicitly assign a locator to a 
	point. An alternative is to assign a default locator for
	the point-type to by specializing the Default_Locator 
	class template.
	*/
	template <
		typename Point,
		typename Locator>
	class Location
	{
	public:
		Location(
			const Point& point,
			const Locator& locator)
			: point_(point)
			, locator_(locator)
		{
		}

		const Point& point() const
		{
			return point_;
		}

		const Locator& locator() const
		{
			return locator_;
		}

	private:
		Point point_;
		Locator locator_;
	};

	//! Constructs a location from a point and a locator.
	template <typename Point, typename Locator>
	Location<Point, Locator> location(
		const Point& point,
		const Locator& locator)
	{
		return Location<Point, Locator>(
			point, locator);
	}

}

#endif
