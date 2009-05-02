#ifndef PASTELDSP_DISCRETEFOURIER_HPP
#define PASTELDSP_DISCRETEFOURIER_HPP

#include "pastel/dsp/discretefourier.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/array.h"
#include "pastel/sys/sparseview.h"
#include "pastel/sys/view_tools.h"

#include <complex>

namespace Pastel
{

	/*
	The fast fourier transformation is based on the following
	recursion property:
	Let
	n := 2^k (k for some integer).
	a := a finite sequence of complex numbers (a(1), ..., a(n)).
	w := e^(i * (2 * pi / n)) (the n:th root of unity).

	The discrete fourier transformation is defined by:
	F{a}(k) := sum{k}(a(k) * w^k)

	The sum can be decomposed into even and odd indices:
	Feven{a}(k) := sum{k}(a(2*k) * w^(2*k))
	Fodd{a}(k) := sum{k}(a(2*k + 1) * w^(2*k + 1))
	= w * sum{k}(a(2*k + 1) * w^(2*k))

	Now the discrete fourier transformation can be given
	recursively as:
	FIX: Complete this.
	*/

	template <typename Real, typename Input_View,
		typename Output_View>
		void discreteFourier(
		const ConstView<1, std::complex<Real>, Input_View>& input,
		const View<1, std::complex<Real>, Output_View>& output)
	{
		const integer n = input.width();

		ENSURE2(n == output.width(), n, output.width());

		if (n == 0)
		{
			return;
		}

		ENSURE1(isPowerOfTwo(n), n);

		if (n == 1)
		{
			// End of recursion
			output(0) = input(0);
			return;
		}

		const integer nHalf = n / 2;

		// Find out the fourier transformation
		// of the even-index subsequence.

		Array<1, std::complex<Real> > evenFourier(nHalf);
		for (integer i = 0;i < nHalf;++i)
		{
			evenFourier(i) = input(i * 2);
		}
		/*
		copy(constSparseView(input, 0, 2), 
			arrayView(evenFourier));
		*/
		discreteFourier(constArrayView(evenFourier), 
			arrayView(evenFourier));

		// Find out the fourier transformation
		// of the odd-index subsequence.

		Array<1, std::complex<Real> > oddFourier(nHalf);
		for (integer i = 0;i < nHalf;++i)
		{
			oddFourier(i) = input(i * 2 + 1);
		}
		/*
		copy(constSparseView(input, 1, 2), 
			arrayView(oddFourier));
		*/
		discreteFourier(constArrayView(oddFourier), 
			arrayView(oddFourier));

		// Combine the results

		static const Real NthRootAngle = 
			2 * constantPi<Real>() / n;

		const std::complex<Real> NthRoot =
			std::polar((Real)1, NthRootAngle);

		std::complex<Real> oddFactor(1);

		for (integer i = 0;i < nHalf;++i)
		{
			output(i) = evenFourier(i) +
				oddFactor * oddFourier(i);
			output(i + nHalf) = evenFourier(i) -
				oddFactor * oddFourier(i);

			oddFactor *= NthRoot;
		}
	}

	template <typename Real, typename Input_View,
		typename Output_View>
		void discreteFourierInverse(
		const ConstView<1, std::complex<Real>, Input_View>& input,
		const View<1, std::complex<Real>, Output_View>& output)
	{
		const integer n = input.width();

		ENSURE2(n == output.width(), n, output.width());

		if (n == 0)
		{
			return;
		}

		ENSURE1(isPowerOfTwo(n), n);

		if (n == 1)
		{
			// End of recursion
			output(0) = input(0);
			return;
		}

		const integer nHalf = n / 2;

		// Find out the inverse fourier transformation
		// of the even-index subsequence.

		Array<1, std::complex<Real> > evenFourier(nHalf);
		copy(constSparseView(input, 0, 2), 
			arrayView(evenFourier));
		discreteFourierInverse(
			constArrayView(evenFourier), 
			arrayView(evenFourier));

		// Find out the inverse fourier transformation
		// of the odd-index subsequence.

		Array<1, std::complex<Real> > oddFourier(nHalf);
		copy(constSparseView(input, 1, 2), 
			arrayView(oddFourier));
		discreteFourierInverse(
			constArrayView(oddFourier), 
			arrayView(oddFourier));

		// Combine the results

		static const Real NthRootAngle = 
			-2 * constantPi<Real>() / n;

		for (integer i = 0;i < nHalf;++i)
		{
			const std::complex<Real> oddFactor(
				polar((Real)1, (Real)i * NthRootAngle));

			output(i) = (evenFourier(i) +
				oddFactor * oddFourier(i)) / 2;
			output(i + nHalf) = (evenFourier(i) -
				oddFactor * oddFourier(i)) / 2;
		}
	}

	namespace Detail_DiscreteFourier
	{

		class FourierFunctor
		{
		public:
			template <typename Left_View, typename Right_View>
			void operator()(
				const Left_View& left,
				const Right_View& right) const
			{
				discreteFourier(left, right);
			}
		};

	}

	template <
		int N,
		typename Real,
		typename Input_View,
		typename Output_View>
		typename boost::enable_if_c<(N > 1), void>::type
		discreteFourier(
		const ConstView<N, std::complex<Real>, Input_View>& input,
		const View<N, std::complex<Real>, Output_View>& output)
	{
		BOOST_STATIC_ASSERT(N > 1);

		Detail_DiscreteFourier::FourierFunctor fourierFunctor;

		Array<N, std::complex<Real> > temp(input.extent());

		visitRows(input, arrayView(temp), 0, fourierFunctor);
		
		for (integer i = 1;i < N - 1;++i)
		{
			visitRows(constArrayView(temp), 
				arrayView(temp), i, fourierFunctor);
		}

		visitRows(constArrayView(temp), output, N - 1, fourierFunctor);
	}

	namespace Detail_DiscreteFourierInverse
	{

		class FourierInverseFunctor
		{
		public:
			template <typename Left_View, typename Right_View>
			void operator()(
				const Left_View& left,
				const Right_View& right) const
			{
				discreteFourierInverse(left, right);
			}
		};

	}

	template <
		int N,
		typename Real,
		typename Input_View,
		typename Output_View>
		typename boost::enable_if_c<(N > 1), void>::type
		discreteFourierInverse(
		const ConstView<N, std::complex<Real>, Input_View>& input,
		const View<N, std::complex<Real>, Output_View>& output)
	{
		BOOST_STATIC_ASSERT(N > 1);

		Detail_DiscreteFourierInverse::FourierInverseFunctor 
			fourierInverseFunctor;

		for (integer i = 0;i < N;++i)
		{
			visitRows(input, output, i, fourierInverseFunctor);
		}
	}

}

#endif
