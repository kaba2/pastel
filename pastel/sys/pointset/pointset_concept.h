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

	struct PointSet_Concept
	{
		// A PointSet is either 
		// * a Set whose elements have a default-locator, or
		// * a LocationSet, which pairs a Set with a Locator.
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::holds<
					Or<
						And<
							Models<Type, Set_Concept>,
							// We must use Set_Element_F rather than Set_Element here
							// to avoid a bug in Visual Studio 2015 Update 1 RTM.
							HasDefaultLocator<typename Set_Element_F<Type>::type>
						>,
						IsTemplateInstance<Type, LocationSet>
					>
				>()
			)
		);
	};

}

#include "pastel/sys/pointset/pointset_dimension.h"
#include "pastel/sys/pointset/pointset_set.h"
#include "pastel/sys/pointset/pointset_locator.h"
#include "pastel/sys/pointset/pointset_n.h"
#include "pastel/sys/pointset/pointset_point_id.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/pointset/pointset_real.h"

#endif
