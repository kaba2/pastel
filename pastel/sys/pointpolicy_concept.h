// Description: PointPolicy concept
// Documentation: pointpolicy.txt

#ifndef PASTEL_POINTPOLICY_CONCEPT_H
#define PASTEL_POINTPOLICY_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vectorexpression_concept.h"

namespace Pastel
{

	namespace PointPolicy_Concept
	{

		class PointPolicy
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
			
			//! The type of a coordinate.
			typedef UserDefinedType Coordinate;
			
			//! The type of a point reference.
			typedef UserDefinedType Object;

			//! The type of an associated vector expression.
			typedef UserDefinedType Expression;

			//! The type of a const-iterator to coordinates.
			typedef UserDefinedType ConstIterator;

			//! Returns a vector expression representing the point.
			/*!
			See the vector expression concept in vectorexpression.txt.
			*/
			Expression operator()(const Object& object) const;

			//! Returns an iterator to the beginning of coordinate data.
			ConstIterator begin(const Object& object) const;

			//! Returns an iterator to the end of coordinate data.
			ConstIterator end(const Object& object) const;

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

}

#endif
