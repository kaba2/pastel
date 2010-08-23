// Description: PointPolicy concept
// Documentation: pointpolicy.txt

#ifndef PASTEL_POINTPOLICY_CONCEPT_H
#define PASTEL_POINTPOLICY_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class PointPolicy_Concept
	{
	public:
		//! Defines the compile-time dimension.
		/*!
		If the dimension 'd' is a compile-time
		constant, N == d. Otherwise, N == Dynamic,
		and 'd' can be obtained for each point at run-time 
		from the dimension() function.
		*/
		enum {N = UserDefinedInteger};
		
		//! Defines the type of the coordinates.
		typedef UserDefinedType Coordinate;
		
		//! Defines the type of a point.
		typedef UserDefinedType Object;

		//! Returns a pointer to a coordinate array for the point.
		const Coordinate* point(const Object& object) const;

		//! Returns a coordinate of the given point on the given axis.
		Coordinate point(const Object& object, integer axis) const;
		
		//! Returns the dimensionality of all points.
		/*!
		If there is no specific dimensionality for all points,
		this function should return Dynamic. In this case
		the dimensionality of each point can be obtained
		from the dimension(object) function. Note:
		it does not necessarily hold that dimension() == N,
		because one can have N == Dynamic, but still a constant
		dimension() != Dynamic.
		*/
		integer dimension() const;

		//! Returns the dimensionality of a given point.
		/*!
		If N is non-negative, this function returns N.
		*/
		integer dimension(const Object& object) const;
	};

}

#endif
