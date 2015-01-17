// Description: Peak signal-to-noise ratio between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_PEAK_SIGNAL_TO_NOISE_H
#define PASTELMATH_SCALAR_PEAK_SIGNAL_TO_NOISE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/input/input_concept.h"

namespace Pastel
{

	//! Returns the peak-signal-to-noise-ratio between sequences.
	template <
		typename Real, 
		typename A_Real_Input, 
		typename B_Real_Input>
	Real peakSignalToNoise(
		A_Real_Input aSet,
		B_Real_Input bSet,
		const NoDeduction<Real>& maxValue);

	//! Returns the peak-signal-to-noise-ratio between sequences.
	/*!
	This is a convenience function which calls
	peakSignalToNoise<Real>(aSet, bSet, 1).
	*/
	template <
		typename Real, 
		typename A_Real_Input, 
		typename B_Real_Input>
	Real peakSignalToNoise(
		A_Real_Input aSet,
		B_Real_Input bSet);
	
}

#include "pastel/math/scalar_peak_signal_to_noise.hpp"

#endif
