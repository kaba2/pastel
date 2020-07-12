// Description: Testing for Matrix
// DocumentationOf: matrix.h

#include "test/test_init.h"

#include "pastel/math/matrix.h"
#include "pastel/math/sampling.h"

#include "pastel/sys/view.h"

#include <algorithm>

namespace
{

	using MatrixD = Matrix<dreal>;

}

TEST_CASE("Implicit (Implicit)")
{
	auto f = [&](MatrixD matrix)
	{
	};

	// The matrix is implicitly constructible
	// from the matrix-expression.
	f(identityMatrix<dreal>(3, 3));
}

TEST_CASE("Norm (Norm)")
{
	MatrixD m(2, 3);
	m << -1, 2, 3,
		4, -5, 6;

	{
		dreal correct = 
			square(-1) + square(2) + square(3) +
			square(4) + square(-5) + square(6);
		REQUIRE(frobeniusNorm2(m) == correct);
	}

	{
		dreal correct = 4 + 5 + 6;
		REQUIRE(maxNorm(m) == correct);
	}

	{
		dreal correct = 3 + 6;
		REQUIRE(manhattanNorm(m) == correct);
	}
}

TEST_CASE("Trace (Trace)")
{
	MatrixD m(2, 3);
	m << -1, 2, 3,
		4, -5, 6;

	{
		dreal correct = -1 + -5;
		REQUIRE(trace(m) == correct);
	}
}

TEST_CASE("DiagonalProduct (DiagonalProduct)")
{
	MatrixD m(2, 3);
	m << -1, 2, 3,
		4, -5, 6;

	{
		dreal correct = -1 * -5;
		REQUIRE(diagonalProduct(m) == correct);
	}
}

TEST_CASE("Determinant (Determinant)")
{
	{
		MatrixD m(1, 1);
		m << -1;
		{
			dreal correct = -1;
			REQUIRE(determinant(m) == correct);
		}
	}

	{
		MatrixD m(2, 2);
		m << -1, 2,
			4, -5;
		{
			dreal correct = (-1 * -5) - (2 * 4);
			REQUIRE(determinant(m) == correct);
		}
	}

	{
		MatrixD m(3, 3);
		m << -1, 2, 3,
			4, -5, 5,
			2, 3, 4;
		{
			dreal correct = 89;
			REQUIRE(std::abs(determinant(m) - correct) < 0.0001);
		}
	}

	//{
	//	MatrixD m = randomRotation<dreal>(10);
	//	{
	//		REQUIRE(std::abs(determinant(m) - 1) < 0.0001);
	//	}
	//}
}

