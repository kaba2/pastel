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

	//! Computes coordinates for a point.
	/*!
	There are three kinds of locators:
	1) Compile-time dimension N for all points.
	2) Run-time dimension n for all points (N < 0).
	3) Run-time dimension n(p) for each point p (n, N < 0).
	*/
	template <typename T>
	concept Locator_Concept__ = requires(T t, typename T::Point point) {
		//! The type of the point-ids.
		typename T::Point;
		
		//! The type of the coordinates.
		typename T::Real;
		
		//! Compile-time dimension.
		/*!
		If the dimension is dynamic, specify N = Dynamic.
		*/
		{T::N} -> std::convertible_to<integer>;
		
		//! Returns the dimension of the points.
		/*!
		A non-negative number denotes that the
		points all share the same dimension.
		Negative number means the dimension changes
		from point to point. 
		*/
		{addConst(t).n()} -> std::convertible_to<integer>;

		//! Returns the dimension of a point.
		/*!
		returns:
		A non-negative integer denoting the
		dimension of the given point.
		*/
		{addConst(t).n(point)} -> std::convertible_to<integer>;

		//! Returns the i:th coordinate of the given point.
		/*!
		Preconditions:
		0 <= i < n()
		*/
		{addConst(t)(point, (integer)0)} -> std::convertible_to<typename T::Real>;
	};

	template <typename T>
	concept Locator_Concept_ = 
		Locator_Concept__<RemoveCvRef<T>>;

}

#include "pastel/sys/locator/locator_n.h"
#include "pastel/sys/locator/locator_archetype.h"

#endif
