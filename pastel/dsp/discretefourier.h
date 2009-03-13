#ifndef PASTELDSP_DISCRETEFOURIER_H
#define PASTELDSP_DISCRETEFOURIER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/memoryview.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view.h"

#include "pastel/math/complex.h"

namespace Pastel
{

	//! Computes a discrete fourier transformation.

	template <typename Real, typename Input_View,
		typename Output_View>
		void discreteFourier(
		const ConstView<1, Complex<Real>, Input_View>& input,
		const View<1, Complex<Real>, Output_View>& output);

	//! Computes a discrete fourier inverse transformation.

	template <typename Real, typename Input_View,
		typename Output_View>
		void discreteFourierInverse(
		const ConstView<1, Complex<Real>, Input_View>& input,
		const View<1, Complex<Real>, Output_View>& output);

	//! Computes a discrete fourier transformation.
	/*!
	This function simply uses the 1d discrete fourier
	transformation for each row on each axis.
	*/

	template <
		int N,
		typename Real,
		typename Input_View,
		typename Output_View>
		typename boost::enable_if_c<(N > 1), void>::type
		discreteFourier(
		const ConstView<N, Complex<Real>, Input_View>& input,
		const View<N, Complex<Real>, Output_View>& output);

	//! Computes a discrete fourier inverse transformation.
	/*!
	This function simply uses the 1d discrete fourier
	inverse transformation for each row on each axis.
	*/

	template <
		int N,
		typename Real,
		typename Input_View,
		typename Output_View>
		typename boost::enable_if_c<(N > 1), void>::type
		discreteFourierInverse(
		const ConstView<N, Complex<Real>, Input_View>& input,
		const View<N, Complex<Real>, Output_View>& output);

}

#include "pastel/dsp/discretefourier.hpp"

#endif