TEST_CASE("MatrixExpressions (MatrixExpressions)")
{
	// Construct an empty matrix.
	MatrixD empty(0, 0);
	{
		REQUIRE(empty.size() == 0);
		REQUIRE(empty.cols() == 0);
		REQUIRE(empty.rows() == 0);
	}

	// Constructs from a matrix expression.
	MatrixD m = (identityMatrix<dreal>(2, 3) * 2).array() + 5;
	{
		MatrixD correct(2, 3);
		correct <<
			7, 5, 5,
			5, 7, 5;

		REQUIRE(ranges::equal(range(m), range(correct)));
	}

	{
		MatrixD test(2, 3);
		test << 
			1, 2, 3,
			4, 5, 6;

		// Adds a matrix expression.
		test += identityMatrix<dreal>(2, 3);
		{
			MatrixD correct(2, 3);
			correct <<
				2, 2, 3,
				4, 6, 6;

			REQUIRE(ranges::equal(range(test), range(correct)));
		}

		// Subtracts a matrix expression.
		test -= identityMatrix<dreal>(2, 3);
		{
			MatrixD correct(2, 3);
			correct <<
				1, 2, 3,
				4, 5, 6;

			REQUIRE(ranges::equal(range(test), range(correct)));
		}

		// Multiplies with a matrix expression.
		test *= identityMatrix<dreal>(3, 2);
		{
			MatrixD correct(2, 2);
			correct <<
				1, 2,
				4, 5;

			REQUIRE(ranges::equal(range(test), range(correct)));
		}
	}

	// Constructs an mxn identity matrix.
	MatrixD a = MatrixD::Identity(4, 6);
	{
		MatrixD correct(4, 6);
		correct <<
			1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0,
			0, 0, 0, 1, 0, 0;

		REQUIRE(ranges::equal(range(a), range(correct)));
		REQUIRE(a.rows() == 4);
		REQUIRE(a.cols() == 6);
		REQUIRE(a.size() == 4 * 6);
	}

	dreal dataSet[] = 
	{
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		10, 11, 12
	};

	// Constructs from a shared array.
	Eigen::Map<Matrix<dreal, Dynamic, Dynamic, false>> shared(dataSet, 4, 3);
	{
		REQUIRE(ranges::equal(range(shared), range(dataSet)));
	}

	// Element access
	for (integer i = 0;i < 12;++i)
	{
		REQUIRE(shared(i) == i + 1);
		REQUIRE(shared(i / 3, i % 3) == i + 1);
	}

	{
		MatrixD test(2, 3);
		test << 
			1, 2, 3,
			4, 5, 6;

		// Subtracts a constant from all elements.
		test.array() -= 1;
		{
			MatrixD correct(2, 3);
			correct <<
				0, 1, 2,
				3, 4, 5;

			REQUIRE(ranges::equal(range(test), range(correct)));
		}

		// Adds a constant to all elements.
		test.array() += 1;
		{
			MatrixD correct(2, 3);
			correct <<
				1, 2, 3,
				4, 5, 6;

			REQUIRE(ranges::equal(range(test), range(correct)));
		}

		// Multiplies all elements with a constant.
		test *= 2;
		{
			MatrixD correct(2, 3);
			correct <<
				2, 4, 6,
				8, 10, 12;

			REQUIRE(ranges::equal(range(test), range(correct)));
		}

		// Divides all elements by a constant.
		test /= 2;
		{
			MatrixD correct(2, 3);
			correct <<
				1, 2, 3,
				4, 5, 6;

			REQUIRE(ranges::equal(range(test), range(correct)));
		}
	}

	a << 1, 0, 1, 0, 1, 0,
		0, 1, 0, 1, 0, 1,
		2, 0, 1, 0, 1, 0,
		0, 2, 0, 1, 0, 1;
	{
		MatrixD correct(4, 6);
		correct <<
			1, 0, 1, 0, 1, 0,
			0, 1, 0, 1, 0, 1,
			2, 0, 1, 0, 1, 0,
			0, 2, 0, 1, 0, 1;

		REQUIRE(ranges::equal(range(a), range(correct)));
	}

	// The 2x2 identity-matrix repeated 2 times vertically,
	// and 3 times horizontally.
	MatrixD I = identityMatrix<dreal>(2, 2);
	MatrixD b(4, 6);
	b << 
		I, I, I,
		I, I, I;
	{
		MatrixD correct(4, 6);
		correct <<
			1, 0, 1, 0, 1, 0,
			0, 1, 0, 1, 0, 1,
			1, 0, 1, 0, 1, 0,
			0, 1, 0, 1, 0, 1;

		REQUIRE(ranges::equal(range(b), range(correct)));
	}
}

TEST_CASE("MatrixArray (MatrixArray)")
{
	MatrixD a(3, 3);
	a << 1, 2, 3,
		-2, 3, -4,
		7, -3, 2;

	Vector3 b = asVector(max(a));
	REQUIRE(b[0] == 7);
	REQUIRE(b[1] == 3);
	REQUIRE(b[2] == 3);

	Vector3 c = asVector(min(a));
	REQUIRE(c[0] == -2);
	REQUIRE(c[1] == -3);
	REQUIRE(c[2] == -4);
}

