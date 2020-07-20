// Description: Functions to retrieve Matlab function arguments

#ifndef PASTELMATLAB_MATLAB_ARGUMENT_H
#define PASTELMATLAB_MATLAB_ARGUMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/sequence/copy_n.h"

#include "pastel/math/matrix.h"

#include "pastelmatlab/matlab_mex.h"
#include "pastelmatlab/matlab_class_ids.h"
#include "pastelmatlab/matlab_matrix.h"

namespace Pastel
{

	//! Creates a Matlab scalar.
	template <typename Type>
	Type* matlabCreateScalar(
		mxArray*& output)
	{
		output = mxCreateNumericMatrix(1, 1,
			typeToMatlabClassId<Type>(), mxREAL);

		return (Type*)mxGetData(output);
	}

	//! Creates a Matlab numeric array.
	/*!
	Preconditions:
	allGreaterEqual(extent, 0)
	*/
	template <typename Type, int M = Dynamic, int N = Dynamic>
	MatrixView<Type, M, N> matlabCreateMatrix(
		integer rows, integer cols,
		mxArray*& output)
	{
		ENSURE_OP(cols, >=, 0);
		ENSURE_OP(rows, >=, 0);

		output = mxCreateNumericMatrix(rows, cols, 
			typeToMatlabClassId<Type>(), mxREAL);
		
		return matlabAsMatrixView<Type, M, N>(output);
	}

	template <typename Type, int M = Dynamic>
	ColMatrixView<Type, M> matlabCreateColMatrix(
		integer rows, mxArray*& output)
	{
		return matlabCreateMatrix<Type, M, 1>(rows, 1, output);
	}

	template <
		typename To_Type,
		typename From_Type, int M, int N>
	MatrixView<To_Type, M, N> matlabCreateMatrix(
		const MatrixView<From_Type, M, N>& from,
		mxArray*& output)
	{
		MatrixView<To_Type, M, N> to = matlabCreateMatrix<To_Type>(
			from.rows(), from.cols(), output);

		std::copy(from.data(), from.data() + from.size(), to.data());

		return to;
	}

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
	Type matlabAsScalar(const mxArray* input, integer index = 0)
	{
		ENSURE(mxIsNumeric(input));

		integer n = mxGetNumberOfElements(input);

		ENSURE_OP(index, >=, 0);
		ENSURE_OP(index, <, n);

		Type result = 0;
		switch(mxGetClassID(input))
		{
		case mxSINGLE_CLASS:
			result = *((real32*)mxGetData(input) + index);
			break;
		case mxDOUBLE_CLASS:
			result = *((real64*)mxGetData(input) + index);
			break;
		case mxINT8_CLASS:
			result = *((int8*)mxGetData(input) + index);
			break;
		case mxUINT8_CLASS:
			result = *((uint8*)mxGetData(input) + index);
			break;
		case mxINT16_CLASS:
			result = *((int16*)mxGetData(input) + index);
			break;
		case mxUINT16_CLASS:
			result = *((uint16*)mxGetData(input) + index);
			break;
		case mxINT32_CLASS:
			result = *((int32*)mxGetData(input) + index);
			break;
		case mxUINT32_CLASS:
			result = *((uint32*)mxGetData(input) + index);
			break;
		case mxINT64_CLASS:
			result = *((int64*)mxGetData(input) + index);
			break;
		case mxUINT64_CLASS:
			result = *((uint64*)mxGetData(input) + index);
			break;
		default:
			// This should not be possible, since
			// the above covers all numeric types.
			{
				bool reachedHere = true;
				ENSURE(!reachedHere);
			}
			break;
		};

		return result;
	}

	//! Retrieves a copy of a string.
	/*!
	Preconditions:
	mxIsChar(input)
	*/
	inline std::string matlabAsString(const mxArray* input)
	{
		ENSURE(mxIsChar(input));

		char* text = mxArrayToString(input);
		std::string result(text);
		mxFree(text);

		return result;
	}

	namespace MatlabStringAsEnum_
	{

		template <
			typename Type,
			typename... ArgumentSet>
		Type matlabStringAsEnum(
			const std::string input)
		{
			bool unknownEnum = true;
			ENSURE(!unknownEnum);
			return Type();
		}

