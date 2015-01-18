// Description: Discrete cosine transform

#ifndef PASTELGFX_COSINE_TRANSFORM_H
#define PASTELGFX_COSINE_TRANSFORM_H

#include "pastel/sys/range.h"

namespace Pastel
{

	//! Computes a discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void dct(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes a discrete cosine transform.
	/*!
	This is a convenience function that calls:
	dct(inputOutput, inputOutput)

	See the documentation for that function.
	*/
	template <typename Real_Range>
	void dct(const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(dct, Dct);

	//! Computes an inverse discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseDct(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an inverse discrete cosine transform.
	/*!
	This is a convenience function that calls:
	inverseDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Range>
	void inverseDct(const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseDct, InverseDct);

	//! Computes an orthogonal discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void orthogonalDct(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an orthogonal discrete cosine transform.
	/*!
	This is a convenience function that calls:
	orthogonalDct(inputOutput, inputOutput)

	See the documentation for that function.
	*/
	template <typename Real_Range>
	void orthogonalDct(const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(orthogonalDct, OrthogonalDct);

	//! Computes an inverse orthogonal discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseOrthogonalDct(
		const Real_ConstRange& input,
		const Real_Range& output);

	//! Computes an orthogonal discrete cosine transform.
	/*!
	This is a convenience function that calls:
	inverseOrthogonalDct(inputOutput, inputOutput)

	See the documentation for that function.
	*/
	template <typename Real_Range>
	void inverseOrthogonalDct(
		const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseOrthogonalDct, InverseOrthogonalDct);

	//! Computes a complex discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void complexDct(
		const Complex_ConstRange& input,
		const Complex_Range& output);

	//! Computes a complex discrete cosine transform.
	/*!
	This is a convenience function that calls:
	complexDct(inputOutput, inputOutput)

	See the documentation for that function.
	*/
	template <typename Real_Range>
	void complexDct(const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(complexDct, ComplexDct);

	//! Computes an inverse complex discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void inverseComplexDct(
		const Complex_ConstRange& input,
		const Complex_Range& output);

	//! Computes an inverse complex discrete cosine transform.
	/*!
	This is a convenience function that calls:
	inverseComplexDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Range>
	void inverseComplexDct(const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseComplexDct, InverseComplexDct);

	//! Computes a unitary discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void unitaryDct(
		const Complex_ConstRange& input,
		const Complex_Range& output);

	//! Computes a unitary discrete cosine transform.
	/*!
	This is a convenience function that calls:
	unitaryDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Range>
	void unitaryDct(const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(unitaryDct, UnitaryDct);

	//! Computes an inverse unitary discrete cosine transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void inverseUnitaryDct(
		const Complex_ConstRange& input,
		Complex_Range output);

	//! Computes an inverse unitary discrete cosine transform.
	/*!
	This is a convenience function that calls:
	inverseUnitaryDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Range>
	void inverseUnitaryDct(const Real_Range& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseUnitaryDct, InverseUnitaryDct);

}

#include "pastel/gfx/cosine_transform.hpp"

#endif
