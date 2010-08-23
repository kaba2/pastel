// Description: NormBijection concept

#ifndef PASTEL_NORMBIJECTION_CONCEPT_H
#define PASTEL_NORMBIJECTION_CONCEPT_H

namespace Pastel
{

	template <typename Real>
	class NormBijection_Concept
	{
	public:
		//! Computes the logarithm of the volume of the unit norm sphere.
		Real lnVolumeUnitSphere(integer dimension) const;
		
		//! Maps a norm bijection value to a norm value.
		Real toNorm(const Real& normBijection) const;

		//! Maps a norm bijection value to a logarithm of the norm value.
		Real toLnNorm(const Real& normBijection) const;

		//! Maps a norm value to a norm bijection value.
		Real toBijection(const Real& norm) const;

		//! Returns a scaling value for norm bijection for scaling the norm value.
		Real scalingFactor(const Real& scaling) const;
		
		//! Returns the norm bijection value along a standard basis axis.
		/*!
		Here it is assumed that 'axisDistance' is non-negative.
		It must be a norm bijection value.
		*/
		Real axis(const Real& axisDistance) const;

		//! Returns the norm bijection value along a standard basis axis.
		/*!
		Here the 'axisDistance' can also be negative.
		It must be a norm bijection value.
		*/
		Real signedAxis(const Real& axisDistance) const;
		
		//! Adds distance along some standard basis axis.
		/*!
		These distances must be norm bijection values.
		Before this addition, the distance along the
		particular standard basis axis must be zero.
		*/
		Real addAxis(
			const Real& distance, 
			const Real& newAxisDistance) const;

		//! Replace distance along some standard basis axis.
		/*!
		These distances must be norm bijection values.
		*/
		Real replaceAxis(
			const Real& distance, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const;
	};

}

#endif

