#ifndef PASTELMATLAB_MATLAB_MATRIX_H
#define PASTELMATLAB_MATLAB_MATRIX_H

#include "pastel/math/matrix.h"
#include "pastelmatlab/matlab_mex.h"
#include "pastelmatlab/matlab_argument.h"
#include "pastel/sys/array.h"

namespace Pastel {

    template <typename Real, int M = Dynamic, int N = Dynamic>
    class MatlabMatrix {
    public:
        MatlabMatrix()
        : data_()
        , view_() {
        }

        MatlabMatrix(integer rows, integer cols)
        : data_(rows, cols)
        , view_(data_.data(), rows, cols) {
        }

        MatlabMatrix(const mxArray* that)
        : MatlabMatrix(that, mxGetM(that), mxGetN(that))
        {
        }

        MatlabMatrix(const mxArray* that, integer rows, integer cols)
        : MatlabMatrix()
        {
            ENSURE(mxIsNumeric(that));
            ENSURE_OP(rows, >=, 0);
            ENSURE_OP(cols, >=, 0);

            integer m = mxGetM(that);
            integer n = mxGetN(that);
            Real* data = (Real*)mxGetData(that);
            
            ENSURE_OP(rows * cols, ==, m * n);

            if (typeToMatlabClassId<Real>() != mxGetClassID(that)) {
                // Types do not match; copy the data.
                MatrixData<Real>(rows, cols).swap(data_);
                matlabGetScalars(that, std::begin(data_.view().range()));
                data = data_.data();
            }

            view_ = MatrixView<Real, M, N>(data, rows, cols);
        }

        MatlabMatrix(const MatlabMatrix& that)
        : data_(that.data_)
        , view_(that.view_) {
            if (that.hasMemory()) {
                view_ = MatrixView<Real, M, N>(data_.data(), that.rows(), that.cols());
            }
        }

        MatlabMatrix(MatlabMatrix&& that)
        : MatlabMatrix() {
            swap(that);
        }

        MatlabMatrix& operator=(const MatlabMatrix& that) = delete;
        
        MatlabMatrix& operator=(MatlabMatrix&& that) {
            MatlabMatrix(std::move(that)).swap(*this);
            return *this;
        }

        bool hasMemory() const {
            return data_.data() == view_.data();
        }

        void swap(MatlabMatrix& that) {
            data_.swap(that.data_);
            view_.swap(that.view_);
        }

        integer rows() const {
            return view_.rows();
        }

        integer cols() const {
            return view_.cols();
        }

        integer size() const {
            return view_.size();
        }

        bool isEmpty() const {
            return view_.isEmpty();
        }

        Real* data() const {
            return (Real*)view_.data();
        }

        const MatrixView<Real, M, N>& view() const {
            return view_;
        }

    private:
        MatrixData<Real> data_;
        MatrixView<Real, M, N> view_;
    };

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

}

#endif
