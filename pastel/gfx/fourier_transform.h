// Description: Discrete Fourier transform

#ifndef PASTELGFX_FOURIER_TRANSFORM_H
#define PASTELGFX_FOURIER_TRANSFORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view/view.h"

#include "pastel/sys/range.h"

#include <complex>

namespace Pastel
{

	//! Computes a discrete Fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void dft(
		const Complex_ConstRange& input,
		const Complex_Range& output);

	//! Computes a discrete Fourier transform.
	/*!
	This is a convenience function that calls:
	dft(inputOutput, inputOutput);
	See the documentation for that function.
	*/
	template <typename Complex_Range>
	void dft(
		const Complex_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(dft, Dft);

	//! Computes a unitary discrete Fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void unitaryDft(
		const Complex_ConstRange& input,
		const Complex_Range& output);

	//! Computes a unitary discrete Fourier transform.
	/*!
	This is a convenience function that calls:
	unitaryDft(inputOutput, inputOutput);
	See the documentation for that function.
	*/
	template <typename Complex_Range>
	void unitaryDft(
		const Complex_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(unitaryDft, UnitaryDft);

	//! Computes an inverse discrete Fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void inverseDft(
		const Complex_ConstRange& input,
		const Complex_Range& output);

	//! Computes an inverse discrete Fourier transform.
	/*!
	This is a convenience function that calls:
	inverseDft(inputOutput, inputOutput);
	See the documentation for that function.
	*/
	template <typename Complex_Range>
	void inverseDft(
		const Complex_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseDft, InverseDft);

	//! Computes an inverse unitary discrete Fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void inverseUnitaryDft(
		const Complex_ConstRange& input,
		const Complex_Range& output);

	//! Computes an inverse unitary discrete Fourier transform.
	/*!
	This is a convenience function that calls:
	inverseUnitaryDft(inputOutput, inputOutput);
	See the documentation for that function.
	*/
	template <typename Complex_Range>
	void inverseUnitaryDft(
		const Complex_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseUnitaryDft, InverseUnitaryDft);

}

#include "pastel/gfx/fourier_transform.hpp"

#endif
