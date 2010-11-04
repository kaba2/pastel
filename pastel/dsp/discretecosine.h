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

	//! Computes a discrete cosine transform.
	/*!
	This is a convenience function that calls:
	dct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Iterator>
	void dct(const ForwardRange<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(dct, Dct);

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

	//! Computes an inverse discrete cosine transform.
	/*!
	This is a convenience function that calls:
	inverseDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Iterator>
	void inverseDct(const ForwardRange<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseDct, InverseDct);

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

	//! Computes an orthogonal discrete cosine transform.
	/*!
	This is a convenience function that calls:
	orthogonalDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Iterator>
	void orthogonalDct(const ForwardRange<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(orthogonalDct, OrthogonalDct);

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

	//! Computes an orthogonal discrete cosine transform.
	/*!
	This is a convenience function that calls:
	inverseOrthogonalDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Iterator>
	void inverseOrthogonalDct(const ForwardRange<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseOrthogonalDct, InverseOrthogonalDct);

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

	//! Computes a complex discrete cosine transform.
	/*!
	This is a convenience function that calls:
	complexDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Iterator>
	void complexDct(const ForwardRange<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(complexDct, ComplexDct);

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

	//! Computes an inverse complex discrete cosine transform.
	/*!
	This is a convenience function that calls:
	inverseComplexDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Iterator>
	void inverseComplexDct(const ForwardRange<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseComplexDct, InverseComplexDct);

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

	//! Computes a unitary discrete cosine transform.
	/*!
	This is a convenience function that calls:
	unitaryDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Iterator>
	void unitaryDct(const ForwardRange<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(unitaryDct, UnitaryDct);

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

	//! Computes an inverse unitary discrete cosine transform.
	/*!
	This is a convenience function that calls:
	inverseUnitaryDct(inputOutput, inputOutput.begin())

	See the documentation for that function.
	*/
	template <typename Real_Iterator>
	void inverseUnitaryDct(const ForwardRange<Real_Iterator>& inputOutput);

	PASTEL_RANGE_ALGORITHM(inverseUnitaryDct, InverseUnitaryDct);

}

#include "pastel/dsp/discretecosine.hpp"

#endif
