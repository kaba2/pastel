#ifndef PASTELGFX_DISCRETEFOURIER_H
#define PASTELGFX_DISCRETEFOURIER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/memoryview.h"
#include "pastel/sys/lineararray.h"
#include "pastel/math/complex.h"

namespace Pastel
{

	//! Computes a discrete fourier transformation of the input data.

	template <
		typename InputView,
		typename OutputView>
	void discreteFourier(
		const InputView& input,
		OutputView& output);

	//! Computes a discrete fourier inverse transformation of the input data.

	template <
		typename InputView,
		typename OutputView>
	void discreteFourierInverse(
		const InputView& input,
		OutputView& output);

	//! Computes a discrete fourier transformation of the input data.

	template <
		typename InputView,
		typename OutputView>
	void discreteFourier2(
		const InputView& input,
		OutputView& output);

	//! Computes a discrete fourier transformation of the input data.

	template <
		typename InputView,
		typename OutputView>
	void discreteFourier2Horizontal(
		const InputView& input,
		OutputView& output);

	//! Computes a discrete fourier inverse transformation of the input data.

	template <
		typename InputView,
		typename OutputView>
	void discreteFourierInverse2(
		const InputView& input,
		OutputView& output);

	//! Computes a discrete fourier inverse transformation of the input data.

	template <
		typename InputView,
		typename OutputView>
	void discreteFourierInverse2Horizontal(
		const InputView& input,
		OutputView& output);

}

#include "pastel/gfx/discretefourier.hpp"

#endif
