// Description: Testing for Matrix
// DocumentationOf: matrix.h

#include "test_pastelmath.h"

#include "pastel/math/matrix_algorithms.h"
#include "pastel/math/lu_decomposition.h"
#include "pastel/math/uniform_sampling.h"

#include "pastel/sys/views.h"

#include <algorithm>

using namespace Pastel;

namespace Pastel
{

	template class Matrix<real>;
	
}

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		typedef Matrix<real> MatrixD;

		virtual void run()
		{
			testNorm();
			testTrace();
			testDeterminant();
			testMatrixExpressions();
			testMatrixLowDimensional();
			testMatrixSimpleArithmetic();
			testMatrixSolve();
			testInverse();
			testMatrixMultiply();
			testMatrixAssigns();
			testSubMatrix();
			testMatrixArray();
			testImplicit();
		}

		void testImplicit()
		{
			auto f = [&](MatrixD matrix)
			{
			};

			// The matrix is implicitly constructible
			// from the matrix-expression.
			f(identityMatrix<real>(3, 3));
		}

		void testNorm()
		{
			MatrixD m(2, 3);
			m |= -1, 2, 3,
				 4, -5, 6;

			{
				real correct = 
					square(-1) + square(2) + square(3) +
					square(4) + square(-5) + square(6);
				TEST_ENSURE_OP(frobeniusNorm2(m), ==, correct);
				TEST_ENSURE_OP(frobeniusNorm(m), ==, std::sqrt(correct));
			}

			{
				real correct = 4 + 5 + 6;
				TEST_ENSURE_OP(maxNorm(m), ==, correct);
			}

			{
				real correct = 3 + 6;
				TEST_ENSURE_OP(manhattanNorm(m), ==, correct);
			}
		}

		void testTrace()
		{
			MatrixD m(2, 3);
			m |= -1, 2, 3,
				 4, -5, 6;
			
			{
				real correct = -1 + -5;
				TEST_ENSURE_OP(trace(m), ==, correct);
			}
		}

		void testDiagonalProduct()
		{
			MatrixD m(2, 3);
			m |= -1, 2, 3,
				 4, -5, 6;
			
			{
				real correct = -1 * -5;
				TEST_ENSURE_OP(diagonalProduct(m), ==, correct);
			}
		}

		void testDeterminant()
		{
			{
				MatrixD m(1, 1);
				m |= -1;
				{
					real correct = -1;
					TEST_ENSURE_OP(determinant(m), ==, correct);
				}
			}

			{
				MatrixD m(2, 2);
				m |= -1, 2,
					 4, -5;
				{
					real correct = (-1 * -5) - (2 * 4);
					TEST_ENSURE_OP(determinant(m), ==, correct);
				}
			}

			{
				MatrixD m(3, 3);
				m |= -1, 2, 3,
					 4, -5, 5,
					 2, 3, 4;
				{
					real correct = 89;
					TEST_ENSURE_OP(std::abs(determinant(m) - 89), <, 0.0001);
				}
			}

			{
				MatrixD m(10, 10);
				setRandomRotation(m);
				{
					TEST_ENSURE_OP(std::abs(determinant(m) - 1), <, 0.0001);
				}
			}
		}

		void testMatrixExpressions()
		{
			// A matrix can be filled manually in row-major 
			// order with the combination of |= and comma 
			// operators. Extraneous values are ignored.

			// Construct an empty matrix.
			MatrixD empty(0, 0);
			{
				TEST_ENSURE_OP(empty.size(), ==, 0);
				TEST_ENSURE_OP(empty.width(), ==, 0);
				TEST_ENSURE_OP(empty.height(), ==, 0);
			}

			// Constructs from a matrix expression.
			MatrixD m = identityMatrix<real>(2, 3) * 2 + 5;
			{
				real correctSet[] = 
				{
					7, 5, 5,
					5, 7, 5
				};

				TEST_ENSURE(boost::equal(m.cRange(), range(correctSet)));
			}

			{
				MatrixD test(2, 3);
				test |= 1, 2, 3,
					    4, 5, 6;

				// Adds a matrix expression.
				test += identityMatrix<real>(2, 3);
				{
					real correctSet[] =
					{
						2, 2, 3,
						4, 6, 6
					};

					TEST_ENSURE(boost::equal(test.cRange(), range(correctSet)));
				}

				// Subtracts a matrix expression.
				test -= identityMatrix<real>(2, 3);
				{
					real correctSet[] =
					{
						1, 2, 3,
						4, 5, 6
					};

					TEST_ENSURE(boost::equal(test.cRange(), range(correctSet)));
				}

				// Multiplies with a matrix expression.
				test *= identityMatrix<real>(3, 2);
				{
					real correctSet[] =
					{
						1, 2,
						4, 5
					};

					TEST_ENSURE(boost::equal(test.cRange(), range(correctSet)));
				}
			}

			// Constructs an mxn identity matrix.
			MatrixD a(4, 6);
			{
				real correctSet[] =
				{
					1, 0, 0, 0, 0, 0,
					0, 1, 0, 0, 0, 0,
					0, 0, 1, 0, 0, 0,
					0, 0, 0, 1, 0, 0
				};

				TEST_ENSURE(boost::equal(a.cRange(), range(correctSet)));
				TEST_ENSURE_OP(a.height(), ==, 4);
				TEST_ENSURE_OP(a.width(), ==, 6);
				TEST_ENSURE_OP(a.m(), ==, 4);
				TEST_ENSURE_OP(a.n(), ==, 6);
				TEST_ENSURE_OP(a.size(), ==, 4 * 6);
			}

			real dataSet[] = 
			{
				1, 2, 3,
				4, 5, 6,
				7, 8, 9,
				10, 11, 12
			};
			
			// Constructs from a shared array.
			MatrixD shared(4, 3, withAliasing(dataSet));
			{
				TEST_ENSURE(boost::equal(shared.cRange(), range(dataSet)));
			}

			TEST_ENSURE(shared.valid(0, 0));
			TEST_ENSURE(!shared.valid(-1, 0));
			TEST_ENSURE(!shared.valid(0, -1));
			TEST_ENSURE(!shared.valid(4, 3));
			TEST_ENSURE(!shared.valid(3, 3));
			TEST_ENSURE(shared.valid(3, 2));

			TEST_ENSURE(shared.involves(
				dataSet, dataSet + 1));
			TEST_ENSURE(shared.involves(
				dataSet + shared.size() - 1, 
				dataSet + shared.size()));
			TEST_ENSURE(!shared.involves(
				dataSet + shared.size(), 
				dataSet + shared.size() + 10));
			TEST_ENSURE(!shared.involves(
				dataSet - 10, dataSet));

			// Element access
			for (integer i = 0;i < 12;++i)
			{
				TEST_ENSURE(shared(i) == i + 1);
				TEST_ENSURE(shared(i / 3, i % 3) == i + 1);
			}

			// Column ranges
			{
				real correctSet[] = 
				{
					2, 5, 8, 11
				};

				TEST_ENSURE(boost::equal(shared.cColumnRange(1), range(correctSet)));
			}

			// Row ranges
			{
				real correctSet[] = 
				{
					4, 5, 6
				};

				TEST_ENSURE(boost::equal(shared.cRowRange(1), range(correctSet)));
			}

			{
				MatrixD test(3, 2);
				test |= 1, 2, 3,
					    4, 5, 6;
				
				// Subtracts a constant from all elements.
				test -= 1;
				{
					real correctSet[] = 
					{
						0, 1, 2,
						3, 4, 5
					};

					TEST_ENSURE(boost::equal(test.cRange(), range(correctSet)));
				}

				// Adds a constant to all elements.
				test += 1;
				{
					real correctSet[] = 
					{
						1, 2, 3,
						4, 5, 6
					};

					TEST_ENSURE(boost::equal(test.cRange(), range(correctSet)));
				}

				// Multiplies all elements with a constant.
				test *= 2;
				{
					real correctSet[] = 
					{
						2, 4, 6,
						8, 10, 12
					};

					TEST_ENSURE(boost::equal(test.cRange(), range(correctSet)));
				}

				// Divides all elements by a constant.
				test /= 2;
				{
					real correctSet[] = 
					{
						1, 2, 3,
						4, 5, 6
					};

					TEST_ENSURE(boost::equal(test.cRange(), range(correctSet)));
				}
			}

			a |= 1, 0, 1, 0, 1, 0,
				0, 1, 0, 1, 0, 1,
				2, 0, 1, 0, 1, 0,
				0, 2, 0, 1, 0, 1;
			{
				real correctSet[] = 
				{
					1, 0, 1, 0, 1, 0,
					0, 1, 0, 1, 0, 1,
					2, 0, 1, 0, 1, 0,
					0, 2, 0, 1, 0, 1
				};

				TEST_ENSURE(boost::equal(a.cRange(), range(correctSet)));
			}
			
			// The 2x2 identity-matrix repeated 2 times vertically,
			// and 3 times horizontally.
			MatrixD b = repeat(
				identityMatrix<real>(2, 2),
				2, 3);
			{
				real correctSet[] = 
				{
					1, 0, 1, 0, 1, 0,
					0, 1, 0, 1, 0, 1,
					1, 0, 1, 0, 1, 0,
					0, 1, 0, 1, 0, 1
				};

				TEST_ENSURE(boost::equal(b.cRange(), range(correctSet)));
			}

			// You can refer to a submatrix of a matrix.

			b(Vector2i(2, 0), Vector2i(4, 2)) = 
				identityMatrix<real>(2, 2) * 2;

			TEST_ENSURE(a == b);

			MatrixD c(2, 9);
			c |= 1, 2, 3, 4, 5, 6, 7, 8, 9,
				 10, 11, 12, 13, 14, 15, 16, 17, 18;

			// You can assign a submatrix to a submatrix
			// inside the same matrix.

			c(Vector2i(0, 7), Vector2i(2, 9)) =
				c(Vector2i(0, 5), Vector2i(2, 7));

			MatrixD d(2, 9);
			d |= 1, 2, 3, 4, 5, 6, 7, 6, 7,
				 10, 11, 12, 13, 14, 15, 16, 15, 16;

			TEST_ENSURE(c == d);

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

			SubMatrix<real> v = c(Vector2i(0, 2), Vector2i(2, -1));
			SubMatrix<real> v2 = c(Vector2i(0, 0), Vector2i(2, 3));

			v2 = v;

			d |= 3, 2, 1, 4, 5, 6, 7, 6, 7,
				 12, 11, 10, 13, 14, 15, 16, 15, 16;

			TEST_ENSURE(c == d);
		}

		void testSubMatrix()
		{
			const integer width = 4;
			const integer height = 4;

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
			const Matrix<real32>::Iterator iterEnd = a.end();

			integer i = 0;
			while(iter != iterEnd)
			{
				*iter = i;
				++iter;
				++i;
				TEST_ENSURE_OP(i, <=, a.size());
			}
			{
				real32 correctSet[] = 
				{
					0,  1,  2,  3,
					4,  5,  6,  7,
					8,  9,  10, 11,
					12, 13, 14, 15,
				};

				TEST_ENSURE(boost::equal(a.cRange(), range(correctSet)));
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

				TEST_ENSURE(boost::equal(a.cRange(), range(correctSet)));
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

				TEST_ENSURE(boost::equal(a.cRange(), range(correctSet)));
			}

			// Finally, a matrix can be viewed as
			// a collection of row vectors.

			for (integer j = 0;j < width;++j)
			{
				a.column(j) = unitAxis<real32, Dynamic>(height, j) * 2;
				TEST_ENSURE_OP(a(j, j), ==, 2);
				a.column(j) = evaluate(unitAxis<real32, Dynamic>(height, j) * 3);
				TEST_ENSURE_OP(a(j, j), ==, 3);
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

				TEST_ENSURE(boost::equal(a.cRange(), range(correctSet)));
			}
		}

		void testMatrixArray()
		{
			MatrixD a(3, 3);
			a |= 1, 2, 3,
				 -2, 3, -4,
				 7, -3, 2;

			Vector3 b = max(a);
			TEST_ENSURE_OP(b[0], ==, 7);
			TEST_ENSURE_OP(b[1], ==, 3);
			TEST_ENSURE_OP(b[2], ==, 3);

			Vector3 c = min(a);
			TEST_ENSURE_OP(c[0], ==, -2);
			TEST_ENSURE_OP(c[1], ==, -3);
			TEST_ENSURE_OP(c[2], ==, -4);
		}

		void testMatrixLowDimensional()
		{
			{
				Matrix<real> a = matrix1x1<real>(5);
				TEST_ENSURE(
					a(0, 0) == 5);
			}
			{
				Matrix<real> a = 
					matrix2x2<real>(
					1, 2,
					3, 4);
				TEST_ENSURE(
					a(0, 0) == 1 && a(0, 1) == 2 &&
					a(1, 0) == 3 && a(1, 1) == 4);

				Matrix<real> b(2, 2);

				b = a;
				TEST_ENSURE(
					b(0, 0) == 1 && b(0, 1) == 2 &&
					b(1, 0) == 3 && b(1, 1) == 4);

				Matrix<real> c(b);
				TEST_ENSURE(
					c(0, 0) == 1 && c(0, 1) == 2 &&
					c(1, 0) == 3 && c(1, 1) == 4);
			}

			{
				Matrix<real> a = 
					matrix3x3<real>(
					1, 2, 3,
					4, 5, 6,
					7, 8, 9);
				TEST_ENSURE(
					a(0, 0) == 1 && a(0, 1) == 2 && a(0, 2) == 3 &&
					a(1, 0) == 4 && a(1, 1) == 5 && a(1, 2) == 6 &&
					a(2, 0) == 7 && a(2, 1) == 8 && a(2, 2) == 9);

				Matrix<real> b(3, 3);

				b = a;
				TEST_ENSURE(
					b(0, 0) == 1 && b(0, 1) == 2 && b(0, 2) == 3 &&
					b(1, 0) == 4 && b(1, 1) == 5 && b(1, 2) == 6 &&
					b(2, 0) == 7 && b(2, 1) == 8 && b(2, 2) == 9);

				Matrix<real> c(b);
				TEST_ENSURE(
					c(0, 0) == 1 && c(0, 1) == 2 && c(0, 2) == 3 &&
					c(1, 0) == 4 && c(1, 1) == 5 && c(1, 2) == 6 &&
					c(2, 0) == 7 && c(2, 1) == 8 && c(2, 2) == 9);
			}
			{
				Matrix<real> a =
					matrix4x4<real>(
					1, 2, 3, 4,
					5, 6, 7, 8,
					9, 10, 11, 12,
					13, 14, 15, 16);
				TEST_ENSURE(
					a(0, 0) == 1 && a(0, 1) == 2 &&
					a(0, 2) == 3 && a(0, 3) == 4 &&
					a(1, 0) == 5 && a(1, 1) == 6 &&
					a(1, 2) == 7 && a(1, 3) == 8 &&
					a(2, 0) == 9 && a(2, 1) == 10 &&
					a(2, 2) == 11 && a(2, 3) == 12 &&
					a(3, 0) == 13 && a(3, 1) == 14 &&
					a(3, 2) == 15 && a(3, 3) == 16);

				Matrix<real> b(4, 4);

				b = a;
				TEST_ENSURE(
					b(0, 0) == 1 && b(0, 1) == 2 &&
					b(0, 2) == 3 && b(0, 3) == 4 &&
					b(1, 0) == 5 && b(1, 1) == 6 &&
					b(1, 2) == 7 && b(1, 3) == 8 &&
					b(2, 0) == 9 && b(2, 1) == 10 &&
					b(2, 2) == 11 && b(2, 3) == 12 &&
					b(3, 0) == 13 && b(3, 1) == 14 &&
					b(3, 2) == 15 && b(3, 3) == 16);

				Matrix<real> c(b);
				TEST_ENSURE(
					c(0, 0) == 1 && c(0, 1) == 2 &&
					c(0, 2) == 3 && c(0, 3) == 4 &&
					c(1, 0) == 5 && c(1, 1) == 6 &&
					c(1, 2) == 7 && c(1, 3) == 8 &&
					c(2, 0) == 9 && c(2, 1) == 10 &&
					c(2, 2) == 11 && c(2, 3) == 12 &&
					c(3, 0) == 13 && c(3, 1) == 14 &&
					c(3, 2) == 15 && c(3, 3) == 16);
			}
		}

		void testMatrixSimpleArithmetic()
		{
			Matrix<real> a(2, 3);

			a |= 1, 2, 3,
				4, 5, 6;

			Matrix<real> b(3, 2);

			b |= 7, 8,
				4, 3,
				3, 6;

			Matrix<real> c(a * b);
			TEST_ENSURE(
				c(0, 0) == 1 * 7 + 2 * 4 + 3 * 3 &&
				c(0, 1) == 1 * 8 + 2 * 3 + 3 * 6 &&
				c(1, 0) == 4 * 7 + 5 * 4 + 6 * 3 &&
				c(1, 1) == 4 * 8 + 5 * 3 + 6 * 6);

			Matrix<real> d(1, 3);
			d |= 5, 2, 6;

			Matrix<real> e(3, 1);
			e(0, 0) = -3;
			e(1, 0) = 6;
			e(2, 0) = -4;
			Matrix<real> f(d * e);

			TEST_ENSURE_OP(f(0, 0), ==, 5 * -3 + 2 * 6 + 6 * -4);

			Matrix<real> g = 
				matrix2x2<real>(
				1, 2,
				3, 4);

			g |=
				1, 2,
				3, 4;
			g *= 4;
			TEST_ENSURE(
				g(0, 0) == 1 * 4 && g(0, 1) == 2 * 4 &&
				g(1, 0) == 3 * 4 && g(1, 1) == 4 * 4);

			g |=
				1, 2,
				3, 4;
			g /= 4;
			TEST_ENSURE(
				g(0, 0) == (real)1 / 4 && g(0, 1) == (real)2 / 4 &&
				g(1, 0) == (real)3 / 4 && g(1, 1) == (real)4 / 4);
		}

		void testInverse()
		{
			const integer n = 10;
			const integer matrices = 100;

			integer count = 0;

			for (integer i = 0;i < matrices;++i)
			{
				Matrix<real> m(n, n);
				setRandomMatrix(m);

				const Matrix<real> mInv = inverse(m);

				const real leftError = 
					manhattanNorm(m * mInv - identityMatrix<real>(n, n));
				const real rightError = 
					manhattanNorm(mInv * m - identityMatrix<real>(n, n));
				if (leftError > 0.001 ||
					rightError > 0.001)
				{
					++count;
				}
			}

			TEST_ENSURE_OP(count, <, 3);
		}

		void testMatrixMultiply()
		{
			const integer n = 10;
			const integer matrices = 100;

			integer count = 0;

			for (integer i = 0;i < matrices;++i)
			{
				Matrix<real> a(n, n);
				setRandomMatrix(a);

				Matrix<real> b(n, n);
				setRandomMatrix(b);

				VectorD v = randomVectorCube<real, Dynamic>(n);

				VectorD result1 = v * (a * b);
				VectorD result2 = (v * a) * b;

				a *= b;

				VectorD result3 = v * a;

				const real error1 = norm(result1 - result2);
				const real error2 = norm(result3 - result2);
				if (error1 > 0.001 ||
					error2 > 0.001)
				{
					++count;
				}
			}

			TEST_ENSURE_OP(count, <, 3);
		}

		void testMatrixAssigns()
		{
			// The idea here is to test
			// for an assignment with an expression
			// which involves the matrix itself.

			const integer n = 10;
			const integer matrices = 100;
			for (integer i = 0;i < matrices;++i)
			{
				Matrix<real> a(n, n);
				setRandomMatrix(a);

				Matrix<real> b(n, n);
				b = a;

				TEST_ENSURE(b == a);

				a += b;
				b += b;
				
				TEST_ENSURE(a == b);

				a -= b;
				b -= b;
				
				TEST_ENSURE(a == b);

				a *= b;
				b *= b;
				
				TEST_ENSURE(a == b);

				a += identityMatrix<real>(n, n) + (5 * b);
				b += identityMatrix<real>(n, n) + (5 * b);
				
				TEST_ENSURE(a == b);

				a += identityMatrix<real>(n, n) + (b * b);
				b += identityMatrix<real>(n, n) + (b * b);

				TEST_ENSURE(a == b);
			}
		}

		void testMatrixSolve()
		{
			const integer iterations = 100;
			const integer n = 10;

			integer count = 0;

			for (integer i = 0;i < iterations;++i)
			{
				Matrix<real> a(n, n);
				setRandomMatrix(a);

				const VectorD b(randomVectorCube<real, Dynamic>(n));

				const VectorD x(solveLinear(a, b));

				const real error =
					norm(a * x - b);

				if (error > 0.001)
				{
					++count;
				}
			}

			TEST_ENSURE_OP(count, <, 3);
		}

	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Matrix", test);
	}

	CallFunction run(addTest);

}

