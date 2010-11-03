#ifndef PASTEL_DISCRETECOSINE_HPP
#define PASTEL_DISCRETECOSINE_HPP

#include "pastel/dsp/discretecosine.h"
#include "pastel/dsp/discretefourier.h"

#include <complex>

namespace Pastel
{

	namespace Dct_Detail
	{

		template <typename Real, bool ComplexOutput>
		class ExtractOutput
		{
		public:
			template <typename Type>
			const Type& operator()(const Type& that) const
			{
				return that;
			}
		};

		template <typename Real>
		class ExtractOutput<Real, false>
		{
		public:
			template <typename Type>
			Real operator()(const Type& that) const
			{
				return that.real();
			}
		};

		template <bool Orthogonal, bool ComplexOutput,
			typename Complex_ConstIterator, typename Complex_Iterator>
		void discreteCosine(
			const ForwardRange<Complex_ConstIterator>& input,
			Complex_Iterator output)
		{
			using namespace Fourier_Detail;

			typedef typename std::iterator_traits<Complex_ConstIterator>::value_type
				InputComplex;
			typedef typename Complex_RealType<InputComplex>::Result Real;
			typedef std::complex<Real> Complex;
			ExtractOutput<Real, ComplexOutput> extractOutput;

			if (input.empty())
			{
				return;
			}

			const integer n = input.size();
			ENSURE1(isPowerOfTwo(n), n);

			// We work conceptually as follows:
			//
			// 1) The size of 'input' is doubled so that 
			// input2[i] = input[i], for 0 <= i <  n, and
			// input2[i] = input[2n - 1 - i], for n <= i < 2n.
			//
			// 2) The size of 'input2' is doubled so that
			// input4[2i] = 0, for 0 <= i < 2n, and
			// input4[2i + 1] = input2[i], for 0 <= i < 2n.
			//
			// We shall compute the first half of the discrete
			// fourier transform of 'input4'.
					
			// Separate the input into odd-index and 
			// even-index subsequences.

			Complex_ConstIterator iter = input.begin();

			// The even-index subsequence of 'input4' is zero.

			std::vector<Complex> oddFourier;
			oddFourier.reserve(2 * n);
			for (integer i = 0;i < n;++i)
			{
				oddFourier.push_back(*iter);
				++iter;
			}
			for (integer i = n - 1;i >= 0;--i)
			{
				oddFourier.push_back(oddFourier[i]);
			}

			// The fourier transformation of the
			// even-index subsequence is zero.

			// Find out the fourier transformation
			// of the odd-index subsequence.
			dft(forwardRange(oddFourier.begin(), oddFourier.end()),
				oddFourier.begin());

			// Combine the results

			const Real NthRootAngle = 
				-2 * constantPi<Real>() / (4 * n);

			const Complex NthRoot(
				std::cos(NthRootAngle),
				std::sin(NthRootAngle));

			// The dft computes 2 * dct.

			// Orthogonal: 2 * (1 / sqrt(2 * n)) = sqrt(2) / sqrt(n)
			// None: 2 * (1 / 2) = 1

			const Real UnitScaling = 
				Orthogonal ? inverse(std::sqrt(2 * (Real)n)) : 
				inverse((Real)2);

			// Report the first half of the dft.
			{
				// Orthogonal: 2 * (1 / (2 * sqrt(n))) = 1 / sqrt(n)
				// Inverse: 2 * (1 / (2 * n)) = 1 / n
				// None: 2 * (1 / 2) = 1

				const Real FirstScaling =
					Orthogonal ? inverse(2 * std::sqrt((Real)n)) :
					inverse((Real)2);

				*output = extractOutput(oddFourier.front() * FirstScaling);
				++output;

				Complex oddFactor(NthRoot * UnitScaling);
						
				for (integer i = 1;i < n;++i)
				{
					*output = extractOutput(oddFactor * oddFourier[i]);
					++output;

					oddFactor *= NthRoot;
				}
			}
		}

		template <bool Orthogonal, bool ComplexOutput,
			typename Complex_ConstIterator, typename Complex_Iterator>
		void inverseDiscreteCosine(
			const ForwardRange<Complex_ConstIterator>& input,
			Complex_Iterator output)
		{
			using namespace Fourier_Detail;

			typedef typename std::iterator_traits<Complex_ConstIterator>::value_type
				InputComplex;
			typedef typename Complex_RealType<InputComplex>::Result Real;
			typedef std::complex<Real> Complex;
			ExtractOutput<Real, ComplexOutput> extractOutput;

			if (input.empty())
			{
				return;
			}

			const integer n = input.size();
			ENSURE1(isPowerOfTwo(n), n);

			Complex_ConstIterator iter = input.begin();

			if (n == 1)
			{
				*output = extractOutput(Complex(*iter));
				++output;
				return;
			}

			const Real NthRootAngle = 
				2 * constantPi<Real>() / (4 * n);

			const Complex NthRoot(
				std::cos(NthRootAngle),
				std::sin(NthRootAngle));

			// Orthogonal: (1 / n) * sqrt(2 * n) = sqrt(2 / n)
			// Inverse: (1 / n) * 2 = 2 / n

			const Real UnitScaling = 
				Orthogonal ? std::sqrt((Real)2 * n) : 2;

			std::vector<Complex> oddFourier;
			oddFourier.reserve(n);

			Complex oddFactor(UnitScaling);
			for (integer i = 0;i < n;++i)
			{
				oddFourier.push_back(oddFactor * *iter);
				++iter;

				oddFactor *= NthRoot;
			}

			// Orthogonal: sqrt(2 / n) / sqrt(2) = sqrt(1 / n)
			// Inverse: (2 / n) / 2 = 1 / n

			const Real FirstScaling =
				Orthogonal ? inverse(std::sqrt((Real)2)) :
				inverse((Real)2);

			oddFourier.front() *= FirstScaling;

			inverseDft(forwardRange(oddFourier.begin(), oddFourier.end()),
				oddFourier.begin());

			const integer nHalf = n / 2;
			for (integer i = 0;i < nHalf;++i)
			{
				*output = extractOutput(oddFourier[i]);
				++output;
				*output = extractOutput(oddFourier[n - 1 - i]);
				++output;
			}
		}

	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void dct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_Detail::discreteCosine<false, false>(input, output);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_Detail::inverseDiscreteCosine<false, false>(input, output);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_Detail::discreteCosine<true, false>(input, output);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_Detail::inverseDiscreteCosine<true, false>(input, output);
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void complexDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Dct_Detail::discreteCosine<false, true>(input, output);
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseComplexDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		// FIX: Does not work yet.
		const bool implementationMissing = true;
		ENSURE(!implementationMissing);

		Dct_Detail::inverseDiscreteCosine<false, true>(input, output);
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void unitaryDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Dct_Detail::discreteCosine<true, true>(input, output);
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseUnitaryDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		// FIX: Does not work yet.
		const bool implementationMissing = true;
		ENSURE(!implementationMissing);

		Dct_Detail::inverseDiscreteCosine<true, true>(input, output);
	}

}

#endif
