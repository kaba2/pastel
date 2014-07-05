// Description: PointPolicy concept
// Documentation: pointpolicies.txt

#ifndef PASTELSYS_POINTPOLICY_CONCEPT_H
#define PASTELSYS_POINTPOLICY_CONCEPT_H

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
			from the n() function.
			*/
			static PASTEL_CONSTEXPR int N = UserDefinedInteger;
			
			//! The type of a coordinate.
			using Real = UserDefinedType;
			
			//! The type of a point reference.
			using Point = UserDefinedType;

			//! The type of an associated vector expression.
			using Expression = UserDefinedType;

			//! The type of a const-iterator to coordinates.
			using ConstIterator = UserDefinedType;

			//! The type of a an iterator range to coordinates.
			using ConstRange = UserDefinedType;

			//! Returns a vector expression representing the point.
			/*!
			See the vector expression concept in vectorexpression.txt.
			*/
			Expression operator()(const Point& point) const;

			//! Returns the coordinate on the given axis.
			Real axis(const Point& point, integer axis) const;

			//! Returns a random-access range to coordinate data.
			ConstRange range(const Point& point) const;

			//! Returns an iterator to the beginning of coordinate data.
			ConstIterator begin(const Point& point) const;

			//! Returns an iterator to the end of coordinate data.
			ConstIterator end(const Point& point) const;

			//! Returns the dimensionality of all points.
			/*!
			If there is no specific dimensionality for all points,
			this function should return Dynamic. In this case
			the dimensionality of each point can be obtained
			from the dimension(point) function. Note:
			it does not necessarily hold that n() == N,
			because one can have N == Dynamic, but still a constant
			n() != Dynamic.
			*/
			integer n() const;

			//! Returns the dimensionality of a given point.
			/*!
			If N is non-negative, this function returns N.
			*/
			integer dimension(const Point& point) const;
		};

	}

}

#endif
