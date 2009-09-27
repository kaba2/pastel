#ifndef PASTEL_RANDOM_EXPONENTIAL_HPP
#define PASTEL_RANDOM_EXPONENTIAL_HPP

#include "pastel/sys/random_exponential.h"
#include "pastel/sys/random_uniform.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real randomExponential()
	{
		/*
		The probability density distribution of the
		exponential distribution is

		p(x) = a e^(-ax), for x >= 0
		       0        , for x <  0

		where a > 0.

		Its cumulative distribution is easy to derive:

		for x < 0:
		c(x) = 0

		for x >= 0:
		c(x) = int[t = 0..x] p(t) dt
		     = int[t = 0..x] a e^(-at) dt
			 = -[t = 0..x] e^(-at) dt
			 = -(e^(-ax) - 1)
			 = 1 - e^(-ax)
	    
		(For a check, c(oo) = 1)
		
		Thus, the inverse of the cdf (for x >= 0) is
		derived by:

		t = 1 - e^(-ax)
		<=>
		1 - t = e^(-ax)
		<=>
		ln(1 - t) = -ax
		<=>
		x = -ln(1 - t) / a

		So if we pick t a uniform random number in [0, 1[,
		then -ln(1 - t) / a is exponentially distributed.
		We simplify this further as follows. First, if t is 
		uniformly distributed in [0, 1[, then 1 - t is
		uniformly distributed in ]0, 1]. 
		Second, we can assume a = 1 without loss of
		generality and let the user do the scaling
		if needed. This improves performance.
		*/

		return -std::log(random1<Real>());
	}

	template <typename Real>
	Real randomExponential(
		const PASTEL_NO_DEDUCTION(Real)& mean)
	{
		PENSURE_OP(mean, >, 0);

		return Pastel::randomExponential<Real>() / mean;
	}

	template <typename Real, int N>
	Vector<Real, N> randomExponentialVector()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomExponentialVector<Real, N>(N);
	}

	template <typename Real, int N>
	Vector<Real, N> randomExponentialVector(
		integer dimension)
	{
		PENSURE_OP(dimension, >=, 0);

		Vector<Real, N> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = randomExponential<Real>();
		}
		
		return result;
	}

	template <typename Real>
	Real exponentialPdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		if (x < 0)
		{
			return 0;
		}

		return std::exp(-x);
	}

	template <typename Real>
	Real exponentialPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& mean)
	{
		PENSURE_OP(mean, >, 0);

		if (x < 0)
		{
			return 0;
		}

		return mean * std::exp(-mean * x);
	}

}

#endif
