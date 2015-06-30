// Description: Peak signal-to-noise ratio between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_PEAK_SIGNAL_TO_NOISE_H
#define PASTELMATH_SCALAR_PEAK_SIGNAL_TO_NOISE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/set_concept.h"

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
		typename Real, 
		typename A_Real_Set, 
		typename B_Real_Set,
		typename... ArgumentSet,
		Requires<
			Models<Real, Real_Concept>,
			Models<A_Real_Set, Set_Concept>,
			Models<B_Real_Set, Set_Concept>
		> = 0>
	Real peakSignalToNoise(
		const A_Real_Set& aSet,
		const B_Real_Set& bSet,
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
