#ifndef PASTEL_POISSONDISKPATTERN_H
#define PASTEL_POISSONDISKPATTERN_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real, typename ReportFunctor, typename ConstSeedIterator>
	void poissonDiskPattern(
		const AlignedBox<N, Real>& window,
		const PASTEL_NO_DEDUCTION(Real)& minDistance,
		ReportFunctor& reportFunctor,
		const ConstSeedIterator& seedSetBegin,
		const ConstSeedIterator& seedSetEnd,
		integer maxRejections = 30);

	template <int N, typename Real, typename ReportFunctor>
	void poissonDiskPattern(
		const AlignedBox<N, Real>& window,
		const PASTEL_NO_DEDUCTION(Real)& minDistance,
		ReportFunctor& reportFunctor,
		integer maxRejections = 30);

}

#include "pastel/geometry/poissondiskpattern.hpp"

#endif
