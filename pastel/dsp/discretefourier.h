// Description: Discrete fourier transform

#ifndef PASTEL_DISCRETEFOURIER_H
#define PASTEL_DISCRETEFOURIER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/memoryview.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view.h"

#include "pastel/sys/forwardrange.h"

#include <complex>

namespace Pastel
{

	//! Computes a discrete fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void dft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes a discrete fourier transform.
	/*!
	This is a convenience function that calls:
	dft(inputOutput, inputOutput.begin());
	See the documentation for that function.
	*/
	template <typename Complex_Iterator>
	void dft(
		const ForwardRange<Complex_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(dft, Dft);

	//! Computes a unitary discrete fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void unitaryDft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes a unitary discrete fourier transform.
	/*!
	This is a convenience function that calls:
	unitaryDft(inputOutput, inputOutput.begin());
	See the documentation for that function.
	*/
	template <typename Complex_Iterator>
	void unitaryDft(
		const ForwardRange<Complex_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(unitaryDft, UnitaryDft);

	//! Computes an inverse discrete fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void inverseDft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an inverse discrete fourier transform.
	/*!
	This is a convenience function that calls:
	inverseDft(inputOutput, inputOutput.begin());
	See the documentation for that function.
	*/
	template <typename Complex_Iterator>
	void inverseDft(
		const ForwardRange<Complex_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseDft, InverseDft);

	//! Computes an inverse unitary discrete fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void inverseUnitaryDft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an inverse unitary discrete fourier transform.
	/*!
	This is a convenience function that calls:
	inverseUnitaryDft(inputOutput, inputOutput.begin());
	See the documentation for that function.
	*/
	template <typename Complex_Iterator>
	void inverseUnitaryDft(
		const ForwardRange<Complex_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseUnitaryDft, InverseUnitaryDft);

}

#include "pastel/dsp/discretefourier.hpp"

#endif
