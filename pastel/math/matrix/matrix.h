// Description: Matrix

#ifndef PASTELMATH_MATRIX_H
#define PASTELMATH_MATRIX_H

#include "pastel/math/matrix/matrix_expression.h"

#include "pastel/sys/array.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/iterator/sparse_iterator.h"

#include <Eigen/Core>
#include <Eigen/Dense>

namespace Pastel
{

	template <typename Real, int M = Eigen::Dynamic, int N = Eigen::Dynamic>
	using Matrix = Eigen::Matrix<Real, M, N>;

	template <typename Real, int M = Eigen::Dynamic, int N = Eigen::Dynamic>
	using MapMatrix = Eigen::Map<Eigen::Matrix<Real, M, N>>;

	template <typename Real, int N = Eigen::Dynamic>
	using RowMatrix = Eigen::Matrix<Real, 1, N>;

	template <typename Real, int N = Eigen::Dynamic>
	using MapRowMatrix = Eigen::Map<Eigen::Matrix<Real, 1, N>>;

	template <typename Real, int M = Eigen::Dynamic>
	using ColMatrix = Eigen::Matrix<Real, M, 1>;

	template <typename Real, int M = Eigen::Dynamic>
	using MapColMatrix = Eigen::Map<Eigen::Matrix<Real, M, 1>>;

	template <typename T>
	using MatrixExpr = Eigen::MatrixBase<T>;

	class SingularMatrix_Exception {};

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
		RowMatrix<Real, N> m = ColMatrix<Real, N>::Zero(1, n);
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

}

#include "pastel/math/matrix/matrix_tools.h"
#include "pastel/math/matrix/matrix_view.h"

#endif
