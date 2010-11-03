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

	//! Computes an orthogonal discrete fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void dftOrthogonal(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an inverse discrete fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void inverseDft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an inverse orthogonal discrete fourier transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void inverseDftOrthogonal(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

}

#include "pastel/dsp/discretefourier.hpp"

#endif
