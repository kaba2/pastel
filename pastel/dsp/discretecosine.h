// Description: Discrete cosine transform

#ifndef PASTEL_DISCRETECOSINE_H
#define PASTEL_DISCRETECOSINE_H

#include "pastel/sys/forwardrange.h"

namespace Pastel
{

	//! Computes a discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void dct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an inverse discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an orthogonal discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void dctOrthogonal(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an inverse orthogonal discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseDctOrthogonal(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

}

#include "pastel/dsp/discretecosine.hpp"

#endif
