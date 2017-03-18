// Description: Locator of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_LOCATOR_H
#define PASTELSYS_POINTSET_LOCATOR_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/set/set_concept.h"

namespace Pastel
{

	namespace PointSet_
	{

		struct MemberPointSetLocator_Concept
		{
			template <typename Type>
			auto requires_(Type&& t) -> decltype
			(
				conceptCheck(
					Concept::holds<
						Models<decltype(addConst(t).pointSetLocator()), Locator_Concept>
					>()
				)
			);
		};

	}

	template <typename Type>
	using HasMemberPointSetLocator = 
		Models<Type, PointSet_::MemberPointSetLocator_Concept>;
	
}

namespace Pastel
{

	//! Returns the member-locator of a point-set.
	template <
		typename Type,
		Requires<
			HasMemberPointSetLocator<Type>
		> = 0
	>
	decltype(auto) pointSetLocator(Type&& that)
	{
		return std::forward<Type>(that).pointSetLocator();
	}

	//! Returns the default locator of point-set elements.
	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>,
			// // Give priority to the member-locator.
			Not<HasMemberPointSetLocator<Set>>,
			// Dimension has to be known at compile-time.
			BoolConstant<(Point_N<Set_Element<Set>>::value >= 0)>
		> = 0
	>
	decltype(auto) pointSetLocator(const Set& set)
	{
		using Point = Set_Element<Set>;
		return Usual_Locator<Point, Point_Real<Point>, Point_N<Point>::value>();
	}

}

namespace Pastel
{

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	struct PointSet_Locator_F 
		: Identity_F<
			RemoveCvRef<decltype(pointSetLocator(std::declval<PointSet>()))>>
	{};

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	using PointSet_Locator =
		typename PointSet_Locator_F<PointSet>::type;

}

#endif
