// Description: Discrete Haar transform

#ifndef PASTEL_HAAR_TRANSFORM_H
#define PASTEL_HAAR_TRANSFORM_H

#include "pastel/sys/forwardrange.h"

namespace Pastel
{

	//! Computes a Haar transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void haar(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes a Haar transform.
	/*!
	This is a convenience function that calls
	haar(input, input.begin()).
	See the documentation for that function.
	*/
	template <typename Real_ConstIterator>
	void haar(
		const ForwardRange<Real_ConstIterator>& input);

	PASTEL_RANGE_ALGORITHM(haar, Haar);

	//! Computes an inverse Haar transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseHaar(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an inverse Haar transform.
	/*!
	This is a convenience function that calls
	inverseHaar(input, input.begin()).
	See the documentation for that function.
	*/
	template <typename Real_ConstIterator>
	void inverseHaar(
		const ForwardRange<Real_ConstIterator>& input);

	PASTEL_RANGE_ALGORITHM(inverseHaar, InverseHaar);

	//! Computes an orthogonal Haar transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalHaar(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an orthogonal Haar transform.
	/*!
	This is a convenience function that calls
	orthogonalHaar(input, input.begin()).
	See the documentation for that function.
	*/
	template <typename Real_ConstIterator>
	void orthogonalHaar(
		const ForwardRange<Real_ConstIterator>& input);

	PASTEL_RANGE_ALGORITHM(orthogonalHaar, OrthogonalHaar);

	//! Computes an inverse orthogonal Haar transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalHaar(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an inverse orthogonal Haar transform.
	/*!
	This is a convenience function that calls
	inverseOrthogonalHaar(input, input.begin()).
	See the documentation for that function.
	*/
	template <typename Real_ConstIterator>
	void inverseOrthogonalHaar(
		const ForwardRange<Real_ConstIterator>& input);

	PASTEL_RANGE_ALGORITHM(inverseOrthogonalHaar, InverseOrthogonalHaar);

}

#include "pastel/dsp/haar_transform.hpp"

#endif
