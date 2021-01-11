#ifndef PASTELMATH_MATRIX_VIEW_H
#define PASTELMATH_MATRIX_VIEW_H

#include <iostream>

#include "pastel/sys/algebra/ring_concept.h"
#include "pastel/sys/range/sparse_range.h"
#include "pastel/sys/math/divide_and_round_up.h"

namespace Pastel {

    template <typename Real, int M = Dynamic, int N = Dynamic>
    requires 
        std::is_object_v<Real> &&
        Ring_Concept<Real>
    class MatrixView {
    public:
        template <typename Real_, int M_, int N_>
        requires 
            std::is_object_v<Real_> &&
            Ring_Concept<Real_>
        friend class MatrixView;

        MatrixView() = default;

        explicit MatrixView(Real* data)
        : MatrixView(data, M, N)
        {
        }

        MatrixView(
            Real* data, 
            integer m, 
            integer n)
        : MatrixView(data, m, n, 1, m)
        {
        }

        MatrixView(
            Real* data, 
            integer m, 
            integer n,
            integer iStride,
            integer jStride) 
        : data_(data)
        , m_((m == 0 && M >= 0) ? M : m)
        , n_((n == 0 && N >= 0) ? N : n)
        , iStride_(iStride)
        , jStride_(jStride)
        {
            ENSURE2(M == Dynamic || M == m_, m_, M);
            ENSURE2(N == Dynamic || N == n_, n_, N);
        }

        template <typename Real_, int M_, int N_>
        requires
            std::is_convertible_v<Real_*, Real*> &&
            IsEqualDim<M, M_> &&
            IsEqualDim<N, N_>
        MatrixView(const MatrixView<Real_, M_, N_>& that) 
        : data_(that.data_)
        , m_(that.m_)
        , n_(that.n_)
        , iStride_(that.iStride_)
        , jStride_(that.jStride_)
        {
        }

        MatrixView(const MatrixView& that) = default;
        MatrixView(MatrixView&& that) = default;

        void swap(MatrixView& that) {
            std::swap(data_, that.data_);
            std::swap(m_, that.m_);
            std::swap(n_, that.n_);
            std::swap(iStride_, that.iStride_);
            std::swap(jStride_, that.jStride_);
        }

        MatrixView& operator=(const MatrixView&) = default;

