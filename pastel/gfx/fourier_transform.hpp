#ifndef PASTELGFX_FOURIER_TRANSFORM_HPP
#define PASTELGFX_FOURIER_TRANSFORM_HPP

#include "pastel/gfx/fourier_transform.h"

#include "pastel/sys/math/constants.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view/sparseview.h"
#include "pastel/sys/view/view_tools.h"

#include <complex>

namespace Pastel
{

	namespace Fourier_
	{

		template <typename Type>
		class Complex_RealType
		{
		public:
			using Result = Type;
		};

		template <typename Real>
		class Complex_RealType<std::complex<Real> >
		{
		public:
			using Result = Real;
		};

		template <bool Inverse, bool TopLevel, bool Orthogonal, 
			typename Complex_ConstRange, typename Complex_Range>
		void discreteFourierSmall(
			const Complex_ConstRange& inputRange,
			const Complex_Range& outputRange)
		{
			typedef typename Complex_RealType<
				typename boost::range_value<Complex_ConstRange>::type>::Result Real;
			using Complex = std::complex<Real>;
			typedef typename boost::range_iterator<Complex_ConstRange>::type
				ConstIterator;

			auto input = inputRange.begin();
			auto output = outputRange.begin();

			integer n = inputRange.size();
			ENSURE_OP(n, <=, 8);
			ENSURE1(isPowerOfTwo(n), n);

			Real Normalization =
				n == 1 ? 1 :
				Orthogonal ? inverse(std::sqrt((Real)n)) :
				(Inverse ? inverse((Real)n) : 1);

			bool Normalize =
				(Inverse || Orthogonal) && TopLevel;

			switch(n)
			{
			case 1:
				{
					// Size 1 dft
					const Complex a0(*input);
					++input;

					*output = a0;
					++output;
				}
				break;
			case 2:
				{
					// Size 1 dfts
					const Complex a0(*input);
					++input;
					const Complex a1(*input);
					++input;

					// Size 2 dft
					if (Normalize)
					{
						*output = (a0 + a1) * Normalization;
						++output;

						*output = (a0 - a1) * Normalization;
						++output;
					}
					else
					{
						*output = a0 + a1;
						++output;

						*output = a0 - a1;
						++output;
					}
				}
				break;
			case 4:
				{
					// Size 1 dfts
					const Complex a0(*input);
					++input;
					const Complex a1(*input);
					++input;
					const Complex a2(*input);
					++input;
					const Complex a3(*input);
					++input;

					// Size-2 dfts
					Complex b0(a0 + a2);
					Complex b1(a0 - a2);
					Complex b2(a1 + a3);
					Complex b3(a1 - a3);

					Complex im(0, 
						Inverse ? -1 : 1);
					
					// Size-4 dft
					if (Normalize)
					{


						*output = (b0 + b2) * Normalization;
						++output;

						*output = (b1 - im * b3) * Normalization;
						++output;

						*output = (b0 - b2) * Normalization;
						++output;

						*output = (b1 + im * b3) * Normalization;
						++output;
					}
					else
					{
						*output = b0 + b2;
						++output;

						*output = b1 - im * b3;
						++output;

						*output = b0 - b2;
						++output;

						*output = b1 + im * b3;
						++output;
					}
				}
				break;
			};
		}

