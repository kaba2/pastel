// Description: NormBijection concept

#ifndef PASTELMATH_NORMBIJECTION_CONCEPT_H
#define PASTELMATH_NORMBIJECTION_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename T>
	concept NormBijection_Concept__ = requires(T t, typename T::Real s) {
		//! The computation type.
		typename T::Real;

		//! Computes the logarithm of the volume of the unit norm sphere.
		{
			addConst(t).lnVolumeUnitSphere(
				// dimension
				(integer)0
			)
		} -> std::convertible_to<typename T::Real>;

		//! Maps a norm bijection value to a norm value.
		{
			addConst(t).toNorm(
				// normBijectionValue
				s
			)
		} -> std::convertible_to<typename T::Real>;

		//! Maps a norm bijection value to a logarithm of the norm value.
		{
			addConst(t).toLnNorm(
				// normBijectionValue
				s
			)
		} -> std::convertible_to<typename T::Real>;

		//! Maps a norm value to a norm bijection value.
		{
			addConst(t).toBijection(
				// norm
				s
			)
		} -> std::convertible_to<typename T::Real>;

		//! Returns a scaling value for norm bijection for scaling the norm value.
		{
			addConst(t).scalingFactor(
				// scaling
				s
			)
		} -> std::convertible_to<typename T::Real>;
		
		//! Returns the norm bijection value along a standard basis axis.
		/*!
		Preconditions:
		axisDistance >= 0

		axisDistance:
		A norm bijection value.
		*/
		{
			addConst(t).axis(
				// axisDistance
				s
			)
		} -> std::convertible_to<typename T::Real>;

		//! Returns the norm bijection value along a standard basis axis.
		/*!
		axisDistance:
		A norm bijection value (possibly negative).
		*/
		{
			addConst(t).signedAxis(
				// axisDistance
				s
			)
		} -> std::convertible_to<typename T::Real>;
		
		//! Adds distance along some standard basis axis.
		/*!
		distance:
		A norm-bijection distance.

		newAxisDistance:
		A norm-bijection distance along a given standard 
		basis axis.
		*/
		{
			addConst(t).addAxis(
				// distance
				s,
				// newAxisDistance
				s
			)
		} -> std::convertible_to<typename T::Real>;

		//! Replace distance along some standard basis axis.
		/*!
		distance:
		A norm-bijection distance.

		oldAxisDistance:
		The current norm-bijection distance along a given 
		standard basis axis.

		newAxisDistance:
		The new norm-bijection distance along the given 
		standard basis axis.
		*/
		{
			addConst(t).replaceAxis(
				// distance
				s,
				// oldAxisDistance
				s,
				// newAxisDistance
				s
			)
		} -> std::convertible_to<typename T::Real>;
	};

	template <typename T>
	concept NormBijection_Concept_ = NormBijection_Concept__<RemoveCvRef<T>>;

}

#include "pastel/math/normbijection/normbijection_archetype.h"

#endif

