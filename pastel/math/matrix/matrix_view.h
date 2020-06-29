#ifndef PASTELMATH_MATRIX_VIEW_H
#define PASTELMATH_MATRIX_VIEW_H

#include "pastel/math/matrix.h"

namespace Pastel {

    template <typename Real>
    class MatrixView {
    public:
        MatrixView(Real* data = nullptr, integer m = 0, integer n = 0) 
        : data_(data)
        , m_(m)
        , n_(n)
        {
        }

        MatrixView(const MatrixView& that) = default;
        MatrixView& operator=(const MatrixView& that) = default;

        bool isEmpty() const {
            return m_ == 0 || n_ == 0;
        }

        integer size() const {
            return m_ * n_;
        }

        Real* data() const {
            return data_;
        }

        integer rows() const {
            return m_;
        }

        integer cols() const {
            return n_;
        }

        void reshape(integer rows, integer cols) {
            ENSURE_OP(rows * cols, ==, rows() * cols());
            m_ = rows;
            n_ = cols;
        }

    private:
        Real* data_ = nullptr;
        integer m_ = 0;
        integer n_ = 0;
    };

    template <typename Real, int M = Dynamic, int N = Dynamic>
    MapMatrix<Real, M, N> matrix(const MatrixView<Real>& view) {
        return MapMatrix<Real, M, N>(view.data(), view.rows(), view.cols());
    }

    template <typename Real, int M = Dynamic>
    MapColMatrix<Real, M> colMatrix(const MatrixView<Real>& view) {
        ENSURE_OP(view.cols(), ==, 1);
        return MapColMatrix<Real, M>(view.data(), view.rows(), view.cols());
    }

    template <typename Real, int N = Dynamic>
    MapRowMatrix<Real, N> rowMatrix(const MatrixView<Real>& view) {
        ENSURE_OP(view.rows(), ==, 1);
        return MapRowMatrix<Real, N>(view.data(), view.rows(), view.cols());
    }

    template <typename Real, int M, int N> 
    MatrixView<Real> view(Matrix<Real, M, N>& matrix) {
        return MatrixView<Real>(matrix.data(), matrix.rows(), matrix.cols());
    }

    template <typename Real, int M, int N> 
    MatrixView<const Real> view(const Matrix<Real, M, N>& matrix) {
        return MatrixView<const Real>(matrix.data(), matrix.rows(), matrix.cols());
    }

}

#endif
