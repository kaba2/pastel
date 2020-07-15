// Description: Peak signal-to-noise ratio between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_PEAK_SIGNAL_TO_NOISE_H
#define PASTELMATH_SCALAR_PEAK_SIGNAL_TO_NOISE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/range_concept.h"

// Implementation

#include "pastel/math/statistic/scalar_mean_square_error.h"

#include <cmath>

namespace Pastel
{

	//! Returns the peak-signal-to-noise-ratio between sequences.
	/*!
	Optional arguments
	------------------

	maxValue (Real : 1):
	The maximum value for the mean-square error.
	*/
	template <
		Real_Concept Real, 
		Range_Concept A_Real_Range, 
		Range_Concept B_Real_Range,
		typename... ArgumentSet
	>
	Real peakSignalToNoise(
		const A_Real_Range& aSet,
		const B_Real_Range& bSet,
		ArgumentSet&&... argumentSet)
	{
		Real maxValue = PASTEL_ARG_S(maxValue, Real(1));

		return 
			10 * std::log10(
				square(maxValue) / 
				scalarMeanSquareError<Real>(aSet, bSet)
			);
	}

}

#endif
