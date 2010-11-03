#ifndef PASTEL_DISCRETEFOURIER_HPP
#define PASTEL_DISCRETEFOURIER_HPP

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

	namespace Fourier_Detail
	{

		template <typename Type>
		class Complex_RealType
		{
		public:
			typedef Type Result;
		};

		template <typename Real>
		class Complex_RealType<std::complex<Real> >
		{
		public:
			typedef Real Result;
		};

		template <bool Inverse, bool TopLevel, bool Orthogonal, 
			typename Complex_ConstIterator, typename Complex_Iterator>
		void discreteFourierSmall(
			const ForwardRange<Complex_ConstIterator>& input,
			Complex_Iterator output)
		{
			typedef typename Complex_RealType<
				typename std::iterator_traits<Complex_ConstIterator>::value_type>::Result Real;
			typedef std::complex<Real> Complex;

			Complex_ConstIterator iter = input.begin();

			const integer n = input.size();
			ENSURE_OP(n, <=, 8);
			ENSURE1(isPowerOfTwo(n), n);

			const Real Normalization =
				n == 1 ? 1 :
				Orthogonal ? inverse(std::sqrt((Real)n)) :
				(Inverse ? inverse((Real)n) : 1);

			const bool Normalize = 
				(Inverse || Orthogonal) && TopLevel;

			switch(n)
			{
			case 1:
				{
					// Size 1 dft
					const Complex a0(*iter);
					++iter;

					*output = a0;
					++output;
				}
				break;
			case 2:
				{
					// Size 1 dfts
					const Complex a0(*iter);
					++iter;
					const Complex a1(*iter);
					++iter;

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
					const Complex a0(*iter);
					++iter;
					const Complex a1(*iter);
					++iter;
					const Complex a2(*iter);
					++iter;
					const Complex a3(*iter);
					++iter;

					// Size-2 dfts
					const Complex b0(a0 + a2);
					const Complex b1(a0 - a2);
					const Complex b2(a1 + a3);
					const Complex b3(a1 - a3);

					const Complex im(0, 
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
			typename Complex_ConstIterator, typename Complex_Iterator>
		void discreteFourier(
			const ForwardRange<Complex_ConstIterator>& input,
			Complex_Iterator output)
		{
			using namespace Fourier_Detail;

			typedef typename std::iterator_traits<Complex_ConstIterator>::value_type
				InputComplex;
			typedef typename Complex_RealType<InputComplex>::Result Real;
			typedef std::complex<Real> Complex;

			if (input.empty())
			{
				return;
			}

			const integer n = input.size();
			ENSURE1(isPowerOfTwo(n), n);

			if (n <= 4)
			{
				// As base-cases we handle 1, 2, and 4 size 
				// instances by hand.

				Fourier_Detail::discreteFourierSmall<Inverse, TopLevel, Orthogonal>(
					input, output);
				return;
			}

			const integer nHalf = n / 2;

			// Separate the input into odd-index and 
			// even-index subsequences.

			Complex_ConstIterator iter = input.begin();

			std::vector<Complex> evenFourier;
			evenFourier.reserve(nHalf);
			std::vector<Complex> oddFourier;
			oddFourier.reserve(nHalf);
			for (integer i = 0;i < nHalf;++i)
			{
				evenFourier.push_back(*iter);
				++iter;
				oddFourier.push_back(*iter);
				++iter;
			}

			// Find out the fourier transformation
			// of the even-index subsequence.
			discreteFourier<Inverse, false, Orthogonal>(
				forwardRange(evenFourier.begin(), evenFourier.end()),
				evenFourier.begin());

			// Find out the fourier transformation
			// of the odd-index subsequence.
			discreteFourier<Inverse, false, Orthogonal>(
				forwardRange(oddFourier.begin(), oddFourier.end()),
				oddFourier.begin());

			// Combine the results

			const Real NthRootAngle = 
				(Inverse ? 2 : -2) * constantPi<Real>() / n;

			const Complex NthRoot(
				std::cos(NthRootAngle),
				std::sin(NthRootAngle));

			const Real Normalization =
				Orthogonal ? inverse(std::sqrt((Real)n)) :
				(Inverse ? inverse((Real)n) : 1);

			const bool Normalize = 
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

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void dft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Fourier_Detail::discreteFourier<false, true, false>(
			input, output);
	}

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void unitaryDft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Fourier_Detail::discreteFourier<false, true, true>(
			input, output);
	}

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void inverseDft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Fourier_Detail::discreteFourier<true, true, false>(
			input, output);
	}

	template <typename Complex_ConstIterator, typename Complex_Iterator>
	void inverseUnitaryDft(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Fourier_Detail::discreteFourier<true, true, true>(
			input, output);
	}

}

#endif
