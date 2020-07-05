#ifndef PASTELMATLAB_MATLAB_MATRIX_H
#define PASTELMATLAB_MATLAB_MATRIX_H

#include "pastel/math/matrix.h"
#include "pastel/matlab/matlab_mex.h"

namespace Pastel {

    template <typename Real, int M, int N>
    class MatlabMatrix {
    public:
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

}

#endif
