// Description: Tools to generate poisson disk patterns

#ifndef PASTEL_POISSONDISKPATTERN_H
#define PASTEL_POISSONDISKPATTERN_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Generates an almost-maximal poisson disk pattern.
	/*!
	window:
	The rectangular region to fill with the pattern.

	minDistance:
	The minimum pairwise distance between pattern points.

	reportFunctor:
	A functor that is used to report the pattern points

	seedSetBegin, seedSetEnd:
	An iterator range describing a set of seed points
	which form the initial pattern.

	maxRejections:
	The maximal effort the algorithm makes to ensure
	that the pattern is maximal.
	*/
	template <int N, typename Real, typename ReportFunctor, typename ConstSeedIterator>
	void poissonDiskPattern(
		const AlignedBox<Real, N>& window,
		const PASTEL_NO_DEDUCTION(Real)& minDistance,
		ReportFunctor& reportFunctor,
		const ConstSeedIterator& seedSetBegin,
		const ConstSeedIterator& seedSetEnd,
		integer maxRejections = 30);

	//! Generates an almost-maximal poisson disk pattern.
	/*!
	This is a convenience function for the more general
	'poissonDiskPattern' function in case one does not want
	to use any seed points. See the documentation
	for the more general version.
	*/
	template <int N, typename Real, typename ReportFunctor>
	void poissonDiskPattern(
		const AlignedBox<Real, N>& window,
		const PASTEL_NO_DEDUCTION(Real)& minDistance,
		ReportFunctor& reportFunctor,
		integer maxRejections = 30);

}

#include "pastel/geometry/poissondiskpattern.hpp"

#endif