        template <typename Real_, int M_, int N_>
        requires 
            std::is_convertible_v<Real_, Real>
        MatrixView& assign(const MatrixView<Real_, M_, N_>& that) {
            ENSURE_OP(rows(), ==, that.rows());
            ENSURE_OP(cols(), ==, that.cols());

            if (canCopyBySpan(that)) {
                ranges::copy(that.span(), std::begin(span()));
            } else {
                ranges::copy(that.range(), std::begin(range()));
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

        integer iStride() const {
            return iStride_;
        }

        integer jStride() const {
            return jStride_;
        }

        bool isRow() const {
            return rows() == 1;
        }

        bool isColumn() const {
            return cols() == 1;
        }

        bool isSquare() const {
            return rows() == cols();
        }

        bool isColumnMajor() const {
            return iStride() == 1 && jStride() == rows();
        }

        bool isRowMajor() const {
            return iStride() == cols() && jStride() == 1;
        }

        bool isMajor(bool columnMajor) const {
            return 
                (columnMajor && isColumnMajor()) || 
                (!columnMajor && isRowMajor());
        }

        std::span<Real> span() const {
            Real* lowestAddress = data_;
            if (iStride() < 0) {
                lowestAddress += iStride() * (rows() - 1);
            }
            if (jStride() < 0) {
                lowestAddress += jStride() * (cols() - 1);
            }
            return {lowestAddress, lowestAddress + spanSize()};
        }

        integer spanSize() const {
            return 
                std::abs(iStride()) * (rows() - 1) + 
                std::abs(jStride()) * (cols() - 1) + 1;
        }

        integer rowSpanSize() const {
            return std::abs(jStride()) * (cols() - 1) + 1;
        }

        integer colSpanSize() const {
            return std::abs(iStride()) * (rows() - 1) + 1;
        }

        bool hasUniqueAddresses() const {
            return size() == 0 ||
                (iStride() != 0 && std::abs(jStride()) >= colSpanSize()) ||
                (jStride() != 0 && std::abs(iStride()) >= rowSpanSize());
        }

        bool isContinuous() const {
            return spanSize() == size();
        }

        template <typename Real_, int M_, int N_>
        bool hasSameMemoryDeltas(const MatrixView<Real_, M_, N_>& that) const {
            return 
                rows() == that.rows() &&
                cols() == that.cols() &&
                iStride() == that.iStride() &&
                jStride() == that.jStride();
        }

        template <typename Real_, int M_, int N_>
        bool canCopyBySpan(const MatrixView<Real_, M_, N_>& that) const {
            return isContinuous() && hasSameMemoryDeltas(that);
        }

        auto rowRange(integer row) const {
            return Pastel::sparseRange(
                std::span(
                    data() + toIndex(row, 0), 
                    data() + toIndex(row, 0) + rowSpanSize()),
                jStride());
        }

        auto columnRange(integer col) const {
            return Pastel::sparseRange(
                std::span(
                    data() + toIndex(0, col), 
                    data() + toIndex(0, col) + colSpanSize()),
                iStride());
        }

        decltype(auto) range() const {
            return ranges::views::transform(
                ranges::views::ints((integer)0, size()),
                [&](integer i) -> Real& {
                    return (*this)(i);
                });
        }

        integer toIndex(integer i, integer j) const {
            ASSERT_OP(i, >=, 0);
            ASSERT_OP(j, >=, 0);
            ASSERT_OP(i, <, rows());
            ASSERT_OP(j, <, cols());
            return i * iStride() + j * jStride();
        }

        Real& operator()(integer i, integer j) const {
            return data_[toIndex(i, j)];
        }

        Real& operator()(integer i) const {
            return data_[toIndex(i % rows(), i / rows())];
        }

        MatrixView<Real, N, M> transpose() const {
            return {data_, cols(), rows(), jStride(), iStride()};
        }

        MatrixView<Real, M, N> flipx() const {
            return {data_ + toIndex(0, cols() - 1), rows(), cols(), iStride(), -jStride()};
        }

        MatrixView<Real, M, N> flipy() const {
            return {data_ + toIndex(rows() - 1, 0), rows(), cols(), -iStride(), jStride()};
        }

        template <int IBegin, int IEnd>
        requires (M >= 0 && 0 <= IBegin && IBegin <= IEnd && IEnd <= M)
        MatrixView<Real, IEnd - IBegin, N> slicey() const {
            return {data_ + toIndex(IBegin, 0), IEnd - IBegin, cols(), iStride(), jStride()};
        }

        MatrixView<Real, Dynamic, N> slicey(integer iBegin, integer iEnd) const {
            ASSERT_OP(iBegin, >=, 0);
            ASSERT_OP(iBegin, <=, iEnd);
            ASSERT_OP(iEnd, <=, rows());
            return {data_ + toIndex(iBegin, 0), iEnd - iBegin, cols(), iStride(), jStride()};
        }

        MatrixView<Real, Dynamic, N> slicey(integer iBegin) const {
            return slicey(iBegin, rows());
        }

        template <int JBegin, int JEnd>
        requires (N >= 0 && 0 <= JBegin && JBegin <= JEnd && JEnd <= N)
        MatrixView<Real, M, JEnd - JBegin> slicex() const {
            return {data_ + toIndex(0, JBegin), rows(), JEnd - JBegin, iStride(), jStride()};
        }

        MatrixView<Real, M, Dynamic> slicex(integer jBegin, integer jEnd) const {
            ASSERT_OP(jBegin, >=, 0);
            ASSERT_OP(jBegin, <=, jEnd);
            ASSERT_OP(jEnd, <=, cols());
            return {data_ + toIndex(0, jBegin), rows(), jEnd - jBegin, iStride(), jStride()};
        }

        MatrixView<Real, Dynamic, N> slicex(integer jBegin) const {
            return slicex(jBegin, cols());
        }

        template <integer step>
        requires (N >= 0 && step > 0)
        MatrixView<Real, M, ModifyN<N, (N + step - 1) / step>> sparsex() {
            return {data_, rows(), divideAndRoundUp(cols(), step), iStride(), jStride() * step};
        }

        MatrixView<Real, M, Dynamic> sparsex(integer step) {
            ASSERT_OP(step, >, 0);
            return {data_, rows(), divideAndRoundUp(cols(), step), iStride(), jStride() * step};
        }

        template <integer step>
        requires (M >= 0 && step > 0)
        MatrixView<Real, ModifyN<M, (M + step - 1) / step>, N> sparsey() {
            return {data_, divideAndRoundUp(rows(), step), cols(), iStride() * step, jStride()};
        }

        MatrixView<Real, Dynamic, N> sparsey(integer step) {
            ASSERT_OP(step, >, 0);
            return {data_, divideAndRoundUp(rows(), step), cols(), iStride() * step, jStride()};
        }

        template <integer times>
        MatrixView<Real, M, times> repeatColumn(integer col) {
            return {data_ + toIndex(0, col), rows(), times, iStride(), times == 1 ? jStride() : 0};
        }

        MatrixView<Real, M, Dynamic> repeatColumn(integer col, integer times) {
            return {data_ + toIndex(0, col), rows(), times, iStride(), times == 1 ? jStride() : 0};
        }

        template <integer times>
        MatrixView<Real, times, N> repeatRow(integer row) {
            return {data_ + toIndex(row, 0), times, cols(), times == 1 ? iStride() : 0, jStride()};
        }

        MatrixView<Real, Dynamic, N> repeatRow(integer row, integer times) {
            return {data_ + toIndex(row, 0), times, cols(), times == 1 ? iStride() : 0, jStride()};
        }

        template <typename Real_, int M_, int N_>
        bool equals(const MatrixView<Real_, M_, N_>& that) const {
            if (rows() != that.rows() ||
                cols() != that.cols()) {
                return false;
            }
            if (canCopyBySpan(that)) {
                return ranges::equal(span(), that.span());
            } else {
                return ranges::equal(range(), that.range());
            }
        }

    private:
        Real* data_ = nullptr;
        integer m_ = 0;
        integer n_ = 0;
        integer iStride_ = 0;
        integer jStride_ = 0;
    };

    template <typename Real, int M, int N>
    MatrixView<Real, M, N> view(Real (&that)[M][N]) {
        return MatrixView<Real, M, N>(&that[0][0], M, N, N, 1);
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
