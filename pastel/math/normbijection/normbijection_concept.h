// Description: NormBijection concept

#ifndef PASTELMATH_NORMBIJECTION_CONCEPT_H
#define PASTELMATH_NORMBIJECTION_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	struct NormBijection_Concept
	{
		template <
			typename Type,
			//! The computation type.
			typename Real = typename Type::Real>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				//! Computes the logarithm of the volume of the unit norm sphere.
				Concept::convertsTo<Real>(
					addConst(t).lnVolumeUnitSphere(
						// dimension
						std::declval<integer>()
					)
				),

				//! Maps a norm bijection value to a norm value.
				Concept::convertsTo<Real>(
					addConst(t).toNorm(
						// normBijectionValue
						std::declval<Real>()
					)
				),

				//! Maps a norm bijection value to a logarithm of the norm value.
				Concept::convertsTo<Real>(
					addConst(t).toLnNorm(
						// normBijectionValue
						std::declval<Real>()
					)
				),

				//! Maps a norm value to a norm bijection value.
				Concept::convertsTo<Real>(
					addConst(t).toBijection(
						// norm
						std::declval<Real>()
					)
				),

				//! Returns a scaling value for norm bijection for scaling the norm value.
				Concept::convertsTo<Real>(
					addConst(t).scalingFactor(
						// scaling
						std::declval<Real>()
					)
				),
				
				//! Returns the norm bijection value along a standard basis axis.
				/*!
				Preconditions:
				axisDistance >= 0

				axisDistance:
				A norm bijection value.
				*/
				Concept::convertsTo<Real>(
					addConst(t).axis(
						// axisDistance
						std::declval<Real>()
					)
				),

				//! Returns the norm bijection value along a standard basis axis.
				/*!
				axisDistance:
				A norm bijection value (possibly negative).
				*/
				Concept::convertsTo<Real>(
					addConst(t).signedAxis(
						// axisDistance
						std::declval<Real>()
					)
				),
				
				//! Adds distance along some standard basis axis.
				/*!
				distance:
				A norm-bijection distance.

				newAxisDistance:
				A norm-bijection distance along a given standard 
				basis axis.
				*/
				Concept::convertsTo<Real>(
					addConst(t).addAxis(
						// distance
						std::declval<Real>(),
						// newAxisDistance
						std::declval<Real>()
					)
				),

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
				Concept::convertsTo<Real>(
					addConst(t).replaceAxis(
						// distance
						std::declval<Real>(),
						// oldAxisDistance
						std::declval<Real>(),
						// newAxisDistance
						std::declval<Real>()
					)
				)
			)
		);
	};

}

#include "pastel/math/normbijection/normbijection_archetype.h"

#endif

