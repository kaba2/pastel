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

	//! Retrieves a copy of a scalar number.
	/*!
	Preconditions:
	mxIsNumeric(input)

	Note: Conversion is done automatically
	from the type of 'input' to type 'Type'.
	*/
	template <typename Type>
	Type asScalar(const mxArray* input);

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
	boost::shared_ptr<Array<Type> > asArray(
		const mxArray* that);

	//! Reports all real arrays in a cell-array.
	/*!
	Preconditions:
	mxIsCell(cellArray)
	mxGetClassID(cell) == typeToMatlabClassId<Type>() 

	Returns:
	The number of cells in the array.

	Note: The elements are reported in Matlab's linearized
	order.
	*/
	template <typename Type, typename ArrayPtr_Iterator>
	integer getArrays(
		const mxArray* cellArray,
		ArrayPtr_Iterator output);

	//! Reports all elements in a numeric array.
	/*!
	Preconditions:
	mxIsNumeric(input)

	Returns:
	The number of elements in the array.

	Note: The elements are reported in Matlab's linearized
	order. Conversions are done automatically from the
	type of 'input' to the iterators value-type.
	*/
	template <typename Scalar_Iterator>
	integer getScalars(
		const mxArray* input,
		Scalar_Iterator output);

	//! Returns the Matlab class-id corresponding to 'Type'.
	template <typename Type>
	mxClassID typeToMatlabClassId();

	typedef boost::shared_ptr<Array<real> > 
		RealArrayPtr;

	typedef boost::shared_ptr<Array<real32> > 
		Real32ArrayPtr;

	typedef boost::shared_ptr<Array<real64> > 
		Real64ArrayPtr;

	typedef boost::shared_ptr<Array<int8> > 
		Int8ArrayPtr;

	typedef boost::shared_ptr<Array<uint8> > 
		UInt8ArrayPtr;

	typedef boost::shared_ptr<Array<int16> > 
		Int16ArrayPtr;

	typedef boost::shared_ptr<Array<uint16> > 
		UInt16ArrayPtr;

	typedef boost::shared_ptr<Array<int32> > 
		Int32ArrayPtr;

	typedef boost::shared_ptr<Array<uint32> > 
		UInt32ArrayPtr;

	typedef boost::shared_ptr<Array<int64> > 
		Int64ArrayPtr;

	typedef boost::shared_ptr<Array<uint64> > 
		UInt64ArrayPtr;

}

#include "pastel/matlab/matlab_argument.hpp"

#endif
