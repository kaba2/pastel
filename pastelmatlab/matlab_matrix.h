#ifndef PASTELMATLAB_MATLAB_MATRIX_H
#define PASTELMATLAB_MATLAB_MATRIX_H

#include "pastel/math/matrix.h"
#include "pastelmatlab/matlab_mex.h"

namespace Pastel {

    template <typename Real, int M = Dynamic, int N = Dynamic>
    class MatlabMatrix {
    public:
        MatlabMatrix()
        : data_()
        , view_() {
        }

        MatlabMatrix(integer rows, integer cols)
        : data_(rows * cols, 0)
        , view_(data_.data(), rows, cols) {
        }

        MatlabMatrix(const mxArray* that) 
        : MatlabMatrix()
        {
            ENSURE(mxIsNumeric(that));

            integer m = mxGetM(that);
            integer n = mxGetN(that);
            Real* data = (Real*)mxGetData(that);

            if (typeToMatlabClassId<Real>() != mxGetClassID(that)) {
                // Types do not match; copy the data.
                data_.resize(m * n);
                matlabGetScalars(that, data_.begin());
                data = data_.data();
            }

            view_ = MatrixView<Real, M, N>(data, m, n);
        }

        MatlabMatrix(const MatlabMatrix& that)
        : data_(that.data_)
        , view_(that.view_) {
            if (that.data_.data() == that.view_.data()) {
                view_ = MatrixView<Real, M, N>(data_.data(), that.rows(), that.cols());
            }
        }

        MatlabMatrix(MatlabMatrix&& that)
        : MatlabMatrix() {
            swap(that);
        }

        MatlabMatrix& operator=(const MatlabMatrix& that) = delete;
        MatlabMatrix& operator=(MatlabMatrix&& that) = delete;

        void swap(MatlabMatrix& that) {
            data_.swap(that.data_);
            view_.swap(that.view_);
        }

        void reshape(integer rows, integer cols) {
            ENSURE_OP(rows * cols, ==, size());
            view_ = MatrixView<Real, M, N>(data(), rows, cols);
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
        std::vector<Real> data_;
        MatrixView<Real, M, N> view_;
    };

	//! Retrieves a reference to a dreal matrix.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type, int M = Dynamic, int N = Dynamic>
	MatlabMatrix<Type, M, N> matlabAsMatrix(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));
		return MatlabMatrix<Type, M, N>(that);
	}

	template <typename Type, int M = Dynamic>
	MatlabMatrix<Type, M, 1> matlabAsColMatrix(
		const mxArray* that)
	{
		return matlabAsMatrix<Type, M, 1>(that);
	}

	//! Retrieves a reference to a linearized dreal array.
	/*!
	Preconditions:
	mxIsNumeric(that)
	*/
	template <typename Type>
	MatlabMatrix<Type> matlabAsVectorizedMatrix(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));

		auto m = matlabAsMatrix<Type>(that);
		m.reshape(m.size(), 1);

		return m;
	}

}

#endif
