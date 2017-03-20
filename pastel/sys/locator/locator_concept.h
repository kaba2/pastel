// Description: Locator concept
// Documentation: locators.txt

#ifndef PASTELSYS_LOCATOR_CONCEPT_H
#define PASTELSYS_LOCATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/real/real_concept.h"

#include "pastel/sys/locator/locator_point.h"
#include "pastel/sys/locator/locator_real.h"

namespace Pastel
{

	struct Locator_Concept
	{
		template <
			typename Type,
			//! The type of the point-ids.
			typename PointId = typename Type::Point,
			//! The type of the coordinates.
			typename Real = typename Type::Real,
			//! Compile-time dimension.
			/*!
			If the dimension is dynamic, specify N = Dynamic.
			*/
			integer N = Type::N
		>
		auto requires_(
			Type&& t, 
			PointId&& pointId = std::declval<PointId>(),
			Real&& s = std::declval<Real>()) -> decltype
		(
			conceptCheck(
				//! Returns the dimension of the points.
				/*!
				A non-negative number denotes that the
				points all share the same dimension.
				Negative number means the dimension changes
				from point to point. 
				*/
				Concept::convertsTo<integer>(
					addConst(t).n()
				),

				//! Returns the dimension of a point.
				/*!
				returns:
				A non-negative integer denoting the
				dimension of the given point.
				*/
				Concept::convertsTo<integer>(
					addConst(t).n(pointId)
				),

				//! Returns the i:th coordinate of the given point.
				/*!
				Preconditions:
				0 <= i < n()
				*/
				Concept::convertsTo<Real>(
					addConst(t)(pointId, (integer)0)
				)
			)
		);
	};

}

#include "pastel/sys/locator/locator_n.h"
#include "pastel/sys/locator/locator_archetype.h"

#endif
