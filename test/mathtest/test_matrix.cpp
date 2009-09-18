#include "pastelmathtest.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/ludecomposition_tools.h"

#include "pastel/math/uniform_sampling.h"
#include "pastel/sys/view_all.h"

#include "pastel/gfx/pcx.h"

#include <algorithm>

using namespace Pastel;

namespace
{

	class TestMatrix
		: public TestSuite
	{
	public:
		TestMatrix()
			: TestSuite(&mathTestReport())
		{
		}

		virtual void run()
		{
			testMatrixExpressions();
			testMatrixLowDimensional();

			testMatrixSimpleArithmetic();

			testMatrixSolve<1>();
			testMatrixSolve<2>();
			testMatrixSolve<3>();
			testMatrixSolve<4>();
			testMatrixSolve<5>();
			testMatrixSolve<Dynamic>();

			testMatrixInverse<1>();
			testMatrixInverse<2>();
			testMatrixInverse<3>();
			testMatrixInverse<4>();
			testMatrixInverse<5>();
			testMatrixInverse<Dynamic>();

			testMatrixMultiply<1>();
			testMatrixMultiply<2>();
			testMatrixMultiply<3>();
			testMatrixMultiply<4>();
			testMatrixMultiply<5>();
			testMatrixMultiply<Dynamic>();

			testMatrixAssigns<1>();
			testMatrixAssigns<2>();
			testMatrixAssigns<3>();
			testMatrixAssigns<4>();
			testMatrixAssigns<5>();
			testMatrixAssigns<Dynamic>();

			testMatrixCondition<2>();
			testMatrixCondition<3>();
			testMatrixCondition<4>();
			testMatrixCondition<5>();
			testMatrixCondition<Dynamic>();

			testSubMatrix();
			testMatrixArray();
		}

		void testMatrixExpressions()
		{
			// A matrix can be filled manually in row-major 
			// order with the combination of |= and comma 
			// operators. Extraneous values are ignored.

			MatrixD a(4, 6);
			a |= 1, 0, 1, 0, 1, 0,
				0, 1, 0, 1, 0, 1,
				2, 0, 1, 0, 1, 0,
				0, 2, 0, 1, 0, 1;
			
			MatrixD b = repeat(
				identityMatrix<real, Dynamic, Dynamic>(2, 2),
				2, 3);

			// You can refer to a submatrix of a matrix.

			b(Vector2i(2, 0), Vector2i(4, 2)) = 
				identityMatrix<real, Dynamic, Dynamic>(2, 2) * 2;

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

			std::cout << c << std::endl;

			TEST_ENSURE(c == d);
		}

		void testSubMatrix()
		{
			const integer width = 4;
			const integer height = 4;

			// A matrix can be accessed through
			// a view interface. The convention
			// for index ordering is opposite
			// for the views.

			Matrix<real32, Dynamic, Dynamic> a(height, width);
			for (integer y = 0;y < height;++y)
			{
				for (integer x = 0;x < width;++x)
				{
					const real32 value = x * y;
					a.view()(x, y) = value;
			
					// The matrix can also be indexed
					// directly.

					TEST_ENSURE_OP(a(y, x), ==, value);
				}
			}
			
			// Demonstrating the modification of a matrix
			// via view interface.

			copy(constSubView(a.constView(), Rectangle2(0, 0, 2, 2)),
				subView(a.view(), Rectangle2(2, 2, 4, 4)));
			
			TEST_ENSURE_OP(a(2, 2), ==, a(0, 0));
			TEST_ENSURE_OP(a(3, 2), ==, a(1, 0));
			TEST_ENSURE_OP(a(3, 3), ==, a(1, 1));
			TEST_ENSURE_OP(a(2, 3), ==, a(0, 1));

			// The matrix can also be viewed as a sequence
			// of values, so that algorithms from the
			// standard library can be used at will.

			Matrix<real32, Dynamic, Dynamic>::Iterator iter = a.begin();
			const Matrix<real32, Dynamic, Dynamic>::Iterator iterEnd = a.end();

			integer i = 0;
			while(iter != iterEnd)
			{
				*iter = i;
				++iter;
				++i;
				TEST_ENSURE_OP(i, <=, a.size());
			}

			std::random_shuffle(a.begin(), a.end());
			std::sort(a.begin(), a.end());

			i = 0;
			iter = a.begin();
			while(iter != iterEnd)
			{
				TEST_ENSURE_OP(*iter, ==, i);
				++iter;
				++i;
			}

			// Finally, a matrix can be viewed as
			// a collection of row vectors.

			for (integer j = 0;j < height;++j)
			{
				a[j] = unitAxis<real32, Dynamic>(width, j) * 2;
				TEST_ENSURE_OP(a[j][j], ==, 2);
				a[j] = evaluate(unitAxis<real32, Dynamic>(width, j) * 3);
				TEST_ENSURE_OP(a[j][j], ==, 3);
			}

			// A const view can be adapted to a matrix expression:

			Array<real32, 2> b(height, width);
			
			a *= asMatrix(constArrayView(b));

			// Because of the way the matrix is stored,
			// it's width and height can be varied
			// without reallocation given that the
			// number of elements stays the same.
			// The data retains its row-major ordering.

			a.reshape(8, 2);

			// The array data structure underlying the matrix
			// can be obtained for non-mutable purposes.

			saveGrayscalePcx(a.asArray(), "test_matrix_asarray.pcx");
		}

		void testMatrixArray()
		{
			Matrix3 a;
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
				Matrix<real, 1, 1> a(5);
				TEST_ENSURE(
					a(0, 0) == 5);
			}
			{
				Matrix<real, 2, 2> a(
					1, 2,
					3, 4);
				TEST_ENSURE(
					a(0, 0) == 1 && a(0, 1) == 2 &&
					a(1, 0) == 3 && a(1, 1) == 4);

				Matrix<real, 2, 2> b;

				b = a;
				TEST_ENSURE(
					b(0, 0) == 1 && b(0, 1) == 2 &&
					b(1, 0) == 3 && b(1, 1) == 4);

				Matrix<real, 2, 2> c(b);
				TEST_ENSURE(
					c(0, 0) == 1 && c(0, 1) == 2 &&
					c(1, 0) == 3 && c(1, 1) == 4);
			}

			{
				Matrix<real, 3, 3> a(
					1, 2, 3,
					4, 5, 6,
					7, 8, 9);
				TEST_ENSURE(
					a(0, 0) == 1 && a(0, 1) == 2 && a(0, 2) == 3 &&
					a(1, 0) == 4 && a(1, 1) == 5 && a(1, 2) == 6 &&
					a(2, 0) == 7 && a(2, 1) == 8 && a(2, 2) == 9);

				Matrix<real, 3, 3> b;

				b = a;
				TEST_ENSURE(
					b(0, 0) == 1 && b(0, 1) == 2 && b(0, 2) == 3 &&
					b(1, 0) == 4 && b(1, 1) == 5 && b(1, 2) == 6 &&
					b(2, 0) == 7 && b(2, 1) == 8 && b(2, 2) == 9);

				Matrix<real, 3, 3> c(b);
				TEST_ENSURE(
					c(0, 0) == 1 && c(0, 1) == 2 && c(0, 2) == 3 &&
					c(1, 0) == 4 && c(1, 1) == 5 && c(1, 2) == 6 &&
					c(2, 0) == 7 && c(2, 1) == 8 && c(2, 2) == 9);
			}
			{
				Matrix<real, 4, 4> a(
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

				Matrix<real, 4, 4> b;

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

				Matrix<real, 4, 4> c(b);
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
			Matrix<real, 2, 3> a;

			a |= 1, 2, 3,
				4, 5, 6;

			Matrix<real, 3, 2> b;

			b |= 7, 8,
				4, 3,
				3, 6;

			Matrix<real, 2, 2> c(a * b);
			TEST_ENSURE(
				c(0, 0) == 1 * 7 + 2 * 4 + 3 * 3 &&
				c(0, 1) == 1 * 8 + 2 * 3 + 3 * 6 &&
				c(1, 0) == 4 * 7 + 5 * 4 + 6 * 3 &&
				c(1, 1) == 4 * 8 + 5 * 3 + 6 * 6);

			Matrix<real, 1, 3> d;
			d |= 5, 2, 6;

			Matrix<real, 3, 1> e;
			e(0, 0) = -3;
			e(1, 0) = 6;
			e(2, 0) = -4;
			Matrix<real, 1, 1> f(d * e);

			TEST_ENSURE_OP(f(0, 0), ==, 5 * -3 + 2 * 6 + 6 * -4);

			Matrix<real, 2, 2> g(
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

		template <int N>
		void testMatrixInverse()
		{
			const integer n = (N == Dynamic) ? 10 : N;

			const integer matrices = 1000;

			integer count = 0;

			for (integer i = 0;i < matrices;++i)
			{
				Matrix<real, N, N> m(n, n);
				setRandomMatrix(m);

				const Matrix<real, N, N> mInv = inverse(m);

				const real leftError = 
					normManhattan(m * mInv - identityMatrix<real, N, N>(n, n));
				const real rightError = 
					normManhattan(mInv * m - identityMatrix<real, N, N>(n, n));
				if (leftError > 0.001 ||
					rightError > 0.001)
				{
					++count;
				}
			}

			TEST_ENSURE_OP(count, <, 10);
		}

		template <int N>
		void testMatrixMultiply()
		{
			const integer n = (N == Dynamic) ? 10 : N;

			const integer matrices = 1000;

			integer count = 0;

			for (integer i = 0;i < matrices;++i)
			{
				Matrix<real, N, N> a(n, n);
				setRandomMatrix(a);

				Matrix<real, N, N> b(n, n);
				setRandomMatrix(b);

				Vector<real, N> v = randomVectorCube<N, real>(n);

				Vector<real, N> result1 = v * (a * b);
				Vector<real, N> result2 = (v * a) * b;

				a *= b;

				Vector<real, N> result3 = v * a;

				const real error1 = norm(result1 - result2);
				const real error2 = norm(result3 - result2);
				if (error1 > 0.001 ||
					error2 > 0.001)
				{
					++count;
				}
			}

			TEST_ENSURE_OP(count, <, 10);
		}

		template <int N>
		void testMatrixAssigns()
		{
			// The idea here is to test
			// for an assignment with an expression
			// which involves the matrix itself.

			const integer n = (N == Dynamic) ? 10 : N;

			const integer matrices = 1000;
			for (integer i = 0;i < matrices;++i)
			{
				Matrix<real, N, N> a(n, n);
				setRandomMatrix(a);

				Matrix<real, N, N> b(n, n);
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

				a += identityMatrix<real, N, N>(n, n) + (5 * b);
				b += identityMatrix<real, N, N>(n, n) + (5 * b);
				
				TEST_ENSURE(a == b);

				a += identityMatrix<real, N, N>(n, n) + (b * b);
				b += identityMatrix<real, N, N>(n, n) + (b * b);

				TEST_ENSURE(a == b);
			}
		}

		template <int N>
		void testMatrixSolve()
		{
			const integer iterations = 1000;

			const integer n = (N == Dynamic) ? 10 : N;

			integer count = 0;

			for (integer i = 0;i < iterations;++i)
			{
				Matrix<real, N, N> a(n, n);
				setRandomMatrix(a);

				const Vector<real, N> b(randomVectorCube<N, real>(n));

				const Vector<real, N> x(solveLinear(a, b));

				const real error =
					norm(x * a - b);

				if (error > 0.001)
				{
					++count;
				}
			}

			TEST_ENSURE_OP(count, <, 10);
		}

		template <int N>
		void testMatrixCondition()
		{
			const integer iterations = 1000;

			const integer n = (N == Dynamic) ? 10 : N;

			integer count = 0;

			for (integer i = 0;i < iterations;++i)
			{
				const real cond = 1 + random<real>() * 9;
				const real det = 1 + random<real>() * 9;

				Matrix<real, N, N> a(n, n);
				setRandomSymmetricPositiveDefinite(det, cond, a);

				const real detError = 
					mabs(determinant(a) - det);
				TEST_ENSURE_OP(detError, <, 0.01);

				/*
				const real condError =
					mabs(conditionManhattan(a) - cond);
				TEST_ENSURE_OP(condError, <, 0.01);
				*/

				if (detError > 0.01)
				{
					++count;
				}
			}

			TEST_ENSURE_OP(count, <, 10);
		}
	};

	void testMatrix()
	{
		TestMatrix test;
		test.run();
	}

	void testAdd()
	{
		mathTestList().add("Matrix", testMatrix);
	}

	CallFunction run(testAdd);

}
