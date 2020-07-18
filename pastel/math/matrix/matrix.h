// Description: Matrix

#ifndef PASTELMATH_MATRIX_H
#define PASTELMATH_MATRIX_H

#include "pastel/sys/array.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/iterator/sparse_iterator.h"

#include <Eigen/Core>
#include <Eigen/Dense>

namespace Pastel
{

	template <int M, int N, bool ColumnMajor>
	constexpr int EigenOptions = Eigen::AutoAlign | (
			((M == 1 && N != 1) || !ColumnMajor) ? Eigen::RowMajor : Eigen::ColMajor
		);

	using StrideD = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>;

	template <typename Real, int M = Eigen::Dynamic, int N = Eigen::Dynamic, bool ColumnMajor = true>
	using Matrix = Eigen::Matrix<Real, M, N, EigenOptions<M, N, ColumnMajor>>;

	template <typename Real, int M = Eigen::Dynamic, int N = Eigen::Dynamic>
	using MapMatrix = Eigen::Map<Eigen::Matrix<Real, M, N, Eigen::ColMajor>, Eigen::Unaligned, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>>;

	template <typename Real, int M = Eigen::Dynamic, int N = Eigen::Dynamic>
	using MapConstMatrix = Eigen::Map<const Eigen::Matrix<Real, M, N, Eigen::ColMajor>, Eigen::Unaligned, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>>;

	template <typename Real, int N = Eigen::Dynamic>
	using RowMatrix = Matrix<Real, 1, N>;

	template <typename Real, int N = Eigen::Dynamic>
	using MapRowMatrix = MapMatrix<Real, 1, N>;

	template <typename Real, int M = Eigen::Dynamic>
	using ColMatrix = Matrix<Real, M, 1>;

	template <typename Real, int M = Eigen::Dynamic>
	using MapColMatrix = MapMatrix<Real, M, 1>;

	template <typename T>
	using MatrixExpr = Eigen::MatrixBase<T>;

	class SingularMatrix_Exception {};

}

#include "pastel/math/matrix/matrix_view.h"
#include "pastel/math/matrix/matrix_data.h"

namespace Pastel {

	template <typename Real, int N>
	ColMatrix<Real, N> asColumnMatrix(const Vector<Real, N>& v) {
		int n = v.size();
		ColMatrix<Real, N> m = ColMatrix<Real, N>::Zero(n, 1);
		for (int i = 0; i < n; ++i) {
			m(i, 0) = v[i];
		}
		return m;
	}

	template <typename Real, int N>
	RowMatrix<Real, N> asRowMatrix(const Vector<Real, N>& v) {
		int n = v.size();
		RowMatrix<Real, N> m = RowMatrix<Real, N>::Zero(1, n);
		for (int i = 0;i < n; ++i) {
			m(0, i) = v[i];
		}
		return m;
	}

	//! Generates a (m x n)-matrix with elements given by a function f(i, j).
	/*!
	Preconditions:
	m >= 0
	n >= 0
	*/
	template <typename Real, int M = Dynamic, int N = Dynamic, typename F>
	Matrix<Real, M, N> generateMatrix(F&& f, integer m = M, integer n = N)
	{
		ENSURE_OP(m, >=, 0);
		ENSURE_OP(n, >=, 0);

		Matrix<Real, M, N> result;
		result.resize(m, n);
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j) {
				result(i, j) = f(i, j);
			}
		}

		return result;
	}


	template <typename Derived>
	decltype(auto) evaluate(const Eigen::DenseBase<Derived>& that) {
		return that.eval();
	}

    template <typename Real, int M, int N>
    MapMatrix<Real, M, N> asMatrix(const MatrixView<Real, M, N>& view) {
        return MapMatrix<Real, M, N>(
			view.data(), view.rows(), view.cols(), 
			StrideD(view.jStride(), view.iStride()));
    }

    template <typename Real, int M, int N>
    MapConstMatrix<Real, M, N> asMatrix(const MatrixView<const Real, M, N>& view) {
        return MapConstMatrix<Real, M, N>(
			view.data(), view.rows(), view.cols(), 
			StrideD(view.jStride(), view.iStride()));
	}

    template <typename Real, int M, int N, int Options> 
    MatrixView<Real, M, N> view(Eigen::Matrix<Real, M, N, Options>& matrix) {
        return MatrixView<Real, M, N>(
			matrix.data(), matrix.rows(), matrix.cols(),
			(Options == Eigen::ColMajor) ? matrix.innerStride() : matrix.outerStride(),
			(Options == Eigen::ColMajor) ? matrix.outerStride() : matrix.innerStride());
    }

    template <typename Real, int M, int N, int Options> 
    MatrixView<const Real, M, N> view(const Eigen::Matrix<Real, M, N, Options>& matrix) {
        return MatrixView<const Real, M, N>(
			matrix.data(), matrix.rows(), matrix.cols(),
			(Options == Eigen::ColMajor) ? matrix.innerStride() : matrix.outerStride(),
			(Options == Eigen::ColMajor) ? matrix.outerStride() : matrix.innerStride());
    }

}

#include "pastel/math/matrix/matrix_tools.h"

#endif
