// Description: Statistics from samples
// Detail: pointSetMean, pointSetVariance, etc.
// Documentation: common.txt

#ifndef PASTEL_STATISTICS_H
#define PASTEL_STATISTICS_H

#include "pastel/sys/vector.h"
#include "pastel/sys/iteratorrange.h"
#include "pastel/sys/pointpolicy_concept.h"

#include <boost/iterator/iterator_traits.hpp>

#include <vector>

namespace Pastel
{

	//! Returns the mean of a number set.
	template <
		typename Real,
		typename Real_ConstIterator>
	Real numberMean(
		const ForwardRange<Real_ConstIterator>& inputSet);

	//! Returns the variance of a number set.
	template <
		typename Real,
		typename Real_ConstIterator>
	Real numberVariance(
		const ForwardRange<Real_ConstIterator>& inputSet,
		const PASTEL_NO_DEDUCTION(Real)& mean,
		bool biased = true);

	//! Returns the covariance between number sets.
	template <
		typename Real,
		typename A_Real_ConstIterator,
		typename B_Real_ConstIterator>
	Real numberCovariance(
		const ForwardRange<A_Real_ConstIterator>& aSet,
		const PASTEL_NO_DEDUCTION(Real)& aMean,
		const ForwardRange<B_Real_ConstIterator>& bSet,
		const PASTEL_NO_DEDUCTION(Real)& bMean,
		bool biased = true);

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
