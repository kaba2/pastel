#include "pastelmathtest.h"
#include "pastel/math/matrix_tools.h"
#include "pastel/math/uniformsampling.h"
#include "pastel/math/ludecomposition_tools.h"

using namespace Pastel;

namespace
{

	template <int N>
	void testLu()
	{
		integer bad = 0;

		for (integer i = 0;i < 10000;++i)
		{
			Matrix<N, N, real> m;
			setRandomMatrix(m);

			LuDecomposition<N, real> lu(m);

			if (!lu.singular())
			{
				Vector<N, real> b = randomVectorCube<N, real>();

				Vector<N, real> x;
				solveLinearSystem(lu, b, x);

				const Vector<N, real> residual = m * x - b;
				const real normResidual = norm(residual);

				if (normResidual > 0.0001)
				{
					++bad;
				}
			}
		}

		REPORT1(bad >= 100, bad);
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
		log() << "Inverting 10000 " << N << " x " << N << " matrices." << logNewLine;

		const integer matrices = 10000;

		integer count = 0;

		for (integer i = 0;i < matrices;++i)
		{
			Matrix<N, N, real> m;
			setRandomMatrix(m);
			const Matrix<N, N, real> mInv(inverse(m));

			Matrix<N, N, real> result = m * mInv - Matrix<N, N, real>();
			modify(result, (real (*)(real))mabs);
			if (max(result) < 0.0001)
			{
				++count;
			}
		}

		REPORT1(count < 9950, count);
	}

	template <int N>
	void testMatrixSolve()
	{
		const integer iterations = 10000;

		integer count = 0;

		for (integer i = 0;i < iterations;++i)
		{
			Matrix<N, N, real> a;
			setRandomMatrix(a);

			const Vector<N, real> b(randomVectorCube<N, real>());

			const Vector<N, real> x(solveLinearSystem(a, b));

			const Vector<N, real> result(
				mabs(x * a - b));

			bool ok = true;
			for (integer j = 0;j < N;++j)
			{
				if (result[j] >= 0.0001)
				{
					ok = false;
					break;
				}
			}
			if (ok)
			{
				++count;
			}
		}

		REPORT1(count < 9950, count);
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
		testMatrixInverse<1>();
		testMatrixInverse<2>();
		testMatrixInverse<3>();
		testMatrixInverse<4>();
		testMatrixInverse<5>();
		testLu<1>();
		testLu<2>();
		testLu<3>();
		testLu<4>();
		testLu<5>();
	}

	void testAdd()
	{
		mathTestList().add("Matrix", testBegin);
	}

	CallFunction run(testAdd);

}
