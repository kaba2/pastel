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
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void dct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an inverse discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an orthogonal discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an inverse orthogonal discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes a complex discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void complexDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an inverse complex discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseComplexDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes a unitary discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void unitaryDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

	//! Computes an inverse unitary discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseUnitaryDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output);

}

#include "pastel/dsp/discretecosine.hpp"

#endif