TEST_CASE("MatrixLowDimensional (MatrixLowDimensional)")
{
	{
		Matrix<dreal, 1, 1> a = matrix1x1<dreal>(5);
		REQUIRE(
			a(0, 0) == 5);
	}
	{
		Matrix<dreal, 2, 2> a = 
			matrix2x2<dreal>(
			1, 2,
			3, 4);
		REQUIRE(a(0, 0) == 1);
		REQUIRE(a(0, 1) == 2);
		REQUIRE(a(1, 0) == 3);
		REQUIRE(a(1, 1) == 4);

		Matrix<dreal, 2, 2> b;

		b = a;
		REQUIRE(b(0, 0) == 1);
		REQUIRE(b(0, 1) == 2);
		REQUIRE(b(1, 0) == 3);
		REQUIRE(b(1, 1) == 4);

		Matrix<dreal, 2, 2> c(b);
		REQUIRE(c(0, 0) == 1);
		REQUIRE(c(0, 1) == 2);
		REQUIRE(c(1, 0) == 3);
		REQUIRE(c(1, 1) == 4);

	}

	{
		Matrix<dreal, 3, 3> a = 
			matrix3x3<dreal>(
			1, 2, 3,
			4, 5, 6,
			7, 8, 9);
		REQUIRE(a(0, 0) == 1);
		REQUIRE(a(0, 1) == 2);
		REQUIRE(a(0, 2) == 3);
		REQUIRE(a(1, 0) == 4);
		REQUIRE(a(1, 1) == 5);
		REQUIRE(a(1, 2) == 6);
		REQUIRE(a(2, 0) == 7);
		REQUIRE(a(2, 1) == 8);
		REQUIRE(a(2, 2) == 9);

		Matrix<dreal, 3, 3> b;

		b = a;
		REQUIRE(b(0, 0) == 1);
		REQUIRE(b(0, 1) == 2);
		REQUIRE(b(0, 2) == 3);
		REQUIRE(b(1, 0) == 4);
		REQUIRE(b(1, 1) == 5);
		REQUIRE(b(1, 2) == 6);
		REQUIRE(b(2, 0) == 7);
		REQUIRE(b(2, 1) == 8);
		REQUIRE(b(2, 2) == 9);

		Matrix<dreal, 3, 3> c(b);
		REQUIRE(c(0, 0) == 1);
		REQUIRE(c(0, 1) == 2);
		REQUIRE(c(0, 2) == 3);
		REQUIRE(c(1, 0) == 4);
		REQUIRE(c(1, 1) == 5);
		REQUIRE(c(1, 2) == 6);
		REQUIRE(c(2, 0) == 7);
		REQUIRE(c(2, 1) == 8);
		REQUIRE(c(2, 2) == 9);

	}
	{
		Matrix<dreal, 4, 4> a =
			matrix4x4<dreal>(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);
		REQUIRE(a(0, 0) == 1);
		REQUIRE(a(0, 1) == 2);
		REQUIRE(a(0, 2) == 3);
		REQUIRE(a(0, 3) == 4);
		REQUIRE(a(1, 0) == 5);
		REQUIRE(a(1, 1) == 6);
		REQUIRE(a(1, 2) == 7);
		REQUIRE(a(1, 3) == 8);
		REQUIRE(a(2, 0) == 9);
		REQUIRE(a(2, 1) == 10);
		REQUIRE(a(2, 2) == 11);
		REQUIRE(a(2, 3) == 12);
		REQUIRE(a(3, 0) == 13);
		REQUIRE(a(3, 1) == 14);
		REQUIRE(a(3, 2) == 15);
		REQUIRE(a(3, 3) == 16);

		Matrix<dreal, 4, 4> b;

		b = a;
		REQUIRE(b(0, 0) == 1);
		REQUIRE(b(0, 1) == 2);
		REQUIRE(b(0, 2) == 3);
		REQUIRE(b(0, 3) == 4);
		REQUIRE(b(1, 0) == 5);
		REQUIRE(b(1, 1) == 6);
		REQUIRE(b(1, 2) == 7);
		REQUIRE(b(1, 3) == 8);
		REQUIRE(b(2, 0) == 9);
		REQUIRE(b(2, 1) == 10);
		REQUIRE(b(2, 2) == 11);
		REQUIRE(b(2, 3) == 12);
		REQUIRE(b(3, 0) == 13);
		REQUIRE(b(3, 1) == 14);
		REQUIRE(b(3, 2) == 15);
		REQUIRE(b(3, 3) == 16);

		Matrix<dreal, 4, 4> c(b);
		REQUIRE(c(0, 0) == 1);
		REQUIRE(c(0, 1) == 2);
		REQUIRE(c(0, 2) == 3);
		REQUIRE(c(0, 3) == 4);
		REQUIRE(c(1, 0) == 5);
		REQUIRE(c(1, 1) == 6);
		REQUIRE(c(1, 2) == 7);
		REQUIRE(c(1, 3) == 8);
		REQUIRE(c(2, 0) == 9);
		REQUIRE(c(2, 1) == 10);
		REQUIRE(c(2, 2) == 11);
		REQUIRE(c(2, 3) == 12);
		REQUIRE(c(3, 0) == 13);
		REQUIRE(c(3, 1) == 14);
		REQUIRE(c(3, 2) == 15);
		REQUIRE(c(3, 3) == 16);

	}
}

