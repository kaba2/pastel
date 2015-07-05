// Description: Locator concept

#ifndef PASTELSYS_LOCATOR_CONCEPT_H
#define PASTELSYS_LOCATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/real/real_concept.h"

namespace Pastel
{

	struct Locator_Concept
	{
		template <
			typename Type,
			//! The type of the coordinates.
			typename Real = typename Type::Real,
			//! The type of the point-ids.
			typename PointId = typename Type::Point,
			//! Compile-time dimension.
			/*!
			If the dimension is dynamic, specify N = Dynamic.
			*/
			integer N = Type::N
		>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Run-time dimension.
				/*!
				Returns:
				N, if N != Dynamic, and
				non-negative integer, otherwise.
				*/
				Concept::convertsTo<integer>(
					addConst(t).n()
				),

				//! Swaps two locators.
				(t.swap(t), 0),

				//! Returns the i:th coordinate of the given point.
				/*!
				Preconditions:
				0 <= i < n()
				*/
				Concept::convertsTo<Real>(
					addConst(t)(std::declval<PointId>(),
					  (integer)0)
				)
			)
		);
	};

	template <
		typename Real_,
		typename Point_,
		integer N_>
	struct Locator_Archetype
	{
		static constexpr integer N = N_;
		using Real = Real_;
		using Point = Point_;

		void swap(Locator_Archetype& that) {}

		Real operator()(const Point& point, integer i) const
		{
			return Real();
		}

		integer n() const
		{
			return 1;
		}
	};

}

#include "pastel/sys/locator/locator_n.h"
#include "pastel/sys/locator/locator_point.h"
#include "pastel/sys/locator/locator_real.h"

#endif
