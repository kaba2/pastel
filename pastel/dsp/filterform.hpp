#ifndef PASTELDSP_FILTERFORM_HPP
#define PASTELDSP_FILTERFORM_HPP

#include "pastel/dsp/filterform.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <
		typename Real,
		typename FilterWindow,
		typename Array>
	void setSincLowPass(
		const Real& cutOff,
		FilterWindow filterWindow,
		Array& output)
	{
		const integer width = output.size();

		ENSURE_OP(width, >, 0);
		ENSURE1(odd(width), width);

		const integer half = (width - 1) / 2;
		const Real scaleSincFreq = 2 * cutOff;
		const Real scaleWindowFreq = (Real)1 / half;
		//const Real scaleSinc = 2 * cutOff;

		Real sum(0);
		for (integer i = 0;i < width;++i)
		{
			const Real distance(mabs(Real(i - half)));
			output[i] = sinc(distance * scaleSincFreq) *
				filterWindow(distance * scaleWindowFreq);
			sum += output[i];
		}

		for (integer i = 0;i < width;++i)
		{
			output[i] /= sum;
		}
	}

	template <
		typename Real,
		typename FilterWindow,
		typename Array>
	void setSincHighPass(
		const Real& cutOff,
		FilterWindow filterWindow,
		Array& output)
	{
		const integer width = output.size();

		ENSURE_OP(width, >, 0);
		ENSURE1(odd(width), width);

		const integer half = (width - 1) / 2;
		const Real scaleSincFreq = 2 * cutOff;
		const Real scaleWindowFreq = (Real)1 / half;
		//const Real scaleSinc = 2 * cutOff;

		Real sum(0);
		for (integer i = 0;i < width;++i)
		{
			const Real distance(mabs(Real(i - half)));
			output[i] = -sinc(distance * scaleSincFreq) *
				filterWindow(distance * scaleWindowFreq);

			sum += output[i];
		}

		for (integer i = 0;i < width;++i)
		{
			output[i] /= sum;
		}
	}

	template <
		typename Real,
		typename FilterWindow,
		typename Output_View>
	void setSincLowPass(
		const PASTEL_NO_DEDUCTION(Real)& cutOff,
		FilterWindow filterWindow,
		const View<2, Real, Output_View>& output)
	{
		const integer width = output.width();
		const integer height = output.height();

		if (width == 0 ||
			height == 0)
		{
			return;
		}

		ENSURE1(odd(width), width);
		ENSURE1(odd(height), height);
		ENSURE_OP(width, ==, height);

		const Vector<Real, 2> half(
			(width - 1) / 2, (height - 1) / 2);
		const Real scaleSincFreq = 2 * cutOff;
		const Real scaleWindowFreq = (Real)1 / half[0];

		Real sum(0);
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				const Vector<Real, 2> delta(
					Vector<Real, 2>(x, y) - half);
				const Real distance(norm(delta));
				*output.cursor(x, y) = sinc(distance * scaleSincFreq) *
					filterWindow(distance * scaleWindowFreq);
				sum += *output.cursor(x, y);
			}
		}

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				*output.cursor(x, y) /= sum;
			}
		}
	}

	template <
		typename Real,
		typename FilterWindow,
		typename Output_View>
	void setSincHighPass(
		const PASTEL_NO_DEDUCTION(Real)& cutOff,
		FilterWindow filterWindow,
		const View<2, Real, Output_View>& output)
	{
		const integer width = output.width();
		const integer height = output.height();

		if (width == 0 ||
			height == 0)
		{
			return;
		}

		ENSURE1(odd(width), width);
		ENSURE1(odd(height), height);
		ENSURE_OP(width, ==, height);

		const Vector<Real, 2> half(
			(width - 1) / 2, (height - 1) / 2);
		const Real scaleSincFreq = 2 * cutOff;
		const Real scaleWindowFreq = (Real)1 / half[0];

		Real sum(0);
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				const Vector<Real, 2> delta(
					Vector<Real, 2>(x, y) - half);
				const Real distance(norm(delta));
				*output.cursor(x, y) = -sinc(distance * scaleSincFreq) *
					filterWindow(distance * scaleWindowFreq);

				sum += *output.cursor(x, y);
			}
		}

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				*output.cursor(x, y) /= sum;
			}
		}
	}

	template <
		typename FilterWindow,
		typename Real,
		typename Output_View>
	void setFilter(
		FilterWindow filterWindow,
		const View<2, Real, Output_View>& output)
	{
		const integer width = output.width();
		const integer height = output.height();

		if (width == 0 ||
			height == 0)
		{
			return;
		}

		ENSURE1(odd(width), width);
		ENSURE1(odd(height), height);
		ENSURE_OP(width, ==, height);

		const Vector<Real, 2> half(
			(width - 1) / 2, (height - 1) / 2);
		const Real scaleWindowFreq = (Real)1 / half[0];

		Real sum(0);
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				const Vector<Real, 2> delta(
					Vector<Real, 2>(x, y) - half);
				const Real distance(norm(delta));
				*output.cursor(x, y) = filterWindow(
					distance * scaleWindowFreq);

				sum += *output.cursor(x, y);
			}
		}

		/*
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				*output.cursor(x, y) /= sum;
			}
		}
		*/
	}

	template <typename Real>
	Real boxWindow(const Real& distance)
	{
		PENSURE_OP(distance, >=, 0);
		if (distance > 1)
		{
			return 0;
		}

		return 1;
	}

	template <typename Real>
	Real bartlettWindow(const Real& distance)
	{
		PENSURE_OP(distance, >=, 0);
		if (distance > 1)
		{
			return 0;
		}

		return 1 - distance;
	}

	template <typename Real>
	Real hanningWindow(const Real& distance)
	{
		PENSURE_OP(distance, >=, 0);
		if (distance > 1)
		{
			return 0;
		}

		return 0.5 *
			(1 + std::cos(2 * constantPi<Real>() * distance));
	}

	template <typename Real>
	Real hammingWindow(const Real& distance)
	{
		PENSURE_OP(distance, >=, 0);
		if (distance > 1)
		{
			return 0;
		}

		return 0.54 +
			0.46 * std::cos(2 * constantPi<Real>() * distance);
	}

	template <typename Real>
	Real blackmanWindow(const Real& distance)
	{
		PENSURE_OP(distance, >=, 0);
		if (distance > 1)
		{
			return 0;
		}

		const Real angle(2 * constantPi<Real>() * distance);
		return 0.42 +
			0.50 * std::cos(angle) +
			0.08 * std::cos(2 * angle);
	}

	template <typename Real>
	Real mitchellNetravali(const Real& distance)
	{
		PENSURE_OP(distance, >=, 0);
		if (distance > 1)
		{
			return 0;
		}

		static const Real B((Real)1 / 3);
		static const Real C((Real)1 / 3);

		const Real x(distance * 2);
		if (x < 1)
		{
			return (6 - 2 * B) +
				x * ((-18 + 12 * B + 6 * C) +
				x * x * (12 - 9 * B - 6 * C));
		}

		return (8 * B + 24 * C) +
			x * ((-12 * B - 48 * C) +
			x * ((6 * B + 30 * C) +
			x * (-B - 6 * C)));
	}

	template <typename Real>
	Real gaussian(const Real& distance,
		const PASTEL_NO_DEDUCTION(Real)& deviation)
	{
		PENSURE_OP(distance, >=, 0);
		PENSURE_OP(deviation, >, 0);
		if (distance > 1)
		{
			return 0;
		}

		static const Real InvSqrtTwoPi(
			inverse(std::sqrt(2 * constantPi<Real>())));
		static const Real InvTwo(inverse(Real(2)));

		const Real invVariance(inverse(deviation));
		const Real d(distance * invVariance);

		return InvSqrtTwoPi * invVariance
			* std::exp(-d * d * InvTwo);
	}

}

#endif
