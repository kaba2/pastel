// Description: Functions to retrieve Matlab function arguments

#ifndef PASTELMATLAB_MATLAB_ARGUMENT_H
#define PASTELMATLAB_MATLAB_ARGUMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"
#include "pastel/sys/vector.h"

#include "pastel/matlab/matlab_mex.h"

namespace Pastel
{

	//! Creates a Matlab scalar.
	template <typename Type>
	Type* createScalar(
		mxArray*& output);

	//! Creates a Matlab numeric array.
	/*!
	Preconditions:
	allGreaterEqual(extent, 0)
	*/
	template <typename Type>
	Array<Type> createArray(
		const Vector2i& extent,
		mxArray*& output);

	//! Creates a Matlab numeric array.
	/*!
	This is a convenience function that calls
	createArray(Vector2i(width, height), output)
	*/
	template <typename Type>
	Array<Type> createArray(
		integer width, integer height,
		mxArray*& output);

	//! Retrieves a copy of a scalar number.
	/*!
	Preconditions:
	mxIsNumeric(input)
	index >= 0
	index < mxGetNumberOfElements(input)

	Note: Conversion is done automatically
	from the type of 'input' to type 'Type'.
	*/
	template <typename Type>
	Type asScalar(const mxArray* input,
		integer index = 0);

	//! Retrieves a copy of a string.
	/*!
	Preconditions:
	mxIsChar(input)
	*/
	std::string asString(const mxArray* input);

	//! Retrieves a reference to a real array.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	Array<Type> asArray(const mxArray* that);

	//! Retrieves a reference to a linearized real array.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	Array<Type> asLinearizedArray(const mxArray* that);

	//! Reports all real arrays in a cell-array.
	/*!
	Preconditions:
	mxIsCell(cellArray)
	mxGetClassID(cell) == typeToMatlabClassId<Type>() 

	returns:
	The number of cells in the array.

	report:
	A reporter for Array<Type>.

	Note: The elements are reported in Matlab's linearized
	order.
	*/
	template <typename Type, typename Array_Output>
	integer getArrays(
		const mxArray* cellArray,
		Array_Output report);

	//! Reports all elements in a numeric array.
	/*!
	Preconditions:
	mxIsNumeric(input)
	offset >= 0

	Offset:
	The index of the element from which
	to start.

	Returns:
	The number of elements in the array.

	Note: The elements are reported in Matlab's linearized
	order. Conversions are done automatically from the
	type of 'input' to the iterators value-type.
	*/
	template <typename Scalar_Iterator>
	integer getScalars(
		const mxArray* input,
		Scalar_Iterator output,
		integer offset = 0);

	//! Returns the Matlab class-id corresponding to 'Type'.
	template <typename Type>
	mxClassID typeToMatlabClassId();

}

#include "pastel/matlab/matlab_argument.hpp"

#endif
