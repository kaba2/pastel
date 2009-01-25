#ifndef PASTELGFX_DISCRETEFOURIER_HPP
#define PASTELGFX_DISCRETEFOURIER_HPP

#include "pastel/gfx/discretefourier.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/lineararray.h"
#include "pastel/math/complex_tools.h"

#include <vector>

namespace Pastel
{

	/*
	The fast fourier transformation is based on the following
	recursion property:
	@par
	Let
	@par
	n := 2^k (k for some integer).
	@par
	a := a finite sequence of complex numbers (a(1), ..., a(n)).
	@par
	w := e^(i * (2 * pi / n)) (the n:th root of unity).
	@par
	The discrete fourier transformation is defined by:
	@par
	F{a}(k) := sum{k}(a(k) * w^k)
	@par
	The sum can be decomposed into even and odd indices:
	@par
	Feven{a}(k) := sum{k}(a(2*k) * w^(2*k))
	@par
	Fodd{a}(k) := sum{k}(a(2*k + 1) * w^(2*k + 1))
	= w * sum{k}(a(2*k + 1) * w^(2*k))
	@par
	Now the discrete fourier transformation can be given
	recursively as:
	@par
	FIX: Complete this.
	*/

	template <
		typename InputView,
		typename OutputView>
	void discreteFourier(
		const InputView& input,
		OutputView& output)
	{
		typedef typename InputView::Element
			InputType;
		typedef typename OutputView::Element
			OutputType;

		typedef InputType Complex;
		typedef Complex::ValueType Real;

		const integer n = input.size();

		ENSURE1(n >= 1, n);
		ENSURE2(n == output.size(), n, output.size());
		ENSURE1(isPowerOfTwo(n), n);

		if (n == 1)
		{
			// End of recursion
			output[0] = input[0];
			return;
		}

		const integer nHalf = n / 2;

		// Find out the fourier transformation
		// of the even-index subsequence.

		std::vector<Complex> evenFourier(nHalf);

		for (integer i = 0;i < nHalf;++i)
		{
			evenFourier[i] = input[2 * i];
		}

		MemoryView<Complex> evenView(
			&evenFourier.front(), nHalf);

		discreteFourier(evenView, evenView);

		// Find out the fourier transformation
		// of the odd-index subsequence.

		std::vector<Complex> oddFourier(nHalf);

		for (integer i = 0;i < nHalf;++i)
		{
			oddFourier[i] = input[2 * i + 1];
		}

		MemoryView<Complex> oddView(
		&oddFourier.front(), nHalf);

		discreteFourier(oddView, oddView);

		// Combine the results

		static const Real NthRootAngle = -2 *
			constantPi<Real>() / n;

		for (integer i = 0;i < nHalf;++i)
		{
			Complex oddFactor(
				polar((Real)1, (Real)i * NthRootAngle));

			output[i] = evenFourier[i] +
				oddFactor * oddFourier[i];
		}

		for (integer i = nHalf;i < n;++i)
		{
			Complex oddFactor(
				polar((Real)1, (Real)i * NthRootAngle));

			output[i] = evenFourier[i - nHalf] +
				oddFactor * oddFourier[i - nHalf];
		}
	}

	template <
		typename InputView,
		typename OutputView>
	void discreteFourierInverse(
		const InputView& input,
		OutputView& output)
	{
		typedef typename InputView::Element
			InputType;
		typedef typename OutputView::Element
			OutputType;

		typedef InputType Complex;
		typedef Complex::ValueType Real;

		const integer n = input.size();

		ENSURE1(n >= 1, n);
		ENSURE2(n == output.size(), n, output.size());
		ENSURE1(isPowerOfTwo(n), n);

		if (n == 1)
		{
			// End of recursion
			output[0] = input[0];
			return;
		}

		const integer nHalf = n / 2;

		// Find out the inverse fourier transformation
		// of the even-index subsequence.

		std::vector<Complex> evenFourier(nHalf);

		for (integer i = 0;i < nHalf;++i)
		{
			evenFourier[i] = input[2 * i];
		}

		MemoryView<Complex> evenView(
			&evenFourier.front(), nHalf);

		discreteFourier(evenView, evenView);

		// Find out the inverse fourier transformation
		// of the odd-index subsequence.

		std::vector<Complex> oddFourier(nHalf);

		for (integer i = 0;i < nHalf;++i)
		{
			oddFourier[i] = input[2 * i + 1];
		}

		MemoryView<Complex> oddView(
			&oddFourier.front(), nHalf);

		discreteFourierInverse(oddView, oddView);

		// Combine the results

		static const Real NthRootAngle = 2 *
			constantPi<Real>() / n;

		for (integer i = 0;i < nHalf;++i)
		{
			Complex oddFactor(
				polar((Real)1, (Real)i * NthRootAngle));

			output[i] = evenFourier[i] +
				oddFactor * oddFourier[i];
			output[i] *= (Real)0.5;
		}

		for (integer i = nHalf;i < n;++i)
		{
			const Complex oddFactor(
				polar((Real)1, (Real)i * NthRootAngle));

			output[i] = evenFourier[i - nHalf] +
				oddFactor * oddFourier[i - nHalf];
			output[i] *= (Real)0.5;
		}
	}

	template <
		typename InputView,
		typename OutputView>
		void discreteFourier2(
		const InputView& input,
		OutputView& output)
	{
		discreteFourierHorizontal(input, output);
		TransposeView<OutputView> transposeOutput(output);
		discreteFourierHorizontal(transposeOutput,
			transposeOutput);
	}

	template <
		typename InputView,
		typename OutputView>
	void discreteFourier2Horizontal(
		const InputView& input,
		OutputView& output)
	{
		const integer width = input.width();
		const integer height = input.height();

		ENSURE1(width >= 1, width);
		ENSURE2(width == output.width(),
			width, output.width());
		ENSURE1(isPowerOfTwo(width), width);

		ENSURE1(height >= 1, height);
		ENSURE2(height == output.height(),
			height, output.height());
		ENSURE1(isPowerOfTwo(height), height);

		// Transformation rows

		for (integer y = 0;y < height;++y)
		{
			RowView<InputView> inputRowView(
				input, y);
			RowView<OutputView> outputRowView(
				output, y);

			discreteFourier(inputRowView, outputRowView);
		}
	}

	template <
		typename InputView,
		typename OutputView>
		void discreteFourierInverse2(
		const InputView& input,
		OutputView& output)
	{
		discreteFourierInverse2Horizontal(input, output);
		TransposeView<OutputView> transposeOutput(output);
		discreteFourierInverse2Horizontal(transposeOutput,
			transposeOutput);
	}

	template <
		typename InputView,
		typename OutputView>
	void discreteFourierInverse2Horizontal(
		const InputView& input,
		OutputView& output)
	{
		const integer width = input.width();
		const integer height = input.height();

		ENSURE1(width >= 1, width);
		ENSURE2(width == output.width(),
			width, output.width());
		ENSURE1(isPowerOfTwo(width), width);

		ENSURE1(height >= 1, height);
		ENSURE2(height == output.height(),
			height, output.height());
		ENSURE1(isPowerOfTwo(height), height);

		// Transformation rows

		for (integer y = 0;y < height;++y)
		{
			RowView<InputView> inputRowView(
				input, y);
			RowView<OutputView> outputRowView(
				output, y);
			discreteFourierInverse(
				inputRowView, outputRowView);
		}
	}

}

#endif
