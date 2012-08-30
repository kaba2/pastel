// Description: Discrete Hadamard transform

#ifndef PASTELGFX_HADAMARD_TRANSFORM_H
#define PASTELGFX_HADAMARD_TRANSFORM_H

#include "pastel/sys/range.h"

namespace Pastel
{

	//! Computes a Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void hadamard(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes a Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <typename Real_Range>
	void hadamard(
		const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(hadamard, Hadamard);

	//! Computes an inverse Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseHadamard(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an inverse Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <typename Real_Range>
	void inverseHadamard(
		const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseHadamard, InverseHadamard);

	//! Computes an orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void orthogonalHadamard(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <typename Real_Range>
	void orthogonalHadamard(
		const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(orthogonalHadamard, OrthogonalHadamard);

	//! Computes an inverse orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseOrthogonalHadamard(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an inverse orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <typename Real_Range>
	void inverseOrthogonalHadamard(
		const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseOrthogonalHadamard, InverseOrthogonalHadamard);

}

#include "pastel/gfx/hadamard_transform.hpp"

#endif
