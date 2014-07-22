// Description: Locator concept

#ifndef PASTELSYS_LOCATOR_CONCEPT_H
#define PASTELSYS_LOCATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/real_concept.h"

namespace Pastel
{

	namespace Locator_Concept
	{

		class Locator
		{
		public:
			//! Compile-time dimension.
			/*!
			If the dimension is dynamic,
			specify N = Dynamic.
			*/
			static PASTEL_CONSTEXPR integer N = UserDefinedInteger;

			//! The type of the coordinates.
			using Real = UserDefinedType;

			//! The type of the points.
			using Point = UserDefinedType;

			//! Run-time dimension.
			/*!
			Returns:
			N, if N != Dynamic, and
			non-negative integer, otherwise.
			*/
			integer n() const;

			//! Swaps two locators.
			void swap(Locator& that);

			//! Returns the i:th coordinate of the given point.
			/*!
			Preconditions:
			0 <= i < n()
			*/
			Real operator()(
				const Point& point, integer i) const;
		};

	}

}

#include "pastel/sys/locator_n.h"
#include "pastel/sys/locator_point.h"
#include "pastel/sys/locator_real.h"

#endif
