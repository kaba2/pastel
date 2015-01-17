// Description: SplitRule concept

#ifndef PASTELGEOMETRY_SPLITRULE_CONCEPT_H
#define PASTELGEOMETRY_SPLITRULE_CONCEPT_H

#include "pastel/sys/input_concept.h"
#include "pastel/sys/locator_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/geometry/shapes/alignedbox.h"

#include <utility>

namespace Pastel
{

	namespace SplitRule_Concept
	{

		//! Splitting rule for kd-tree nodes.
		class SplitRule
		{
		public:
			//! Returns the splitting plane for a kd-tree node.
			/*!
			pointSet:
			A set of points.

			locator:
			A locator to recover the coordinates of the points.

			bound:
			A bounding aligned box for the points.

			returns:
			A pair (s, i), where s denotes the splitting position
			along the i:th standard basis vector in RR^n.

			Postconditions:
			1) 0 <= splitAxis < locator.n()
			2) bound.min()[splitAxis] <= splitPosition <= bound.max()[splitAxis]
			3) The split position has to be such that both sides
			of the splitting plane contain points. A point on the splitting
			plane can be thought to be on either side.
			*/
			template <
				typename Point_Input,
				typename Locator,
				typename Real = typename Locator::Real,
				integer N = Locator::N>
			std::pair<Real, integer> operator()(
				Point_Input pointSet,
				const Locator& locator,
				const AlignedBox<Real, N>& bound) const;
		};

	}

}

#endif
