// Description: Statistics from samples
// Detail: pointSetMean, pointSetVariance, etc.
// Documentation: common.txt

#ifndef PASTEL_STATISTICS_H
#define PASTEL_STATISTICS_H

#include "pastel/sys/vector.h"
#include "pastel/sys/range.h"
#include "pastel/sys/pointpolicy_concept.h"

#include <boost/iterator/iterator_traits.hpp>

#include <vector>

namespace Pastel
{

	//! Returns the mean of a number set.
	template <
		typename Real,
		typename Real_ConstRange>
	Real numberMean(
		const Real_ConstRange& inputSet);

	//! Returns the variance of a number set.
	template <
		typename Real,
		typename Real_ConstRange>
	Real numberVariance(
		const Real_ConstRange& inputSet,
		const PASTEL_NO_DEDUCTION(Real)& mean,
		bool biased = true);

	//! Returns the covariance between number sets.
	template <
		typename Real,
		typename A_Real_ConstRange,
		typename B_Real_ConstRange>
	Real numberCovariance(
		const A_Real_ConstRange& aSet,
		const PASTEL_NO_DEDUCTION(Real)& aMean,
		const B_Real_ConstRange& bSet,
		const PASTEL_NO_DEDUCTION(Real)& bMean,
		bool biased = true);

	//! Returns the mean of a point set.
	template <typename Point_ConstRange, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> pointSetMean(
		const Point_ConstRange& pointSet,
		const PointPolicy& pointPolicy);

	//! Returns the variance along each axis.
	template <typename Point_ConstRange, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
		pointSetVariance(
		const Point_ConstRange& pointSet,
		const typename PointPolicy::Point& mean,
		const PointPolicy& pointPolicy);

	//! Returns the mean squared error between sequences.
	template <
		typename Real, 
		typename A_ConstRange, 
		typename B_ConstRange>
	Real meanSquaredError(
		const A_ConstRange& aSet,
		const B_ConstRange& bSet);

	//! Computes peak-signal-to-noise-ratio (PSNR) between sequences.
	template <
		typename Real, 
		typename A_ConstRange, 
		typename B_ConstRange>
	Real peakSignalToNoise(
		const A_ConstRange& aSet,
		const B_ConstRange& bSet,
		const PASTEL_NO_DEDUCTION(Real)& maxValue);

	//! Computes peak-signal-to-noise-ratio (PSNR) between sequences.
	/*!
	This is a convenience function that calls:
	peakSignalToNoise<Real>(aSet, bSet, 1);
	See the documentation for that function.
	*/
	template <
		typename Real, 
		typename A_ConstRange, 
		typename B_ConstRange>
	Real peakSignalToNoise(
		const A_ConstRange& aSet,
		const B_ConstRange& bSet);

}

#include "pastel/math/statistics.hpp"

#endif