TEST_CASE("MatrixSimpleArithmetic (MatrixSimpleArithmetic)")
{
	Matrix<dreal, 2, 3> a;

	a << 
        1, 2, 3,
		4, 5, 6;

	Matrix<dreal, 3, 2> b;

	b << 7, 8,
		4, 3,
		3, 6;

	Matrix<dreal, 2, 2> c(a * b);
	REQUIRE(c(0, 0) == 1 * 7 + 2 * 4 + 3 * 3);
	REQUIRE(c(0, 1) == 1 * 8 + 2 * 3 + 3 * 6);
	REQUIRE(c(1, 0) == 4 * 7 + 5 * 4 + 6 * 3);
	REQUIRE(c(1, 1) == 4 * 8 + 5 * 3 + 6 * 6);

	Matrix<dreal, 1, 3> d;
	d << 5, 2, 6;

	Matrix<dreal, 3, 1> e;
	e(0, 0) = -3;
	e(1, 0) = 6;
	e(2, 0) = -4;
	Matrix<dreal, 1, 1> f(d * e);

	REQUIRE(f(0, 0) == 5 * -3 + 2 * 6 + 6 * -4);

	Matrix<dreal, 2, 2> g =
		matrix2x2<dreal>(
		1, 2,
		3, 4);

	g <<
		1, 2,
		3, 4;
	g *= 4;
	REQUIRE(g(0, 0) == 1 * 4);
	REQUIRE(g(0, 1) == 2 * 4);
	REQUIRE(g(1, 0) == 3 * 4);
	REQUIRE(g(1, 1) == 4 * 4);

	g <<
		1, 2,
		3, 4;
	g /= 4;
	REQUIRE(g(0, 0) == (dreal)1 / 4);
	REQUIRE(g(0, 1) == (dreal)2 / 4);
	REQUIRE(g(1, 0) == (dreal)3 / 4);
	REQUIRE(g(1, 1) == (dreal)4 / 4);

}

TEST_CASE("Inverse (Inverse)")
{
	integer n = 10;
	integer matrices = 100;

	integer count = 0;

	for (integer i = 0;i < matrices;++i)
	{
		Matrix<dreal> m = randomMatrix<dreal>(n, n);

		Matrix<dreal> mInv = inverse(m);

		dreal leftError =
			manhattanNorm(m * mInv - identityMatrix<dreal>(n, n));
		dreal rightError =
			manhattanNorm(mInv * m - identityMatrix<dreal>(n, n));
		if (leftError > 0.001 ||
			rightError > 0.001)
		{
			++count;
		}
	}

	REQUIRE(count < 3);
}

TEST_CASE("MatrixMultiply (MatrixMultiply)")
{
	integer n = 10;
	integer matrices = 100;

	integer count = 0;

	for (integer i = 0;i < matrices;++i)
	{
		Matrix<dreal> a = randomMatrix<dreal>(n, n);
		Matrix<dreal> b = randomMatrix<dreal>(n, n);

		VectorD v = randomVectorCube<dreal, Dynamic>(n);

		VectorD result1 = v * (a * b).eval();
		VectorD result2 = (v * a) * b;

		a *= b;

		VectorD result3 = v * a;

		dreal error1 = norm(result1 - result2);
		dreal error2 = norm(result3 - result2);
		if (error1 > 0.001 ||
			error2 > 0.001)
		{
			++count;
		}
	}

	REQUIRE(count < 3);
}

TEST_CASE("MatrixAssigns (MatrixAssigns)")
{
	// The idea here is to test
	// for an assignment with an expression
	// which involves the matrix itself.

	integer n = 10;
	integer matrices = 100;
	for (integer i = 0;i < matrices;++i)
	{
		Matrix<dreal> a = randomMatrix<dreal>(n, n);

		Matrix<dreal> b(n, n);
		b = a;

		REQUIRE(b == a);

		a += b;
		b += b;

		REQUIRE(a == b);

		a -= b;
		b -= b;

		REQUIRE(a == b);

		a *= b;
		b *= b;

		REQUIRE(a == b);

		a += identityMatrix<dreal>(n, n) + (5 * b);
		b += identityMatrix<dreal>(n, n) + (5 * b);

		REQUIRE(a == b);

		a += identityMatrix<dreal>(n, n) + (b * b);
		b += identityMatrix<dreal>(n, n) + (b * b);

		REQUIRE(a == b);
	}
}

TEST_CASE("MatrixSolve (MatrixSolve)")
{
	integer iterations = 100;
	integer n = 10;

	integer count = 0;

	for (integer i = 0;i < iterations;++i)
	{
		Matrix<dreal> a = randomMatrix<dreal>(n, n);
		Matrix<dreal> aCopy = a;

		VectorD b(randomVectorCube<dreal, Dynamic>(n));

		VectorD x(solveLinearInplace(view(aCopy), b));

		dreal error =
			norm(a * x - b);

		if (error > 0.001)
		{
			++count;
		}
	}

	REQUIRE(count < 3);
}

