// Description: Locator concept

#ifndef PASTELSYS_LOCATOR_CONCEPT_H
#define PASTELSYS_LOCATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"

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

			//! The type of the point coordinates.
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

			//! Returns the i:th coordinate of the given point.
			/*!
			Preconditions:
			0 <= i < n()
			*/
			Real operator()(const Point& point, integer i) const;
		};

	}

}

#endif
