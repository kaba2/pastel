// Description: Point-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POINT_H
#define PASTELSYS_POINTSET_POINT_H

#include "pastel/sys/pointset_concept.h"
#include "pastel/sys/location_set.h"

#include <type_traits>

namespace Pastel
{

	namespace PointSet_
	{

		template <typename PointSet>
		struct Point
		{
			using type = Input_Value<PointSet>;
		};

		template <
			typename Point_Input,
			typename Locator>
		struct Point<LocationSet<Point_Input, Locator>>
		{
			using type = typename Point<Point_Input>::type;
		};

	}

	//! Returns the underlying point-type of a point-set-type.
	template <typename PointSet>
	using PointSet_Point = typename PointSet_::Point<
			typename std::decay<PointSet>::type
		>::type;

}

#endif
