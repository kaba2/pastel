// Description: Input of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_INPUT_H
#define PASTELSYS_POINTSET_INPUT_H

#include "pastel/sys/pointset_concept.h"

namespace Pastel
{

	//! Returns the point-input itself.
	template <typename Point_Input>
	const Point_Input& pointSetInput(const Point_Input& that)
	{
		return that;
	}

	template <typename Point_Input>
	Point_Input& pointSetInput(Point_Input& that)
	{
		return that;
	}

	//! Returns the point-input of a location-set.
	template <typename Point_Input, typename Locator>
	const Point_Input& pointSetInput(
		const LocationSet<Point_Input, Locator>& pointSet)
	{
		return pointSet.pointInput();
	}

	template <typename Point_Input, typename Locator>
	Point_Input& pointSetInput(
		LocationSet<Point_Input, Locator>& pointSet)
	{
		return pointSet.pointInput();
	}

	namespace PointSet_
	{

		template <typename PointSet>
		struct PointSet_Input
		{
			using type = PointSet;
		};

		template <
			typename Point_Input,
			typename Locator>
		struct PointSet_Input<LocationSet<Point_Input, Locator>>
		{
			using type = typename PointSet_Input<Point_Input>::type;
		};

	}

	template <typename PointSet>
	using PointSet_Input = typename PointSet_::PointSet_Input<PointSet>::type;


}

#endif
