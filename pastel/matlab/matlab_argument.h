// Description: Functions to retrieve Matlab function arguments

#ifndef PASTEL_MATLAB_ARGUMENT_H
#define PASTEL_MATLAB_ARGUMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"

#include <boost/shared_ptr.hpp>

// This is the Matlab's mex header file.
#include "mex.h"

namespace Pastel
{

	typedef boost::shared_ptr<Array<real> > 
		RealArrayPtr;

	//! Retrieves a reference to a real array.
	RealArrayPtr realArray(
		const mxArray* that);

	//! Reports all real arrays in a cell-array.
	/*!
	Returns:
	The number of cells in the array.

	Note: The elements are reported in Matlab's linearized
	order.
	*/
	template <typename RealArrayPtr_Iterator>
	integer asRealArrays(
		const mxArray* cellArray,
		RealArrayPtr_Iterator output);

	//! Reports all elements in a real-array as integers.
	/*!
	Returns:
	The number of elements in the array.
	*/
	template <typename Integer_Iterator>
	integer getIntegers(
		const mxArray* input,
		Integer_Iterator output);

	//! Reports all elements in a real-array as reals.
	/*!
	Returns:
	The number of elements in the array.

	Note: The elements are reported in Matlab's linearized
	order.
	*/
	template <typename Real_Iterator>
	integer getReals(
		const mxArray* input,
		Real_Iterator output);

	//! Retrieves a copy of an integer.
	integer asInteger(const mxArray* input);

	//! Retrieves a copy of a real number.
	real asReal(const mxArray* input);

	//! Retrieves a copy of a string.
	std::string asString(const mxArray* input);

}

#include "pastel/matlab/matlab_argument.hpp"

#endif
