#ifndef PASTELMATH_MATRIX_VIEW_H
#define PASTELMATH_MATRIX_VIEW_H

#include <iostream>

namespace Pastel {

    template <typename Real, int M = Dynamic, int N = Dynamic, bool ColumnMajor = true>
    requires 
        std::is_object_v<Real> &&
        Real_Concept<Real>
    class MatrixView {
    public:
        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        requires 
            std::is_object_v<Real_> &&
            Real_Concept<Real_>
        friend class MatrixView;

        MatrixView(Real* data = nullptr, integer m = 0, integer n = 0) 
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

        decltype(auto) range() const {
            return Pastel::range(data(), data() + size());
        }

        Real& operator()(integer i, integer j) const {
            if constexpr (ColumnMajor) {
                return data_[j * cols() + i];
            } else {
                return data_[i * rows() + j];
            }
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
