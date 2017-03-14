// Description: Point-set concept
// Documentation: pointset.txt

#ifndef PASTELSYS_POINTSET_CONCEPT_H
#define PASTELSYS_POINTSET_CONCEPT_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/type_traits/is_template_instance.h"
#include "pastel/sys/type_traits/or.h"

namespace Pastel
{

	namespace PointSet_
	{

		struct PointSet_Set_Concept
		{
			// A PointSet is either 
			// * a Range whose elements have a default-locator, or
			// * a LocationSet, which pairs a Set with a Locator.
			template <typename Type>
			auto requires_(Type&& t) -> decltype
			(
				conceptCheck(
					Concept::holds<
						And<
							Models<Type, Set_Concept>,
							HasDefaultLocator<Set_Element<Type>>
						>
					>()
				)
			);
		};

		struct PointSet_LocationSet_Concept
		{
			template <typename Type>
			auto requires_(Type&& t) -> decltype
			(
				conceptCheck(
					Concept::holds<
						IsLocationSet<Type>
					>()
				)
			);
		};

	}


	struct PointSet_Concept
	{
		// A PointSet is either 
		// * a Range whose elements have a default-locator, or
		// * a LocationSet, which pairs a Set with a Locator.
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::holds<
					Not<Models<Type, Point_Concept>>,
					Or<
						// Note that the two cases below cannot be embedded
						// here directly. This is because Set_Element<Type> 
						// fails the concept-checking when Type is not Set.
						Models<Type, PointSet_::PointSet_Set_Concept>,
						Models<Type, PointSet_::PointSet_LocationSet_Concept>
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