		template <bool Inverse, bool TopLevel, bool Orthogonal,
			typename Complex_ConstRange, typename Complex_Range>
		void discreteFourier(
			const Complex_ConstRange& inputRange,
			const Complex_Range& outputRange)
		{
			using namespace Fourier_;

			typedef typename boost::range_value<Complex_ConstRange>::type
				InputComplex;
			using Real = typename Complex_RealType<InputComplex>::Result;
			using Complex = std::complex<Real>;

			if (inputRange.empty())
			{
				return;
			}

			integer n = inputRange.size();
			ENSURE1(isPowerOfTwo(n), n);

			if (n <= 4)
			{
				// As base-cases we handle 1, 2, and 4 size 
				// instances by hand.

				Fourier_::discreteFourierSmall<Inverse, TopLevel, Orthogonal>(
					inputRange, outputRange);
				return;
			}

			integer nHalf = n / 2;

			// Separate the inputRange into odd-index and 
			// even-index subsequences.

			auto input = inputRange.begin();
			auto output = outputRange.begin();

			std::vector<Complex> evenFourier;
			evenFourier.reserve(nHalf);
			std::vector<Complex> oddFourier;
			oddFourier.reserve(nHalf);
			for (integer i = 0;i < nHalf;++i)
			{

				evenFourier.push_back(*input);
				++input;
				oddFourier.push_back(*input);
				++input;
			}

			// Find out the Fourier transformation
			// of the even-index subsequence.
			discreteFourier<Inverse, false, Orthogonal>(
				range(evenFourier.begin(), evenFourier.end()),
				range(evenFourier.begin(), evenFourier.end()));

			// Find out the Fourier transformation
			// of the odd-index subsequence.
			discreteFourier<Inverse, false, Orthogonal>(
				range(oddFourier.begin(), oddFourier.end()),
				range(oddFourier.begin(), oddFourier.end()));

			// Combine the results

			Real NthRootAngle =
				(Inverse ? 2 : -2) * constantPi<Real>() / n;

			Complex NthRoot(
				std::cos(NthRootAngle),
				std::sin(NthRootAngle));

			Real Normalization =
				Orthogonal ? inverse(std::sqrt((Real)n)) :
				(Inverse ? inverse((Real)n) : 1);

			bool Normalize =
				(Inverse || Orthogonal) && TopLevel;

			// Report the first half of the dft.
			{
				Complex oddFactor(1);
						
				for (integer i = 0;i < nHalf;++i)
				{
					if (Normalize)
					{
						*output = (evenFourier[i] +
							oddFactor * oddFourier[i]) * Normalization;
					}
					else
					{
						*output = evenFourier[i] +
							oddFactor * oddFourier[i];
					}
					++output;

					oddFactor *= NthRoot;
				}
			}

			// Report the second half of the dft.
			{
				Complex oddFactor(1);

				for (integer i = 0;i < nHalf;++i)
				{
					if (Normalize)
					{
						*output = (evenFourier[i] -
							oddFactor * oddFourier[i]) * Normalization;
					}
					else
					{
						*output = evenFourier[i] -
							oddFactor * oddFourier[i];
					}
					++output;

					oddFactor *= NthRoot;
				}
			}
		}

	}

	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void dft(
		const Complex_ConstRange& input,
		const Complex_Range& output)
	{
		Fourier_::discreteFourier<false, true, false>(
			input, output);
	}

	template <typename Complex_Range>
	void dft(
		const Complex_Range& inputOutput)
	{
		Pastel::dft(inputOutput, inputOutput);
	}

	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void unitaryDft(
		const Complex_ConstRange& input,
		const Complex_Range& output)
	{
		Fourier_::discreteFourier<false, true, true>(
			input, output);
	}

	template <typename Complex_Range>
	void unitaryDft(
		const Complex_Range& inputOutput)
	{
		Pastel::unitaryDft(inputOutput, inputOutput);
	}

	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void inverseDft(
		const Complex_ConstRange& input,
		const Complex_Range& output)
	{
		Fourier_::discreteFourier<true, true, false>(
			input, output);
	}

	template <typename Complex_Range>
	void inverseDft(
		const Complex_Range& inputOutput)
	{
		Pastel::inverseDft(inputOutput, inputOutput);
	}

	template <
		typename Complex_ConstRange, 
		typename Complex_Range>
	void inverseUnitaryDft(
		const Complex_ConstRange& input,
		const Complex_Range& output)
	{
		Fourier_::discreteFourier<true, true, true>(
			input, output);
	}

	template <typename Complex_Range>
	void inverseUnitaryDft(
		const Complex_Range& inputOutput)
	{
		Pastel::inverseUnitaryDft(inputOutput, inputOutput);
	}

}

#endif
