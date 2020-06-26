// Description: Testing for Matrix
// DocumentationOf: matrix.h

#include "test/test_init.h"

#include "pastel/math/matrix.h"
#include "pastel/math/sampling.h"

#include "pastel/sys/view.h"

#include <algorithm>

namespace Pastel
{

	template class Matrix<dreal>;

}

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
	m = { -1, 2, 3,
		4, -5, 6 };

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
	m = {-1, 2, 3,
		4, -5, 6};

	{
		dreal correct = -1 + -5;
		REQUIRE(trace(m) == correct);
	}
}

TEST_CASE("DiagonalProduct (DiagonalProduct)")
{
	MatrixD m(2, 3);
	m = {-1, 2, 3,
		4, -5, 6};

	{
		dreal correct = -1 * -5;
		REQUIRE(diagonalProduct(m) == correct);
	}
}

TEST_CASE("Determinant (Determinant)")
{
	{
		MatrixD m(1, 1);
		m = -1;
		// VC2013 has a bug with singular initializer lists:
		//m = { -1 };
		{
			dreal correct = -1;
			REQUIRE(determinant(m) == correct);
		}
	}

	{
		MatrixD m(2, 2);
		m = {-1, 2,
			4, -5};
		{
			dreal correct = (-1 * -5) - (2 * 4);
			REQUIRE(determinant(m) == correct);
		}
	}

	{
		MatrixD m(3, 3);
		m = {-1, 2, 3,
				4, -5, 5,
				2, 3, 4};
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
		REQUIRE(empty.width() == 0);
		REQUIRE(empty.height() == 0);
	}

	// Constructs from a matrix expression.
	MatrixD m = identityMatrix<dreal>(2, 3) * 2 + 5;
	{
		dreal correctSet[] = 
		{
			7, 5, 5,
			5, 7, 5
		};

		REQUIRE(boost::equal(m.cRange(), range(correctSet)));
	}

	{
		MatrixD test(2, 3);
		test = {1, 2, 3,
			4, 5, 6};

		// Adds a matrix expression.
		test += identityMatrix<dreal>(2, 3);
		{
			dreal correctSet[] =
			{
				2, 2, 3,
				4, 6, 6
			};

			REQUIRE(boost::equal(test.cRange(), range(correctSet)));
		}

		// Subtracts a matrix expression.
		test -= identityMatrix<dreal>(2, 3);
		{
			dreal correctSet[] =
			{
				1, 2, 3,
				4, 5, 6
			};

			REQUIRE(boost::equal(test.cRange(), range(correctSet)));
		}

		// Multiplies with a matrix expression.
		test *= identityMatrix<dreal>(3, 2);
		{
			dreal correctSet[] =
			{
				1, 2,
				4, 5
			};

			REQUIRE(boost::equal(test.cRange(), range(correctSet)));
		}
	}

	// Constructs an mxn identity matrix.
	MatrixD a(4, 6);
	{
		dreal correctSet[] =
		{
			1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0,
			0, 0, 0, 1, 0, 0
		};

		REQUIRE(boost::equal(a.cRange(), range(correctSet)));
		REQUIRE(a.height() == 4);
		REQUIRE(a.width() == 6);
		REQUIRE(a.m() == 4);
		REQUIRE(a.n() == 6);
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
	MatrixD shared(4, 3, withAliasing(dataSet));
	{
		REQUIRE(boost::equal(shared.cRange(), range(dataSet)));
	}

	REQUIRE(shared.valid(0, 0));
	REQUIRE(!shared.valid(-1, 0));
	REQUIRE(!shared.valid(0, -1));
	REQUIRE(!shared.valid(4, 3));
	REQUIRE(!shared.valid(3, 3));
	REQUIRE(shared.valid(3, 2));

	REQUIRE(shared.involves(
		dataSet, dataSet + 1));
	REQUIRE(shared.involves(
		dataSet + shared.size() - 1, 
		dataSet + shared.size()));
	REQUIRE(!shared.involves(
		dataSet + shared.size(), 
		dataSet + shared.size() + 10));
	REQUIRE(!shared.involves(
		dataSet - 10, dataSet));

	// Element access
	for (integer i = 0;i < 12;++i)
	{
		REQUIRE(shared(i) == i + 1);
		REQUIRE(shared(i / 3, i % 3) == i + 1);
	}

	// Column ranges
	{
		dreal correctSet[] = 
		{
			2, 5, 8, 11
		};

		REQUIRE(boost::equal(shared.cColumnRange(1), range(correctSet)));
	}

	// Row ranges
	{
		dreal correctSet[] = 
		{
			4, 5, 6
		};

		REQUIRE(boost::equal(shared.cRowRange(1), range(correctSet)));
	}

	{
		MatrixD test(3, 2);
		test = {1, 2, 3,
			4, 5, 6};

		// Subtracts a constant from all elements.
		test -= 1;
		{
			dreal correctSet[] = 
			{
				0, 1, 2,
				3, 4, 5
			};

			REQUIRE(boost::equal(test.cRange(), range(correctSet)));
		}

		// Adds a constant to all elements.
		test += 1;
		{
			dreal correctSet[] = 
			{
				1, 2, 3,
				4, 5, 6
			};

			REQUIRE(boost::equal(test.cRange(), range(correctSet)));
		}

		// Multiplies all elements with a constant.
		test *= 2;
		{
			dreal correctSet[] = 
			{
				2, 4, 6,
				8, 10, 12
			};

			REQUIRE(boost::equal(test.cRange(), range(correctSet)));
		}

		// Divides all elements by a constant.
		test /= 2;
		{
			dreal correctSet[] = 
			{
				1, 2, 3,
				4, 5, 6
			};

			REQUIRE(boost::equal(test.cRange(), range(correctSet)));
		}
	}

	a = {1, 0, 1, 0, 1, 0,
		0, 1, 0, 1, 0, 1,
		2, 0, 1, 0, 1, 0,
		0, 2, 0, 1, 0, 1};
	{
		dreal correctSet[] = 
		{
			1, 0, 1, 0, 1, 0,
			0, 1, 0, 1, 0, 1,
			2, 0, 1, 0, 1, 0,
			0, 2, 0, 1, 0, 1
		};

		REQUIRE(boost::equal(a.cRange(), range(correctSet)));
	}

	// The 2x2 identity-matrix repeated 2 times vertically,
	// and 3 times horizontally.
	MatrixD b = repeat(
		identityMatrix<dreal>(2, 2),
		2, 3);
	{
		dreal correctSet[] = 
		{
			1, 0, 1, 0, 1, 0,
			0, 1, 0, 1, 0, 1,
			1, 0, 1, 0, 1, 0,
			0, 1, 0, 1, 0, 1
		};

		REQUIRE(boost::equal(b.cRange(), range(correctSet)));
	}

	// You can refer to a submatrix of a matrix.

	b(Vector2i(2, 0), Vector2i(4, 2)) = 
		identityMatrix<dreal>(2, 2) * 2;

	REQUIRE(a == b);

	MatrixD c(2, 9);
	c = {1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18};

	// You can assign a submatrix to a submatrix
	// inside the same matrix.

	c(Vector2i(0, 7), Vector2i(2, 9)) =
		c(Vector2i(0, 5), Vector2i(2, 7));

	MatrixD d(2, 9);
	d = {1, 2, 3, 4, 5, 6, 7, 6, 7,
		10, 11, 12, 13, 14, 15, 16, 15, 16};

	REQUIRE(c == d);

	/*
	c(Range(0, 1), Range(0, 2)) =
		c(Range(0, 1), Range(5, 3));
	*/

	// You can assign a submatrix to a submatrix
	// inside the same matrix, even if the data
	// ranges overlap. In this case a temporary
	// is created behind the scenes to guarantee
	// correct behaviour.
	// Note also that you can view the subranges
	// in a reversed manner by reversing the range
	// values.

	SubMatrix<dreal> v = c(Vector2i(0, 2), Vector2i(2, -1));
	SubMatrix<dreal> v2 = c(Vector2i(0, 0), Vector2i(2, 3));

	v2 = v;

	d = {3, 2, 1, 4, 5, 6, 7, 6, 7,
		12, 11, 10, 13, 14, 15, 16, 15, 16};

	REQUIRE(c == d);
}

TEST_CASE("SubMatrix (SubMatrix)")
{
	integer width = 4;
	integer height = 4;

	Matrix<real32> a(height, width);
	for (integer y = 0;y < height;++y)
	{
		for (integer x = 0;x < width;++x)
		{

			a(x, y) = x * y;
		}
	}

	// The matrix can also be viewed as a sequence
	// of values, so that algorithms from the
	// standard library can be used at will.

	Matrix<real32>::Iterator iter = a.begin();
	Matrix<real32>::Iterator iterEnd = a.end();

	integer i = 0;
	while(iter != iterEnd)
	{

		*iter = i;
		++iter;
		++i;
		REQUIRE(i <= a.size());
	}
	{
		real32 correctSet[] = 
		{
			0,  1,  2,  3,
			4,  5,  6,  7,
			8,  9,  10, 11,
			12, 13, 14, 15,
		};

		REQUIRE(boost::equal(a.cRange(), range(correctSet)));
	}

	std::random_shuffle(a.begin(), a.end());
	std::sort(a.begin(), a.end());
	{
		real32 correctSet[] = 
		{
			0,  1,  2,  3,
			4,  5,  6,  7,
			8,  9,  10, 11,
			12, 13, 14, 15,
		};

		REQUIRE(boost::equal(a.cRange(), range(correctSet)));
	}

	i = 0;
	iter = a.begin();
	while(iter != iterEnd)
	{
		++iter;
		++i;
	}
	{
		real32 correctSet[] = 
		{
			0,  1,  2,  3,
			4,  5,  6,  7,
			8,  9,  10, 11,
			12, 13, 14, 15,
		};

		REQUIRE(boost::equal(a.cRange(), range(correctSet)));
	}

	// Finally, a matrix can be viewed as
	// a collection of row vectors.

	for (integer j = 0;j < width;++j)
	{
		a.column(j) = unitAxis<real32, Dynamic>(height, j) * 2;
		REQUIRE(a(j, j) == 2);
		a.column(j) = evaluate(unitAxis<real32, Dynamic>(height, j) * 3);
		REQUIRE(a(j, j) == 3);
	}

	// Because of the way the matrix is stored,
	// it's width and height can be varied
	// without reallocation given that the
	// number of elements stays the same.
	// The data retains its row-major ordering.

	a.reshape(8, 2);
	{
		real32 correctSet[] = 
		{
			3, 0,
			0, 0,
			0, 3,
			0, 0,
			0, 0,
			3, 0,
			0, 0,
			0, 3
		};

		REQUIRE(boost::equal(a.cRange(), range(correctSet)));
	}
}

TEST_CASE("MatrixArray (MatrixArray)")
{
	MatrixD a(3, 3);
	a = {1, 2, 3,
			-2, 3, -4,
			7, -3, 2};

	Vector3 b = max(a);
	REQUIRE(b[0] == 7);
	REQUIRE(b[1] == 3);
	REQUIRE(b[2] == 3);

	Vector3 c = min(a);
	REQUIRE(c[0] == -2);
	REQUIRE(c[1] == -3);
	REQUIRE(c[2] == -4);
}

TEST_CASE("MatrixLowDimensional (MatrixLowDimensional)")
{
	{
		Matrix<dreal> a = matrix1x1<dreal>(5);
		REQUIRE(
			a(0, 0) == 5);
	}
	{
		Matrix<dreal> a = 
			matrix2x2<dreal>(
			1, 2,
			3, 4);
		REQUIRE(a(0, 0) == 1);
		REQUIRE(a(0, 1) == 2);
		REQUIRE(a(1, 0) == 3);
		REQUIRE(a(1, 1) == 4);

		Matrix<dreal> b(2, 2);

		b = a;
		REQUIRE(b(0, 0) == 1);
		REQUIRE(b(0, 1) == 2);
		REQUIRE(b(1, 0) == 3);
		REQUIRE(b(1, 1) == 4);

		Matrix<dreal> c(b);
		REQUIRE(c(0, 0) == 1);
		REQUIRE(c(0, 1) == 2);
		REQUIRE(c(1, 0) == 3);
		REQUIRE(c(1, 1) == 4);

	}

	{
		Matrix<dreal> a = 
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

		Matrix<dreal> b(3, 3);

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

		Matrix<dreal> c(b);
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
		Matrix<dreal> a =
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

		Matrix<dreal> b(4, 4);

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

		Matrix<dreal> c(b);
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
	Matrix<dreal> a(2, 3);

	a = {1, 2, 3,
		4, 5, 6};

	Matrix<dreal> b(3, 2);

	b = {7, 8,
		4, 3,
		3, 6};

	Matrix<dreal> c(a * b);
	REQUIRE(c(0, 0) == 1 * 7 + 2 * 4 + 3 * 3);
	REQUIRE(c(0, 1) == 1 * 8 + 2 * 3 + 3 * 6);
	REQUIRE(c(1, 0) == 4 * 7 + 5 * 4 + 6 * 3);
	REQUIRE(c(1, 1) == 4 * 8 + 5 * 3 + 6 * 6);

	Matrix<dreal> d(1, 3);
	d = {5, 2, 6};

	Matrix<dreal> e(3, 1);
	e(0, 0) = -3;
	e(1, 0) = 6;
	e(2, 0) = -4;
	Matrix<dreal> f(d * e);

	REQUIRE(f(0, 0) == 5 * -3 + 2 * 6 + 6 * -4);

	Matrix<dreal> g =
		matrix2x2<dreal>(
		1, 2,
		3, 4);

	g =
	{1, 2,
	3, 4};
	g *= 4;
	REQUIRE(g(0, 0) == 1 * 4);
	REQUIRE(g(0, 1) == 2 * 4);
	REQUIRE(g(1, 0) == 3 * 4);
	REQUIRE(g(1, 1) == 4 * 4);

	g =
		{1, 2,
		3, 4};
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

		VectorD result1 = v * (a * b);
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

		VectorD b(randomVectorCube<dreal, Dynamic>(n));

		VectorD x(solveLinear(a, b));

		dreal error =
			norm(a * x - b);

		if (error > 0.001)
		{
			++count;
		}
	}

	REQUIRE(count < 3);
}

