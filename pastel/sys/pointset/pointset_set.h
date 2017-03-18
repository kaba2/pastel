// Description: Set of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_SET_H
#define PASTELSYS_POINTSET_SET_H

#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	namespace PointSet_
	{

		struct MemberPointSet_Concept
		{
			template <typename Type>
			auto requires_(Type&& t) -> decltype
			(
				conceptCheck(
					Concept::holds<
						Models<decltype(addConst(t).pointSetSet())>
					>()
				)
			);
		};

	}

	template <typename Type>
	using HasMemberPointSet = 
		Models<Type, PointSet_::MemberPointSet_Concept>;

	template <
		typename Type,
		Requires<
			HasMemberPointSet<Type>
		> = 0
	>
	decltype(auto) pointSetSet(Type&& that)
	{
		return std::forward<Type>(that).pointSetSet();
	}


	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>,
			// Give priority to the member-locator.
			Not<HasMemberPointSet<Set>>
		> = 0
	>
	decltype(auto) pointSetSet(Set&& set)
	{
		return std::forward<Set>(set);
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
	using PointSet_Set = RemoveCvRef<decltype(pointSetSet(std::declval<PointSet>()))>;

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	struct PointSet_Set_F 
		: Identity_F<PointSet_Set<PointSet>>
	{};

}

#endif
