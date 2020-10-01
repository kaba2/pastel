#ifndef PASTELMATH_MATRIX_DATA_H
#define PASTELMATH_MATRIX_DATA_H

#include "pastel/sys/mytypes.h"

namespace Pastel {

    template <typename Real, int M = Dynamic, int N = Dynamic>
    requires IsPlain<Real>
    class MatrixData {
    private:
        struct RowData {
            RowData() = default;
            
            Real* data = nullptr;
            integer cols = 0;

            void swap(RowData& that) {
                std::swap(data, that.data);
                std::swap(cols, that.cols);
            }
        };

        struct ColData {
            ColData() = default;

            Real* data = nullptr;
            integer rows = 0;

            void swap(ColData& that) {
                std::swap(data, that.data);
                std::swap(rows, that.rows);
            }
        };

        struct DynamicData {
            DynamicData() = default;

            Real* data = nullptr;
            integer rows = 0;
            integer cols = 0;

            void swap(DynamicData& that) {
                std::swap(data, that.data);
                std::swap(rows, that.rows);
                std::swap(cols, that.cols);
            }
        };

        using StaticData = Real[M * N > 0 ? M * N : 1];

        static constexpr bool IsStatic = (M != Dynamic && N != Dynamic);
        using Data = std::conditional_t<M == Dynamic,
            std::conditional_t<N == Dynamic, DynamicData, ColData>,
            std::conditional_t<N == Dynamic, RowData, StaticData>
        >;

    public:
        MatrixData() 
        : data_()
        , iStride_(0)
        , jStride_(0)
        {
        }

        template <typename Real_, int M_, int N_>
        requires 
            IsEqualDim<M, M_> &&
            IsEqualDim<N, N_> &&
            std::is_convertible_v<Real_, Real>
        MatrixData(const MatrixView<Real_, M_, N_>& that)
        : MatrixData(that.rows(), that.cols())
        {
            view().assign(that);
        }
        
        MatrixData(const MatrixData& that) 
        : MatrixData(that.rows(), that.cols())
        {
            view().assign(that.view());
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

        MatrixData(
            integer rows, integer cols,
            bool columnMajor = true)
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
            iStride_ = 1;
            jStride_ = rows;
            if (!columnMajor) {
                std::swap(iStride_, jStride_);
            }
        }

        Real& operator()(integer i, integer j) {
            return (Real&)(*(const MatrixData*)this)(i, j);
        }

        const Real& operator()(integer i, integer j) const {
            return *(data() + i * iStride() + j * jStride());
        }

        void swap(MatrixData& that) {
            if constexpr (IsStatic) {
                std::swap(data_, that.data_);
            } else {
                data_.swap(that.data_);
            }

            std::swap(iStride_, that.iStride_);
            std::swap(jStride_, that.jStride_);
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

        integer iStride() const {
            return iStride_;
        }

        integer jStride() const {
            return jStride_;
        }

        integer size() const {
            return rows() * cols();
        }

        Real* data() {
            if constexpr (IsStatic) {
                return &data_[0];
            } else {
                return data_.data;
            }
        }

        const Real* data() const {
            if constexpr (IsStatic) {
                return &data_[0];
            } else {
                return data_.data;
            }
        }

        template <typename Real_, int M_, int N_>
        bool operator==(const MatrixView<Real_, M_, N_>& that) const {
            return view().equals(that);
        }

        template <typename Real_, int M_, int N_>
        bool operator!=(const MatrixView<Real_, M_, N_>& that) const {
            return !(*this == that);
        }

        template <typename Real_, int M_, int N_>
        requires std::is_convertible_v<Real_, Real>
        MatrixData& operator=(const MatrixData<Real_, M_, N_>& that) {
            return assign(that.view());
        }

        MatrixData& operator=(const MatrixData& that) {
            return assign(that.view());
        }

        template <typename Real_, int M_, int N_>
        requires std::is_convertible_v<Real_, Real>
        MatrixData& operator=(const MatrixView<Real_, M_, N_>& that) {
            return assign(that);
        }

        template <typename Real_, int M_, int N_>
        requires std::is_convertible_v<Real_, Real>
        MatrixData& assign(const MatrixView<Real_, M_, N_>& that) {
            if (rows() == that.rows() && cols() == that.cols()) {
                view().assign(that.view());
            } else {
                MatrixData(that).swap(*this);
            }
            return *this;
        }

        // template <typename Real_, int M_, int N_>
        // requires std::is_convertible_v<Real_, Real>
        // MatrixData& assign(MatrixView<Real_, M_, N_>&& that) {
        //     that.swap(*this);
        //     MatrixData().swap(that);
        //     return *this;
        // }

        operator MatrixView<Real, M, N>() {
            return view();
        }

        operator MatrixView<const Real, M, N>() const {
            return view();
        }

        MatrixView<Real, M, N> view() {
            return MatrixView<Real, M, N>(data(), rows(), cols(), iStride(), jStride());
        }

        MatrixView<const Real, M, N> view() const {
            return MatrixView<const Real, M, N>(data(), rows(), cols(), iStride(), jStride());
        }

    private:
        Data data_;
        integer iStride_;
        integer jStride_;
    };

}

#endif
