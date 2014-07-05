#ifndef PASTELMATH_SCALAR_PEAK_SIGNAL_TO_NOISE_HPP
#define PASTELMATH_SCALAR_PEAK_SIGNAL_TO_NOISE_HPP

#include "pastel/math/scalar_peak_signal_to_noise.h"
#include "pastel/math/scalar_mean_square_error.h"

#include <cmath>

namespace Pastel
{

	template <
		typename Real, 
		typename A_Real_Input, 
		typename B_Real_Input>
	Real peakSignalToNoise(
		A_Real_Input aSet,
		B_Real_Input bSet,
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		return 10 * std::log10(
			square(maxValue) / scalarMeanSquareError<Real>(aSet, bSet));
	}

	template <
		typename Real, 
		typename A_Real_Input, 
		typename B_Real_Input>
	Real peakSignalToNoise(
		A_Real_Input aSet,
		B_Real_Input bSet)
	{
		return Pastel::peakSignalToNoise<Real>(
			aSet, bSet, 1);
	}

}

#endif
