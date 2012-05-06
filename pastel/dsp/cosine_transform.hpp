#ifndef PASTEL_COSINE_TRANSFORM_HPP
#define PASTEL_COSINE_TRANSFORM_HPP

#include "pastel/dsp/cosine_transform.h"
#include "pastel/dsp/fourier_transform.h"

#include <complex>

namespace Pastel
{

	namespace Dct_
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
		void discreteCosineSmall(
			const ForwardIterator_Range<Complex_ConstIterator>& input,
			Complex_Iterator output)
		{
			using namespace Fourier_;

			typedef typename std::iterator_traits<Complex_ConstIterator>::value_type
				InputComplex;
			typedef typename Complex_RealType<InputComplex>::Result Real;
			typedef std::complex<Real> Complex;
			ExtractOutput<Real, ComplexOutput> extractOutput;

			const integer n = input.size();

			ENSURE(isPowerOfTwo(n));
			ENSURE_OP(n, <=, 4);

			// The formulas here are the result of 
			// simplification by the following three
			// rules:
			//
			// 1) cos(alpha) = cos(2pi - alpha)
			// 2) cos(alpha) = -cos(pi - alpha)
			// 3) cos(alpha) = cos(-alpha)
			//
			// Using these, it is possible to bring
			// any alpha to the range 0 <= alpha < pi / 2,
			// after which the simplifications follow.

			Complex_ConstIterator iter = input.begin();

			if (n == 1)
			{
				*output = extractOutput(Complex(*iter));
				++output;
				return;
			}

			const Real UnitScaling = 
				Orthogonal ? std::sqrt(2 / (Real)n) : 1;
			const Real FirstScaling =
				Orthogonal ? inverse(std::sqrt((Real)n)) : 1;
			const Real NthRootAngle = 
				constantPi<Real>() / (2 * n);

			switch(n)
			{
			case 2:
				{
					// 1D dcts
					const Complex a0(*iter);
					++iter;
					const Complex a1(*iter);
					++iter;

					// 2D dct
					const Real w1 =
						std::cos(NthRootAngle);

					*output = extractOutput(
						(a0 + a1) * FirstScaling);
					++output;

					*output = extractOutput(
						((a0 - a1) * w1) * UnitScaling);
					++output;
				}
				break;
			case 4: 
				{
					// 1D dcts
					const Complex a0(*iter);
					++iter;
					const Complex a1(*iter);
					++iter;
					const Complex a2(*iter);
					++iter;
					const Complex a3(*iter);
					++iter;
					
					// 2D dcts
					const Complex b0(a0 - a3);
					const Complex b1(a1 - a2);
					const Complex b2(a0 + a3);
					const Complex b3(a1 + a2);
					
					// 4D dct
					const Real w1 =
						std::cos(NthRootAngle);

					const Real w2 =
						std::cos(2 * NthRootAngle);

					const Real w3 =
						std::cos(3 * NthRootAngle);

					*output = extractOutput(
						(b2 + b3) * FirstScaling);
					++output;

					*output = extractOutput(
						(b0 * w1 + b1 * w3) * UnitScaling);
					++output;

					*output = extractOutput(
						((b2 - b3) * w2) * UnitScaling);
					++output;

					*output = extractOutput(
						(b0 * w3 - b1 * w1) * UnitScaling);
					++output;
				}
				break;
			};
		}

		template <bool Orthogonal, bool ComplexOutput,
			typename Complex_ConstIterator, typename Complex_Iterator>
		void discreteCosine(
			const ForwardIterator_Range<Complex_ConstIterator>& input,
			Complex_Iterator output)
		{
			using namespace Fourier_;

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

			if (n <= 4)
			{
				discreteCosineSmall<Orthogonal, ComplexOutput>(input, output);
				return;
			}

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
			// Fourier transform of 'input4'.
					
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

			// The Fourier transformation of the
			// even-index subsequence is zero.

			// Find out the Fourier transformation
			// of the odd-index subsequence.
			dft(range(oddFourier.begin(), oddFourier.end()),
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
			const ForwardIterator_Range<Complex_ConstIterator>& input,
			Complex_Iterator output)
		{
			using namespace Fourier_;

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

			inverseDft(range(oddFourier.begin(), oddFourier.end()),
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
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_::discreteCosine<false, false>(input, output);
	}

	template <typename Real_Iterator>
	void dct(const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::dct(inputOutput, inputOutput.begin());
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseDct(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_::inverseDiscreteCosine<false, false>(input, output);
	}

	template <typename Real_Iterator>
	void inverseDct(const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::inverseDct(inputOutput, inputOutput.begin());
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalDct(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_::discreteCosine<true, false>(input, output);
	}

	template <typename Real_Iterator>
	void orthogonalDct(const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::orthogonalDct(inputOutput, inputOutput.begin());
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalDct(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Dct_::inverseDiscreteCosine<true, false>(input, output);
	}

	template <typename Real_Iterator>
	void inverseOrthogonalDct(const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::inverseOrthogonalDct(inputOutput, inputOutput.begin());
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void complexDct(
		const ForwardIterator_Range<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Dct_::discreteCosine<false, true>(input, output);
	}

	template <typename Real_Iterator>
	void complexDct(const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::complexDct(inputOutput, inputOutput.begin());
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseComplexDct(
		const ForwardIterator_Range<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		// FIX: Does not work yet.
		const bool implementationMissing = true;
		ENSURE(!implementationMissing);

		Dct_::inverseDiscreteCosine<false, true>(input, output);
	}

	template <typename Real_Iterator>
	void inverseComplexDct(const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::inverseComplexDct(inputOutput, inputOutput.begin());
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void unitaryDct(
		const ForwardIterator_Range<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		Dct_::discreteCosine<true, true>(input, output);
	}

	template <typename Real_Iterator>
	void unitaryDct(const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::unitaryDct(inputOutput, inputOutput.begin());
	}

	template <
		typename Complex_ConstIterator, 
		typename Complex_Iterator>
	void inverseUnitaryDct(
		const ForwardIterator_Range<Complex_ConstIterator>& input,
		Complex_Iterator output)
	{
		// FIX: Does not work yet.
		const bool implementationMissing = true;
		ENSURE(!implementationMissing);

		Dct_::inverseDiscreteCosine<true, true>(input, output);
	}

	template <typename Real_Iterator>
	void inverseUnitaryDct(const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::inverseUnitaryDct(inputOutput, inputOutput.begin());
	}

}

#endif
