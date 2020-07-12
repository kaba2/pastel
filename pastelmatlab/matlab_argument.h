// Description: Functions to retrieve Matlab function arguments

#ifndef PASTELMATLAB_MATLAB_ARGUMENT_H
#define PASTELMATLAB_MATLAB_ARGUMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/sequence/copy_n.h"

#include "pastel/math/matrix.h"

#include "pastelmatlab/matlab_mex.h"
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

	//! Returns the Matlab class-id corresponding to 'Type'.
	template <typename Type>
	mxClassID typeToMatlabClassId()
	{
		PASTEL_STATIC_ASSERT(
			std::is_pointer<Type>::value ||
			std::is_integral<Type>::value || 
			std::is_floating_point<Type>::value)

		if (std::is_pointer<Type>::value)
		{
			switch(sizeof(Type))
			{
			case 8:
				return mxUINT64_CLASS;
			case 4:
				return mxUINT32_CLASS;
			};
		}

		// Note: the mxCHAR_CLASS coincides in type with
		// mxINT8_CLASS. Therefore, we leave it out here:
		// the important thing is to support the 
		// numeric types. The mxCHAR_CLASS is handled
		// specially for strings.

		if (std::is_integral<Type>::value)
		{
			if (std::is_signed<Type>::value)
			{
				switch(sizeof(Type))
				{
				case 8:
					return mxINT64_CLASS;
				case 4:
					return mxINT32_CLASS;
				case 2:
					return mxINT16_CLASS;
				case 1:
					return mxINT8_CLASS;
				};
			}
			else
			{
				switch(sizeof(Type))
				{
				case 8:
					return mxUINT64_CLASS;
				case 4:
					return mxUINT32_CLASS;
				case 2:
					return mxUINT16_CLASS;
				case 1:
					return mxUINT8_CLASS;
				};
			}
		}

		if (std::is_floating_point<Type>::value)
		{
			if (std::is_same<float, typename std::remove_cv<Type>::type>::value)
			{
				return mxSINGLE_CLASS;
			}
			if (std::is_same<double, typename std::remove_cv<Type>::type>::value)
			{
				return mxDOUBLE_CLASS;
			}
		}

		bool reachedHere = true;
		ENSURE(!reachedHere);

		return mxUNKNOWN_CLASS;
	}

}

#endif
