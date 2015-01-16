// Description: Coordinate-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_REAL_H
#define PASTELSYS_POINTSET_REAL_H

#include "pastel/sys/pointset_concept.h"
#include "pastel/sys/pointset_location.h"
#include "pastel/sys/point_real.h"

namespace Pastel
{

	namespace PointSet_
	{

		template <typename... PointSetSet>
		struct PointSet_Real;

		template <typename PointSet, typename... PointSetSet>
		struct PointSet_Real<PointSet, PointSetSet...>
		{
			using type = typename std::common_type<
				typename PointSet_Real<PointSet>::type, 
				typename PointSet_Real<PointSetSet...>::type>::type;
		};

		template <typename PointSet>
		struct PointSet_Real<PointSet>
		{
			using type = Point_Real<PointSet_Location<PointSet>>;
		};

	}

	template <typename... PointSetSet>
	using PointSet_Real = typename PointSet_::PointSet_Real<PointSetSet...>::type;

}

#endif
