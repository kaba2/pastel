// Description: Discrete Haar transform

#ifndef PASTELDSP_HAAR_TRANSFORM_H
#define PASTELDSP_HAAR_TRANSFORM_H

#include "pastel/sys/range.h"

namespace Pastel
{

	//! Computes a Haar transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void haar(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes a Haar transform.
	/*!
	This is a convenience function that calls
	haar(inputOutput, inputOutput).
	See the documentation for that function.
	*/
	template <typename Real_Range>
	void haar(
		const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(haar, Haar);

	//! Computes an inverse Haar transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseHaar(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an inverse Haar transform.
	/*!
	This is a convenience function that calls
	inverseHaar(inputOutput, inputOutput).
	See the documentation for that function.
	*/
	template <typename Real_Range>
	void inverseHaar(
		const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseHaar, InverseHaar);

	//! Computes an orthogonal Haar transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void orthogonalHaar(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an orthogonal Haar transform.
	/*!
	This is a convenience function that calls
	orthogonalHaar(inputOutput, inputOutput).
	See the documentation for that function.
	*/
	template <typename Real_Range>
	void orthogonalHaar(
		const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(orthogonalHaar, OrthogonalHaar);

	//! Computes an inverse orthogonal Haar transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseOrthogonalHaar(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an inverse orthogonal Haar transform.
	/*!
	This is a convenience function that calls
	inverseOrthogonalHaar(inputOutput, inputOutput).
	See the documentation for that function.
	*/
	template <typename Real_ConstRange>
	void inverseOrthogonalHaar(
		const Real_ConstRange& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseOrthogonalHaar, InverseOrthogonalHaar);

}

#include "pastel/dsp/haar_transform.hpp"

#endif