		template <
			typename Type,
			typename... ArgumentSet>
		Type matlabStringAsEnum(
			const std::string input,
			const std::string& key,
			NoDeduction<Type> value,
			ArgumentSet&&... argumentSet)
		{
			if (input == key)
			{
				return value;
			}
			
			return matlabStringAsEnum<Type>(input, 
				std::forward<ArgumentSet>(argumentSet)...);
		}

	}

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
		ArgumentSet&&... argumentSet)
	{
		return MatlabStringAsEnum_::matlabStringAsEnum<Type>(
			matlabAsString(input),
			std::forward<ArgumentSet>(argumentSet)...);
	}

	//! Retrieves a reference to a dreal matrix.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type, int M = Dynamic, int N = Dynamic>
	MatrixView<Type, M, N> matlabAsMatrixView(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));
		ENSURE_OP(typeToMatlabClassId<Type>(), ==, mxGetClassID(that));

		integer m = mxGetM(that);
		integer n = mxGetN(that);

		return MatrixView<Type, M, N>((Type*)mxGetData(that), m, n);
	}

	template <typename Type, int M = Dynamic>
	ColMatrixView<Type, M> matlabAsColMatrixView(
		const mxArray* that)
	{
		return matlabAsMatrixView<Type, M, 1>(that);
	}


	//! Reports all dreal arrays in a cell-array.
	/*!
	Preconditions:
	mxIsCell(cellArray)
	mxGetClassID(cell) == typeToMatlabClassId<Type>() 

	returns:
	The number of cells in the array.

	report:
	A reporter for MatrixView<Type>.

	Note: The elements are reported in Matlab's linearized
	order.
	*/
	template <typename Type, typename Array_Output>
	integer matlabGetArrays(
		const mxArray* cellArray,
		Array_Output report)
	{
		ENSURE(mxIsCell(cellArray));

		integer n = 
			mxGetNumberOfElements(cellArray);

		for (integer i = 0;i < n;++i)
		{
			const mxArray* cell = mxGetCell(cellArray, i);
			report(matlabAsMatrix<Type>(cell));
		}

		return n;
	}

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
		integer offset = 0)
	{
		ENSURE(mxIsNumeric(input));
		ENSURE_OP(offset, >=, 0);

		integer n = 
			mxGetNumberOfElements(input);
		if (offset >= n)
		{
			return n;
		}

		integer m = n - offset;

		switch(mxGetClassID(input))
		{
		case mxSINGLE_CLASS:
			copy_n((real32*)mxGetData(input) + offset, m, output);
			break;
		case mxDOUBLE_CLASS:
			copy_n((real64*)mxGetData(input) + offset, m, output);
			break;
		case mxINT8_CLASS:
			copy_n((int8*)mxGetData(input) + offset, m, output);
			break;
		case mxUINT8_CLASS:
			copy_n((uint8*)mxGetData(input) + offset, m, output);
			break;
		case mxINT16_CLASS:
			copy_n((int16*)mxGetData(input) + offset, m, output);
			break;
		case mxUINT16_CLASS:
			copy_n((uint16*)mxGetData(input) + offset, m, output);
			break;
		case mxINT32_CLASS:
			copy_n((int32*)mxGetData(input) + offset, m, output);
			break;
		case mxUINT32_CLASS:
			copy_n((uint32*)mxGetData(input) + offset, m, output);
			break;
		case mxINT64_CLASS:
			copy_n((int64*)mxGetData(input) + offset, m, output);
			break;
		case mxUINT64_CLASS:
			copy_n((uint64*)mxGetData(input) + offset, m, output);
			break;
		default:
			// This should not be possible, since
			// the above covers all numeric types.
			ENSURE(false);
			break;
		}
		
		return n;
	}

	//! Retrieves a reference to a dreal matrix.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	MatlabMatrix<Type> matlabAsMatrix(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));
		return MatlabMatrix<Type>(that);
	}

	//! Retrieves a reference to a dreal matrix.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	MatlabMatrix<Type, Dynamic, 1> matlabAsColMatrix(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));
		return MatlabMatrix<Type, Dynamic, 1>(that);
	}

	//! Retrieves a reference to a dreal matrix.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	MatlabMatrix<Type> matlabAsVectorizedMatrix(const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));
		return MatlabMatrix<Type>(that, 1, mxGetNumberOfElements(that));
	}

	//! Reports all dreal arrays in a cell-array.
	/*!
	Preconditions:
	mxIsCell(cellArray)
	mxGetClassID(cell) == typeToMatlabClassId<Real>() 

	returns:
	A random acccess range of MatlabMatrix's.

	Note: The elements are reported in Matlab's linearized
	order.
	*/
	template <typename Real>
	ranges::random_access_range auto matlabAsMatrixRange(const mxArray* cellArray)
	{
		ENSURE(mxIsCell(cellArray));

		return ranges::views::transform(
			ranges::views::ints((integer)0, (integer)mxGetNumberOfElements(cellArray)),
			[cellArray](integer i) {
				const mxArray* cell = mxGetCell(cellArray, i);
				return matlabAsMatrix<Real>(cell);
			}
		);
	}

	//! Retrieves an array of signals.
    template <typename Real>
	inline Array<MatlabMatrix<Real>> matlabAsMatrixArray(
		const mxArray* cellArray)
	{
		ENSURE(mxIsCell(cellArray));

		integer signals = mxGetM(cellArray);
		integer trials = mxGetN(cellArray);
		
		Array<MatlabMatrix<Real>> signalSet(
			Vector2i(trials, signals));

		for (integer y = 0;y < signals;++y)
		{
			for (integer x = 0;x < trials;++x)
			{
				const mxArray* signal = 
					mxGetCell(cellArray, signals * x + y);

				matlabAsMatrix<Real>(signal).swap(signalSet(x, y));
			}
		}

		return signalSet;
	}

}

#endif
