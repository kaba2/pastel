// Description: Point-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POINT_H
#define PASTELSYS_POINTSET_POINT_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/input/input_value.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#include <type_traits>

namespace Pastel
{

	namespace PointSet_
	{

		template <typename PointSet>
		struct Point
		: Input_Value_F<PointSet>
		{};

		template <
			typename Point_Input,
			typename Locator>
		struct Point<LocationSet<Point_Input, Locator>>
		: Input_Value_F<Point_Input>
		{};

	}

	template <typename PointSet>
	struct PointSet_Point_F
	: PointSet_::Point<PointSet>
	{};

	//! Returns the underlying point-type of a point-set-type.
	template <typename PointSet>
	using PointSet_Point = 
		typename PointSet_Point_F<PointSet>::type;

}

#endif
