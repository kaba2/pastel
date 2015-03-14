// Description: Locator concept

#ifndef PASTELSYS_LOCATOR_CONCEPT_H
#define PASTELSYS_LOCATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/real/real_concept.h"

namespace Pastel
{

	struct Locator_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Compile-time dimension.
				/*!
				If the dimension is dynamic,
				specify N = Dynamic.
				*/
				Concept::convertsTo<integer>(Type::N),

				//! The type of the coordinates.
				std::declval<typename Type::Real>(),

				//! The type of the points.
				std::declval<typename Type::Point>(),

				//! Run-time dimension.
				/*!
				Returns:
				N, if N != Dynamic, and
				non-negative integer, otherwise.
				*/
				Concept::convertsTo<integer>(t.n()),

				//! Swaps two locators.
				(t.swap(t), 0),

				//! Returns the i:th coordinate of the given point.
				/*!
				Preconditions:
				0 <= i < n()
				*/
				Concept::convertsTo<typename Type::Real>(
					t(std::declval<typename Type::Point>(),
					  (integer)0))
			)
		);
	};

}

#include "pastel/sys/locator/locator_n.h"
#include "pastel/sys/locator/locator_point.h"
#include "pastel/sys/locator/locator_real.h"

#endif
