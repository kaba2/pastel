// Description: Functions to retrieve Matlab function arguments

#ifndef PASTELMATLAB_MATLAB_ARGUMENT_H
#define PASTELMATLAB_MATLAB_ARGUMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/ensure.h"

#include "pastel/math/matrix.h"

#include "pastelmatlab/matlab_mex.h"
#include "pastelmatlab/matlab_class_ids.h"
#include "pastelmatlab/matlab_scalar.h"
#include "pastelmatlab/matlab_matrix.h"

namespace Pastel
{

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

}

#endif
