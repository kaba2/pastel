#ifndef PASTELMATH_MATRIX_VIEW_H
#define PASTELMATH_MATRIX_VIEW_H

#include <iostream>

#include "pastel/sys/algebra/ring_concept.h"
#include "pastel/sys/range/sparse_range.h"

namespace Pastel {

    template <typename Real, int M = Dynamic, int N = Dynamic, bool ColumnMajor = true>
    requires 
        std::is_object_v<Real> &&
        Ring_Concept<Real>
    class MatrixView {
    public:
        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        requires 
            std::is_object_v<Real_> &&
            Ring_Concept<Real_>
        friend class MatrixView;

        MatrixView(Real* data = nullptr, integer m = (M >= 0 ? M : 0), integer n = (N >= 0 ? N : 0)) 
        : data_(data)
        , m_(m)
        , n_(n)
        {
            ENSURE(M == Dynamic || M == m);
            ENSURE(N == Dynamic || N == n);
        }

        template <typename Real_, int M_, int N_>
        requires
            std::is_convertible_v<Real_*, Real*> &&
            IsEqualDim<M, M_> &&
            IsEqualDim<N, N_>
        MatrixView(const MatrixView<Real_, M_, N_, ColumnMajor>& that) 
        : data_(that.data_)
        , m_(that.m_)
        , n_(that.n_)
        {
        }

        MatrixView(const MatrixView& that) = default;
        MatrixView(MatrixView&& that) 
        {
            swap(that);
        }

        void swap(MatrixView& that) {
            std::swap(data_, that.data_);
            std::swap(m_, that.m_);
            std::swap(n_, that.n_);
        }

        MatrixView& operator=(const MatrixView&) = default;

        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        requires 
            std::is_convertible_v<Real_, Real>
        MatrixView& assign(const MatrixView<Real_, M_, N_, ColumnMajor_>& that) {
            ENSURE_OP(rows(), ==, that.rows());
            ENSURE_OP(cols(), ==, that.cols());
            if constexpr (ColumnMajor == ColumnMajor_) {
                std::copy(that.data(), that.data() + that.size(), data());
            } else {
                for (int i = 0;i < rows(); ++i) {
                    for (int j = 0;j < cols(); ++j) {
                        (*this)(i, j) = that(i, j);
                    }
                }
            }
            return *this;
        }

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
            ENSURE_OP(rows * cols, ==, size());
            m_ = rows;
            n_ = cols;
        }

        decltype(auto) rowRange(integer row) const {
            if constexpr (ColumnMajor) {
                return Pastel::sparseRange(
                    Pastel::range(
                        data() + toIndex(row, 0), 
                        data() + size()),
                    rows());

            } else {
                return Pastel::range(
                    data() + toIndex(row, 0), 
                    data() + toIndex(row + 1, 0));
            }
        }

        decltype(auto) columnRange(integer col) const {
            if constexpr (!ColumnMajor) {
                return Pastel::sparseRange(
                    Pastel::range(
                        data() + toIndex(0, col), 
                        data() + size()),
                    cols());

            } else {
                return Pastel::range(
                    data() + toIndex(0, col), 
                    data() + toIndex(0, col + 1));
            }
        }

        decltype(auto) range() const {
            return Pastel::range(data(), data() + size());
        }

        integer toIndex(integer i, integer j) const {
            if constexpr (ColumnMajor) {
                return j * rows() + i;
            } else {
                return i * cols() + j;
            }
        }

        Real& operator()(integer i, integer j) const {
            return data_[toIndex(i, j)];
        }

        Real& operator()(integer i) const {
            return data_[i];
        }

        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        bool equals(const MatrixView<Real_, M_, N_, ColumnMajor_>& that) const {
            if (rows() != that.rows() ||
                cols() != that.cols()) {
                return false;
            }

            if constexpr (ColumnMajor == ColumnMajor_) {
                return ranges::equal(range(), that.range());
            } else {
                for (int i = 0;i < rows(); ++i) {
                    for (int j = 0;j < cols(); ++j) {
                        if ((*this)(i, j) != that(i, j)) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

    private:
        Real* data_ = nullptr;
        integer m_ = 0;
        integer n_ = 0;
    };

    template <typename Real, int M, int N>
    MatrixView<Real, M, N, false> view(Real (&that)[M][N]) {
        return MatrixView<Real, M, N, false>(&that[0][0], M, N);
    }

    template <typename Real, int M = Dynamic>
    using ColMatrixView = MatrixView<Real, M, 1>;

    template <typename Real, int N = Dynamic>
    using RowMatrixView = MatrixView<Real, 1, N>;

	template <typename Real, int M, int N>
	std::ostream& operator<<(
		std::ostream& stream,
		const MatrixView<Real, M, N>& that)
	{
        for (integer i = 0; i < that.rows(); ++i) {
            for (integer j = 0;j < that.cols(); ++j) {
                stream << that(i, j) << " ";
            }
            stream << std::endl;
        }
        return stream;
    }

}

#endif
