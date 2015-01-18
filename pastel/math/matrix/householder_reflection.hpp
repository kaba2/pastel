#ifndef PASTELMATH_HOUSEHOLDER_REFLECTION_HPP
#define PASTELMATH_HOUSEHOLDER_REFLECTION_HPP

#include "pastel/math/matrix/householder_reflection.h"

namespace Pastel
{

	template <typename Real>
	void householder(
		Matrix<Real>& a, const Vector<Real>& x, 
		integer i, integer j)
	{
		ENSURE_OP(i, >=, 0);
		ENSURE_OP(i, <, a.m());
		ENSURE_OP(j, >=, 0);
		ENSURE_OP(j, <, a.n());
		ENSURE_OP(x.n(), ==, a.m());

		integer m = a.m();
		integer n = a.n();

		// Let
		// 
		//    u = x + sgn(x_i) |x| e_i.
		// 
		// Then
		//
		//    |u|^2 = |x|^2 + 2sgn(x_i) |x| x_i + |x|^2
		//          = 2|x|^2 + 2|x_i| |x|
		//          = 2(|x|^2 + |x_i| |x|)
		//          = 2c.
		// 
		// and
		//
		//    I - 2vv^T = I - 2uu^T / |u|^2
		//              = I - uu^T / c.
		
		// Compute |x|^2.
		Real xNorm2 = 0;
		// We act as if x(0 : i) = 0.
		for (integer k = i;k < m;++k)
		{
			xNorm2 += square(x[k]);
		}

		// Compute |x|.
		Real xNorm = std::sqrt(xNorm2);

		Real c = xNorm2 + std::abs(x[i]) * xNorm;
		Real s = sign(x[i]) * xNorm;
		
		// Compute new A(i : m, j : n).
		for (integer p = j;p < n;++p)
		{
			// Compute d = dot(u(i : m), A(i : m, p)) / c = u^T y / c.
			Real d = (x[i] + s) * a(i, p);
			for (integer k = i + 1;k < m;++k)
			{
				d += x[k] * a(k, p);
			}
			d /= c;

			// (I - uu^T / c)y = y - u (u^T y / c)
			//                 = y - ud.
			a(i, p) -= (x[i] + s) * d;
			for (integer k = i + 1;k < m;++k)
			{
				a(k, p) -= x[k] * d;
			}
		}
	}

}

#endif
