// Description: Functions to retrieve Matlab function arguments

#ifndef PASTELMATLAB_MATLAB_ARGUMENT_H
#define PASTELMATLAB_MATLAB_ARGUMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"
#include "pastel/sys/vector.h"

#include "pastel/matlab/matlab_mex.h"

#include <armadillo>

namespace Pastel
{

	//! Creates a Matlab scalar.
	template <typename Type>
	Type* matlabCreateScalar(
		mxArray*& output);

	//! Creates a Matlab numeric array.
	/*!
	Preconditions:
	allGreaterEqual(extent, 0)
	*/
	template <typename Type>
	Array<Type> matlabCreateArray(
		const Vector2i& extent,
		mxArray*& output);

	//! Creates a Matlab numeric array.
	/*!
	This is a convenience function that calls
	matlabCreateArray(Vector2i(width, height), output)
	*/
	template <typename Type>
	Array<Type> matlabCreateArray(
		integer width, integer height,
		mxArray*& output);

	//! Constructs a Matlab numeric array.
	template <
		typename To_Type,
		typename From_Type>
	Array<To_Type> matlabCreateArray(
		const arma::Mat<From_Type>& from,
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
	Type matlabAsScalar(const mxArray* input,
		integer index = 0);

	//! Retrieves a copy of a string.
	/*!
	Preconditions:
	mxIsChar(input)
	*/
	std::string matlabAsString(const mxArray* input);

	//! Retrieves a reference to a real matrix.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	arma::Mat<Type> matlabAsMatrix(const mxArray* that);

	//! Retrieves an enum given as strings.
	/*!
	input:
	A matlab array which contains the enum
	value as a string.

	argumentSet:
	Subsequent key-value arguments,
	with the key convertible to std::string,
	and the value with type 'Type'.

	returns:
	The value of the key which matches the
	string in 'input'.
	*/
	template <
		typename Type,
		typename... ArgumentSet>
	Type matlabStringAsEnum(
		const mxArray* input,
		ArgumentSet&&... argumentSet);

	//! Retrieves a reference to a real array.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	Array<Type> matlabAsArray(const mxArray* that);

	//! Retrieves a reference to a linearized real array.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	Array<Type> matlabAsLinearizedArray(const mxArray* that);

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
	integer matlabGetArrays(
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
	integer matlabGetScalars(
		const mxArray* input,
		Scalar_Iterator output,
		integer offset = 0);

	//! Returns the Matlab class-id corresponding to 'Type'.
	template <typename Type>
	mxClassID typeToMatlabClassId();

}

#include "pastel/matlab/matlab_argument.hpp"

#endif
