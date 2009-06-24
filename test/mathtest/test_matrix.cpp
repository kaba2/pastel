#include "pastelmathtest.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/ludecomposition_tools.h"

#include "pastel/sys/random_vector.h"
#include "pastel/sys/view_all.h"

#include <algorithm>

using namespace Pastel;

namespace
{

	void testMatrixView()
	{
		const integer width = 4;
		const integer height = 4;

		// A matrix can be accessed through
		// a view interface. The convention
		// for index ordering is opposite
		// for the views.

		Matrix<Dynamic, Dynamic, real> a(height, width);
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				const real value = x * y;
				a.view()(x, y) = value;
		
				// The matrix can also be indexed
				// directly.

				REPORT(a(y, x) != value);
			}
		}
		
		// Demonstrating the modification of a matrix
		// via view interface.

		copy(constSubView(a.constView(), Rectangle2(0, 0, 2, 2)),
			subView(a.view(), Rectangle2(2, 2, 4, 4)));
		
		REPORT(a(2, 2) != a(0, 0));
		REPORT(a(3, 2) != a(1, 0));
		REPORT(a(3, 3) != a(1, 1));
		REPORT(a(2, 3) != a(0, 1));

		// The matrix can also be viewed as a sequence
		// of values, so that algorithms from the
		// standard library can be used at will.

		Matrix<Dynamic, Dynamic, real>::Iterator iter = a.begin();
		const Matrix<Dynamic, Dynamic, real>::Iterator iterEnd = a.end();

		integer i = 0;
		while(iter != iterEnd)
		{
			*iter = i;
			++iter;
			++i;
			ENSURE(i <= a.size());
		}

		std::random_shuffle(a.begin(), a.end());
		std::sort(a.begin(), a.end());

		i = 0;
		iter = a.begin();
		while(iter != iterEnd)
		{
			REPORT(*iter != i);
			++iter;
			++i;
		}

		// Finally, a matrix can be viewed as
		// a collection of row vectors.

		for (integer j = 0;j < height;++j)
		{
			a[j] = unitAxis<Dynamic, real>(width, j) * 2;
			REPORT(a[j][j] != 2);
			a[j] = evaluate(unitAxis<Dynamic, real>(width, j) * 3);
			REPORT(a[j][j] != 3);
		}

		// A const view can be adapted to a matrix expression:

		Array<2, real> b(height, width);
		
		a *= asMatrix(constArrayView(b));
	}

	void testMatrixLowDimensional()
	{
		{
			Matrix<1, 1, real> a(5);
			REPORT(
				a(0, 0) != 5);
		}
		{
			Matrix<2, 2, real> a(
				1, 2,
				3, 4);
			REPORT(
				a(0, 0) != 1 || a(0, 1) != 2 ||
				a(1, 0) != 3 || a(1, 1) != 4);

			Matrix<2, 2, real> b;

			b = a;
			REPORT(
				b(0, 0) != 1 || b(0, 1) != 2 ||
				b(1, 0) != 3 || b(1, 1) != 4);

			Matrix<2, 2, real> c(b);
			REPORT(
				c(0, 0) != 1 || c(0, 1) != 2 ||
				c(1, 0) != 3 || c(1, 1) != 4);
		}

		{
			Matrix<3, 3, real> a(
				1, 2, 3,
				4, 5, 6,
				7, 8, 9);
			REPORT(
				a(0, 0) != 1 || a(0, 1) != 2 || a(0, 2) != 3 ||
				a(1, 0) != 4 || a(1, 1) != 5 || a(1, 2) != 6 ||
				a(2, 0) != 7 || a(2, 1) != 8 || a(2, 2) != 9);

			Matrix<3, 3, real> b;

			b = a;
			REPORT(
				b(0, 0) != 1 || b(0, 1) != 2 || b(0, 2) != 3 ||
				b(1, 0) != 4 || b(1, 1) != 5 || b(1, 2) != 6 ||
				b(2, 0) != 7 || b(2, 1) != 8 || b(2, 2) != 9);

			Matrix<3, 3, real> c(b);
			REPORT(
				c(0, 0) != 1 || c(0, 1) != 2 || c(0, 2) != 3 ||
				c(1, 0) != 4 || c(1, 1) != 5 || c(1, 2) != 6 ||
				c(2, 0) != 7 || c(2, 1) != 8 || c(2, 2) != 9);
		}
		{
			Matrix<4, 4, real> a(
				1, 2, 3, 4,
				5, 6, 7, 8,
				9, 10, 11, 12,
				13, 14, 15, 16);
			REPORT(
				a(0, 0) != 1 || a(0, 1) != 2 ||
				a(0, 2) != 3 || a(0, 3) != 4 ||
				a(1, 0) != 5 || a(1, 1) != 6 ||
				a(1, 2) != 7 || a(1, 3) != 8 ||
				a(2, 0) != 9 || a(2, 1) != 10 ||
				a(2, 2) != 11 || a(2, 3) != 12 ||
				a(3, 0) != 13 || a(3, 1) != 14 ||
				a(3, 2) != 15 || a(3, 3) != 16);

			Matrix<4, 4, real> b;

			b = a;
			REPORT(
				b(0, 0) != 1 || b(0, 1) != 2 ||
				b(0, 2) != 3 || b(0, 3) != 4 ||
				b(1, 0) != 5 || b(1, 1) != 6 ||
				b(1, 2) != 7 || b(1, 3) != 8 ||
				b(2, 0) != 9 || b(2, 1) != 10 ||
				b(2, 2) != 11 || b(2, 3) != 12 ||
				b(3, 0) != 13 || b(3, 1) != 14 ||
				b(3, 2) != 15 || b(3, 3) != 16);

			Matrix<4, 4, real> c(b);
			REPORT(
				c(0, 0) != 1 || c(0, 1) != 2 ||
				c(0, 2) != 3 || c(0, 3) != 4 ||
				c(1, 0) != 5 || c(1, 1) != 6 ||
				c(1, 2) != 7 || c(1, 3) != 8 ||
				c(2, 0) != 9 || c(2, 1) != 10 ||
				c(2, 2) != 11 || c(2, 3) != 12 ||
				c(3, 0) != 13 || c(3, 1) != 14 ||
				c(3, 2) != 15 || c(3, 3) != 16);
		}
	}

	void testMatrixSimpleArithmetic()
	{
		Matrix<2, 3, real> a;

		a[0].set(1, 2, 3);
		a[1].set(4, 5, 6);

		Matrix<3, 2, real> b;
		b[0].set(7, 8);
		b[1].set(4, 3);
		b[2].set(3, 6);

		Matrix<2, 2, real> c(a * b);
		REPORT(
			c(0, 0) != 1 * 7 + 2 * 4 + 3 * 3 ||
			c(0, 1) != 1 * 8 + 2 * 3 + 3 * 6 ||
			c(1, 0) != 4 * 7 + 5 * 4 + 6 * 3 ||
			c(1, 1) != 4 * 8 + 5 * 3 + 6 * 6);

		Matrix<1, 3, real> d;
		d[0].set(5, 2, 6);

		Matrix<3, 1, real> e;
		e(0, 0) = -3;
		e(1, 0) = 6;
		e(2, 0) = -4;
		Matrix<1, 1, real> f(d * e);

		REPORT(f(0, 0) != 5 * -3 + 2 * 6 + 6 * -4);

		Matrix<2, 2, real> g(
			1, 2,
			3, 4);

		g.set(
			1, 2,
			3, 4);
		g *= 4;
		REPORT(
			g(0, 0) != 1 * 4 || g(0, 1) != 2 * 4 ||
			g(1, 0) != 3 * 4 || g(1, 1) != 4 * 4);

		g.set(
			1, 2,
			3, 4);
		g /= 4;
		REPORT(
			g(0, 0) != (real)1 / 4 || g(0, 1) != (real)2 / 4 ||
			g(1, 0) != (real)3 / 4 || g(1, 1) != (real)4 / 4);
	}

	template <int N>
	void testMatrixInverse()
	{
		const integer n = (N == Dynamic) ? 10 : N;

		const integer matrices = 1000;

		integer count = 0;

		for (integer i = 0;i < matrices;++i)
		{
			Matrix<N, N, real> m(n, n);
			setRandomMatrix(m);

			const Matrix<N, N, real> mInv = inverse(m);

			const real leftError = 
				normManhattan(m * mInv - identityMatrix<N, N, real>(n, n));
			const real rightError = 
				normManhattan(mInv * m - identityMatrix<N, N, real>(n, n));
			if (leftError > 0.001 ||
				rightError > 0.001)
			{
				++count;
			}
		}

		REPORT1(count > 10, count);
	}

	template <int N>
	void testMatrixMultiply()
	{
		const integer n = (N == Dynamic) ? 10 : N;

		const integer matrices = 1000;

		integer count = 0;

		for (integer i = 0;i < matrices;++i)
		{
			Matrix<N, N, real> a(n, n);
			setRandomMatrix(a);

			Matrix<N, N, real> b(n, n);
			setRandomMatrix(b);

			Vector<N, real> v = randomVectorCube<N, real>(n);

			Vector<N, real> result1 = v * (a * b);
			Vector<N, real> result2 = (v * a) * b;

			a *= b;

			Vector<N, real> result3 = v * a;

			const real error1 = norm(result1 - result2);
			const real error2 = norm(result3 - result2);
			if (error1 > 0.001 ||
				error2 > 0.001)
			{
				++count;
			}
		}

		REPORT1(count > 10, count);
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
			Matrix<N, N, real> a(n, n);
			setRandomMatrix(a);

			Matrix<N, N, real> b(n, n);
			b = a;

			ENSURE(b == a);

			a += b;
			b += b;
			
			ENSURE(a == b);

			a -= b;
			b -= b;
			
			ENSURE(a == b);

			a *= b;
			b *= b;
			
			ENSURE(a == b);

			a += identityMatrix<N, N, real>(n, n) + (5 * b);
			b += identityMatrix<N, N, real>(n, n) + (5 * b);
			
			ENSURE(a == b);

			a += identityMatrix<N, N, real>(n, n) + (b * b);
			b += identityMatrix<N, N, real>(n, n) + (b * b);

			ENSURE(a == b);
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
			Matrix<N, N, real> a(n, n);
			setRandomMatrix(a);

			const Vector<N, real> b(randomVectorCube<N, real>(n));

			const Vector<N, real> x(solveLinear(a, b));

			const real error =
				norm(x * a - b);

			if (error > 0.001)
			{
				++count;
			}
		}

		REPORT1(count > 10, count);
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

			Matrix<N, N, real> a(n, n);
			setRandomSymmetricPositiveDefinite(det, cond, a);

			const real detError = 
				mabs(determinant(a) - det);
			const real condError =
				mabs(conditionManhattan(a) - cond);

			REPORT1(detError > 0.01, detError);
			REPORT1(condError > 0.01, condError);

			if (detError > 0.01 ||
				condError > 0.01)
			{
				++count;
			}
		}

		REPORT2(count > 10, count, N);
	}

	void testBegin()
	{
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

		testMatrixView();
	}

	void testAdd()
	{
		mathTestList().add("Matrix", testBegin);
	}

	CallFunction run(testAdd);

}
