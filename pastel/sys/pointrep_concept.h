// Description: Point-representation concept

#ifndef PASTELSYS_POINTREP_CONCEPT_H
#define PASTELSYS_POINTREP_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class PointRep_Concept
	{
	public:
		//! Compile-time dimension.
		/*!
		If the dimension is dynamic,
		specify N = Dynamic.
		*/
		enum {N = UserDefinedInteger};

		//! The type of the point coordinates.
		typedef UserDefinedType Real;
			
		//! The type of the points.
		typedef UserDefinedType Point;

		//! Run-time dimension.
		/*!
		Returns:
		N, if N != Dynamic, and
		something else, otherwise.
		*/
		integer n() const;

		//! Returns the i:th coordinate of the given point.
		Real operator()(const Point& point, integer i) const;
	};

}

#endif
