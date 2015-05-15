// Description: Input of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_INPUT_H
#define PASTELSYS_POINTSET_INPUT_H

#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	namespace PointSet_
	{

		//! Returns the point-input itself.
		template <typename Point_Input>
		decltype(auto) pointSetInput(
			Point_Input&& that)
		{
			return std::forward<Point_Input>(that);
		}

		//! Returns the point-input of a location-set.
		template <typename Point_Input, typename Locator>
		decltype(auto) pointSetInput(
			const LocationSet<Point_Input, Locator>& pointSet)
		{
			return pointSet.pointInput();
		}

		template <typename Point_Input, typename Locator>
		decltype(auto) pointSetInput(
			LocationSet<Point_Input, Locator>& pointSet)
		{
			return pointSet.pointInput();
		}

	}

	//! Returns the point-input.
	template <typename PointSet>
	decltype(auto) pointSetInput(PointSet&& pointSet)
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
		return PointSet_::pointSetInput(std::forward<PointSet>(pointSet));
	}

}

namespace Pastel
{

	template <typename PointSet>
	struct PointSet_Input_F
	: Identity_F<PointSet>
	{};

	template <
		typename Input,
		typename Locator>
	struct PointSet_Input_F<LocationSet<Input, Locator>>
	: Identity_F<Input>
	{};

	template <typename PointSet>
	using PointSet_Input = 
		typename PointSet_Input_F<PointSet>::type;

}

#endif
