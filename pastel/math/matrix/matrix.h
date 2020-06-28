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

	template <typename T>
	using MatrixExpr = Eigen::MatrixBase<T>;

	class SingularMatrix_Exception {};

	template <typename Real, int N>
	Matrix<Real, N, 1> asColumnMatrix(const Vector<Real, N>& v) {
		int n = v.size();
		Matrix<Real, N, 1> m = Matrix<Real, N, 1>::Zero(n, 1);
		for (int i = 0; i < n; ++i) {
			m(i, 0) = v[i];
		}
		return m;
	}

	template <typename Real, int N>
	Matrix<Real, 1, N> asRowMatrix(const Vector<Real, N>& v) {
		int n = v.size();
		Matrix<Real, 1, N> m = Matrix<Real, N, 1>::Zero(1, n);
		for (int i = 0;i < n; ++i) {
			m(0, i) = v[i];
		}
		return m;
	}

	template <typename Derived>
	decltype(auto) evaluate(const Eigen::DenseBase<Derived>& that) {
		return that.eval();
	}

}

#include "pastel/math/matrix/matrix_tools.h"

#endif
