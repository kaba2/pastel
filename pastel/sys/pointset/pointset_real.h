// Description: Coordinate-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_REAL_H
#define PASTELSYS_POINTSET_REAL_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_location.h"
#include "pastel/sys/point/point_real.h"

namespace Pastel
{

	namespace PointSet_
	{

		template <typename PointSet>
		struct PointSet_Real
		{
			PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

			using type = Point_Real<PointSet_Location<PointSet>>;
		};

	}

	template <typename... PointSetSet>
	using PointSet_Real_F =
		std::common_type<typename PointSet_::PointSet_Real<PointSetSet>::type...>;

	template <typename... PointSetSet>
	using PointSet_Real =
		typename PointSet_Real_F<PointSetSet...>::type;
}

#endif
