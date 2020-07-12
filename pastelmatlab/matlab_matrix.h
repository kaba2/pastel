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

        MatlabMatrix(const mxArray* that) {
            ENSURE(mxIsNumeric(that));

            integer m = mxGetM(that);
            integer n = mxGetN(that);

            if (typeToMatlabClassId<Real>() == mxGetClassID(that)) {
                // Types match; aliase the existing data.
                view_ = MatrixView<Real, M, N>((Real*)mxGetData(that), m, n);
            } 
            else {
                // Types do not match; copy the data.
                resize(m, n);
                matlabGetScalars(that, data_.begin());
            }
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

        void swap(MatlabMatrix& that) {
            data_.swap(that.data_);
            view_.swap(that.view_);
        }

        void resize(integer rows, integer cols) {
            data_.resize(rows * cols);
            view_ = MatrixView<Real, M, N>(data_.data(), rows, cols);
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
            return (Real*)data_.data();
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
		m.resize(1, m.size());
		return m;
	}

}

#endif
