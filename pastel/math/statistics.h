// Description: Statistics from samples
// Detail: pointSetMean, pointSetVariance, etc.
// Documentation: common.txt

#ifndef PASTEL_STATISTICS_H
#define PASTEL_STATISTICS_H

#include "pastel/sys/vector.h"
#include "pastel/sys/forwardrange.h"
#include "pastel/sys/pointpolicy_concept.h"

#include <vector>

namespace Pastel
{

	//! Returns the mean of a point set.
	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> pointSetMean(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy);

	//! Returns the variance along each axis.
	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
		pointSetVariance(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const typename PointPolicy::Point& mean,
		const PointPolicy& pointPolicy);

	//! Returns the mean squared error between sequences.
	template <
		typename Real, 
		typename A_ConstIterator, 
		typename B_ConstIterator>
	Real meanSquaredError(
		const ForwardRange<A_ConstIterator>& aSet,
		const ForwardRange<B_ConstIterator>& bSet);

	//! Computes peak-signal-to-noise-ratio (PSNR) between sequences.
	template <
		typename Real, 
		typename A_ConstIterator, 
		typename B_ConstIterator>
	Real peakSignalToNoise(
		const ForwardRange<A_ConstIterator>& aSet,
		const ForwardRange<B_ConstIterator>& bSet,
		const PASTEL_NO_DEDUCTION(Real)& maxValue);

	//! Computes peak-signal-to-noise-ratio (PSNR) between sequences.
	/*!
	This is a convenience function that calls:
	peakSignalToNoise<Real>(aSet, bSet, 1);
	See the documentation for that function.
	*/
	template <
		typename Real, 
		typename A_ConstIterator, 
		typename B_ConstIterator>
	Real peakSignalToNoise(
		const ForwardRange<A_ConstIterator>& aSet,
		const ForwardRange<B_ConstIterator>& bSet);

}

#include "pastel/math/statistics.hpp"

#endif
