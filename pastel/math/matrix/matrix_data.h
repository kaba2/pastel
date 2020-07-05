#ifndef PASTELMATH_MATRIX_DATA_H
#define PASTELMATH_MATRIX_DATA_H

#include "pastel/sys/mytypes.h"

namespace Pastel {

    template <typename Real, int M = Dynamic, int N = Dynamic, bool ColumnMajor = true>
    requires IsPlain<Real>
    class MatrixData {
    private:
        struct RowData {
            Real* data = nullptr;
            integer cols = 0;
        };

        struct ColData {
            Real* data = nullptr;
            integer rows = 0;
        };

        struct DynamicData {
            Real* data = nullptr;
            integer rows = 0;
            integer cols = 0;
        };

        using StaticData = Real[M * N];
        
        static constexpr bool IsStatic = (M != Dynamic && N != Dynamic);
        using Data = std::conditional_t<M == Dynamic,
            std::conditional_t<N == Dynamic, DynamicData, ColData>,
            std::conditional_t<N == Dynamic, RowData, StaticData>
        >;

    public:
        MatrixData() 
        : data_()
        {
        }

        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        requires 
            IsEqualDim<M, M_> &&
            IsEqualDim<N, N_> &&
            std::is_convertible_v<Real_, Real>
        MatrixData(const MatrixView<Real_, M_, N_, ColumnMajor_>& that) 
        : MatrixData(that.rows(), that.cols())
        {
            view().assign(that);
        }
        
        MatrixData(const MatrixData& that) 
        : MatrixData(that.rows(), that.cols())
        {
            if constexpr (IsStatic) {
                data_ = that.data_;
            } else {
                std::copy(that.data(), that.data() + that.size(), data());
            }
        }
        
        MatrixData(MatrixData&& that) 
        : MatrixData()
        {
            swap(that);
        }
        
        ~MatrixData() {
            if constexpr (!IsStatic) {
                if (data_.data) {
                    delete[] data_.data;
                }
            }
        }

        MatrixData(integer rows, integer cols)
        : MatrixData() 
        {
            ENSURE_OP(rows, >=, 0);
            ENSURE_OP(cols, >=, 0);
            ENSURE(M == Dynamic || rows == M);
            ENSURE(N == Dynamic || cols == N);

            if constexpr (!IsStatic) {
                data_.data = (rows * cols > 0) ? new Real[rows * cols] : nullptr;
            }
            if constexpr (M == Dynamic) {
                data_.rows = rows;
            }
            if constexpr (N == Dynamic) {
                data_.cols = cols;
            }
        }

        Real& operator()(integer i, integer j) {
            return (Real&)(*(const MatrixData*)this)(i, j);
        }

        const Real& operator()(integer i, integer j) const {
            if constexpr (ColumnMajor) {
                return *(data() + j * rows() + i);
            } else {
                return *(data() + i * cols() + j);
            }
        }

        void swap(MatrixData& that) {
            if constexpr (IsStatic) {
                std::swap(data_, that.data_);
            } else {
                std::swap(data_.data, that.data_.data);
                if (M == Dynamic) {
                    std::swap(data_.rows, that.data_.rows);
                }
                if (N == Dynamic) {
                    std::swap(data_.cols, that.data_.cols);
                }
            }
        }

        integer rows() const {
            if constexpr (M == Dynamic) {
                return data_.rows;
            } else {
                return M;
            }
        }

        integer cols() const {
            if constexpr (N == Dynamic) {
                return data_.cols;
            } else {
                return N;
            }
        }

        integer size() const {
            return rows() * cols();
        }

        Real* data() {
            return (Real*)((const MatrixData*)this)->data();
        }

        const Real* data() const {
            if constexpr (IsStatic) {
                return &data_[0];
            } else {
                return data_.data;
            }
        }

        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        bool operator==(const MatrixView<Real_, M_, N_, ColumnMajor_>& that) const {
            return view().equals(that);
        }

        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        bool operator!=(const MatrixView<Real_, M_, N_, ColumnMajor_>& that) const {
            return !(*this == that);
        }

        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        requires std::is_convertible_v<Real_, Real>
        MatrixData& operator=(const MatrixData<Real_, M_, N_, ColumnMajor_>& that) {
            return assign(that.view());
        }

        template <typename Real_, int M_, int N_, bool ColumnMajor_>
        requires std::is_convertible_v<Real_, Real>
        MatrixData& operator=(const MatrixView<Real_, M_, N_, ColumnMajor_>& that) {
            view().assign(that);
            return *this;
        }

        operator MatrixView<Real, M, N, ColumnMajor>() {
            return view();
        }

        operator MatrixView<const Real, M, N, ColumnMajor>() const {
            return view();
        }

        MatrixView<Real, M, N, ColumnMajor> view() {
            return MatrixView<Real, M, N, ColumnMajor>(data(), rows(), cols());
        }

        MatrixView<const Real, M, N, ColumnMajor> view() const {
            return MatrixView<const Real, M, N, ColumnMajor>(data(), rows(), cols());
        }

    private:
        Data data_;
    };

}

#endif
