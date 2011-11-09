// Description: Functions to retrieve Matlab function arguments

#ifndef PASTEL_MATLAB_ARGUMENT_H
#define PASTEL_MATLAB_ARGUMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"
#include "pastel/sys/vector.h"

#include "pastel/matlab/matlab_mex.h"

#include <boost/shared_ptr.hpp>

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
	boost::shared_ptr<Array<Type> > createArray(
		const Vector2i& extent,
		mxArray*& output);

	//! Creates a Matlab numeric array.
	/*!
	This is a convenience function that calls
	createArray(Vector2i(width, height), output)
	*/
	template <typename Type>
	boost::shared_ptr<Array<Type> > createArray(
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
	boost::shared_ptr<Array<Type> > asArray(
		const mxArray* that);

	//! Retrieves a reference to a linearized real array.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	boost::shared_ptr<Array<Type> > asLinearizedArray(
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

	typedef boost::shared_ptr<Array<integer> > 
		IntegerArrayPtr;

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
