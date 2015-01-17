// Description: Pairing of a point-input and a locator
// Documentation: pointset.txt

#ifndef PASTELSYS_LOCATION_SET_H
#define PASTELSYS_LOCATION_SET_H

#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	//! A point-input and a locator.
	/*!
	This class is used to explicitly assign a locator to a point-input.
	Alternatively, if the underlying point-type has a default locator,
	then this assignment need not be done.
	*/
	template <
		typename Point_Input,
		typename Locator>
	class LocationSet
	{
	public:
		LocationSet(
			const Point_Input& pointInput,
			const Locator& locator)
			: pointInput_(pointInput)
			, locator_(locator)
		{
		}

		Point_Input& pointInput()
		{
			return pointInput_;
		}
		
		const Point_Input& pointInput() const
		{
			return pointInput_;
		}

		const Locator& locator() const
		{
			return locator_;
		}

	private:
		Point_Input pointInput_;
		Locator locator_;
	};

	//! Constructs a location-set from a point-set and a locator.
	template <typename Point_Input, typename Locator>
	LocationSet<Point_Input, Locator> locationSet(
		const Point_Input& pointInput,
		const Locator& locator)
	{
		return LocationSet<Point_Input, Locator>(
			pointInput, locator);
	}

}

#endif
