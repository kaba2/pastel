// Description: Point concept

#ifndef PASTELSYS_POINT_CONCEPT_H
#define PASTELSYS_POINT_CONCEPT_H

#include "pastel/sys/locator/location.h"
#include "pastel/sys/real/real_ring_concept.h"

#include "pastel/sys/point/point_dimension.h"
#include "pastel/sys/point/point_axis.h"
#include "pastel/sys/point/point_point_id.h"

namespace Pastel
{

	struct Point_Concept
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::models<Real_Ring_Concept>(pointAxis(addConst(t), (integer)0)),
				Concept::convertsTo<integer>(dimension(addConst(t))),
				//Concept::models<Locator>(pointLocator(addConst(t))),
				pointPointId(addConst(t))
			)
		);
	};

	template <typename T>
	concept Point_Concept_ = requires(T t) {
		{removeReference(pointAxis(addConst(t), (integer)0))} -> Real_Ring_Concept_;
		{dimension(addConst(t))} -> std::convertible_to<integer>;
		// {removeReference(pointLocator(addConst(t)))} -> Locator_Concept_;
		pointPointId(addConst(t));
	};

}

#include "pastel/sys/point/point_locator.h"
#include "pastel/sys/point/point_real.h"
#include "pastel/sys/point/point_archetype.h"

#include <iostream>

namespace Pastel
{

	template <Point_Concept_ Point>
	void printPoint(
		std::ostream& stream, 
		const Point& point)
	{
		integer n = dimension(point);
		stream << "(";
		for (integer i = 0;i < n; ++i)
		{
			stream << pointAxis(point, i);
			if (i < n - 1)
			{
				stream << ", ";
			}
		}
		stream << ")";
	}

}

#endif
