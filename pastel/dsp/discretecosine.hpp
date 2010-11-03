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

		template <bool Inverse, bool Orthogonal, bool ComplexOutput,
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

			// FIX: The inverse-dct does not yet work!
			ENSURE(!Inverse);

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
			if (Inverse)
			{
				inverseDft(forwardRange(oddFourier.begin(), oddFourier.end()),
					oddFourier.begin());
			}
			else
			{
				dft(forwardRange(oddFourier.begin(), oddFourier.end()),
					oddFourier.begin());
			}

			// Combine the results

			const Real NthRootAngle = 
				(Inverse ? 2 : -2) * constantPi<Real>() / (4 * n);

			const Complex NthRoot(
				std::cos(NthRootAngle),
				std::sin(NthRootAngle));

			// The dft computes 2 * dct.

			// Orthogonal: 2 * (1 / sqrt(2 * n)) = sqrt(2) / sqrt(n)
			// Inverse: 2 * (1 / n) = 2 / n
			// None: 2 * (1 / 2) = 1

			const Real UnitScaling = 
				Orthogonal ? inverse(std::sqrt(2 * (Real)n)) : 
				(Inverse ? inverse((Real)n) : inverse((Real)2));

			// Report the first half of the dft.
			{
				// Orthogonal: 2 * (1 / (2 * sqrt(n))) = 1 / sqrt(n)
				// Inverse: 2 * (1 / (2 * n)) = 1 / n
				// None: 2 * (1 / 2) = 1

				const Real FirstScaling =
					Orthogonal ? inverse(2 * std::sqrt((Real)n)) :
					(Inverse ? inverse(2 * (Real)n) : inverse((Real)2));

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

	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void dct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_Detail::discreteCosine<false, false, false>(input, output);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_Detail::discreteCosine<true, false, false>(input, output);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_Detail::discreteCosine<false, true, false>(input, output);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalDct(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_Detail::discreteCosine<true, true, false>(input, output);
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void complexDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Dct_Detail::discreteCosine<false, false, true>(input, output);
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseComplexDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Dct_Detail::discreteCosine<true, false, true>(input, output);
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void unitaryDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Dct_Detail::discreteCosine<false, true, true>(input, output);
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseUnitaryDct(
		const ForwardRange<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Dct_Detail::discreteCosine<true, true, true>(input, output);
	}

}

#endif
