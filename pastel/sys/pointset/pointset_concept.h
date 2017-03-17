// Description: Point-set concept
// Documentation: pointset.txt

#ifndef PASTELSYS_POINTSET_CONCEPT_H
#define PASTELSYS_POINTSET_CONCEPT_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/type_traits/or.h"

namespace Pastel
{

	struct PointSet_Concept_Element
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::holds<
					Models<Type, Set_Concept>,
					Not<Models<Type, Point_Concept>>,
					HasDefaultLocator<Set_Element<Type>>
				>()
			)
		);
	};

	struct PointSet_Concept_Member
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::holds<
					Models<Type, Set_Concept>,
					Not<Models<Type, Point_Concept>>,
					Models<decltype(addConst(t).pointSetLocator()), Locator_Concept>
				>()
			)
		);
	};

	struct PointSet_Concept
	{
		// A PointSet is a Set with an associated locator.
		// The locator is searched for in the following order:
		// 1) class member function locator(),
		// 2) the locator of a set-element.
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::holds<
					Or<
						// Note that the two cases below cannot be embedded
						// here directly. This is because Set_Element<Type> 
						// fails the concept-checking when Type is not Set.
						Models<Type, PointSet_Concept_Element>,
						Models<Type, PointSet_Concept_Member>
					>
				>()
			)
		);
	};

}

#include "pastel/sys/pointset/pointset_dimension.h"
#include "pastel/sys/pointset/pointset_empty.h"
#include "pastel/sys/pointset/pointset_set.h"
#include "pastel/sys/pointset/pointset_locator.h"
#include "pastel/sys/pointset/pointset_n.h"
#include "pastel/sys/pointset/pointset_point_id.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/pointset/pointset_real.h"

#endif
