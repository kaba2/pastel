// Description: Locator of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_LOCATOR_H
#define PASTELSYS_POINTSET_LOCATOR_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/locator/default_locator.h"

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
			Models<Set_Element<Set>, Point_Concept>,
			// Give priority to the member-locator.
			Not<HasMemberPointSetLocator<Set>>
		> = 0
	>
	decltype(auto) pointSetLocator(const Set& set)
	{
		using Point = Set_Element<Set>;
		using Real = Point_Real<Point>;
		static constexpr integer N = Point_N<Point>::value;

		return Default_Locator<Point, Real, N>(N);
	}

}

namespace Pastel
{

	template <typename PointSet>
	struct PointSet_Locator_F 
		: Identity_F<
			RemoveCvRef<decltype(pointSetLocator(std::declval<PointSet>()))>>
	{};

	template <typename PointSet>
	using PointSet_Locator =
		typename PointSet_Locator_F<PointSet>::type;

}

#endif
