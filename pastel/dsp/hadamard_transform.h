// Description: Discrete Hadamard transform

#ifndef PASTEL_HADAMARD_TRANSFORM_H
#define PASTEL_HADAMARD_TRANSFORM_H

#include "pastel/sys/iterator_range.h"

namespace Pastel
{

	//! Computes a Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void hadamard(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes a Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <typename Real_Iterator>
	void hadamard(
		const ForwardIterator_Range<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(hadamard, Hadamard);

	//! Computes an inverse Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseHadamard(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an inverse Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <typename Real_Iterator>
	void inverseHadamard(
		const ForwardIterator_Range<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseHadamard, InverseHadamard);

	//! Computes an orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalHadamard(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <typename Real_Iterator>
	void orthogonalHadamard(
		const ForwardIterator_Range<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(orthogonalHadamard, OrthogonalHadamard);

	//! Computes an inverse orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalHadamard(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output);

	//! Computes an inverse orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <typename Real_Iterator>
	void inverseOrthogonalHadamard(
		const ForwardIterator_Range<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseOrthogonalHadamard, InverseOrthogonalHadamard);

}

#include "pastel/dsp/hadamard_transform.hpp"

#endif
