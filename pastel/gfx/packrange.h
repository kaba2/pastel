// Description: Packing of a scalar image to [0, 1] range
// Documentation: undocumented.txt

#ifndef PASTELGFX_PACKRANGE_H
#define PASTELGFX_PACKRANGE_H

#include "pastel/sys/array.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	namespace PackRange_
	{

		template <typename Real>
		class MinMaxFunctor
		{
		public:
			MinMaxFunctor(
				Real& min,
				Real& max)
				: min_(min)
				, max_(max)
			{
			}

			void operator()(const Real& that) const
			{
				if (that < min_)
				{
					min_ = that;
				}
				if (that > max_)
				{
					max_ = that;
				}
			}

		private:
			Real& min_;
			Real& max_;
		};

		template <typename Real>
		class ScalingFunctor
		{
		public:
			ScalingFunctor(
				const Real& min,
				const Real& newMin,
				const Real& scale)
				: min_(min)
				, newMin_(newMin)
				, scale_(scale)

			{
			}

			void operator()(Real& that) const
			{
				that = (that - min_) * scale_ + newMin_;
			}

		private:
			const Real& min_;
			const Real& newMin_;
			const Real& scale_;
		};
	}

	template <typename Real, int N, typename Image_View>
	void minMax(
		const ConstView<N, Real, Image_View>& image,
		Real& min,
		Real& max)
	{
		min = infinity<Real>();
		max = -infinity<Real>();

		PackRange_::MinMaxFunctor<Real> minMaxFunctor(min, max);
		visit(image, minMaxFunctor);
	}

	template <typename Real, int N, typename Image_View>
	void packRange(
		const View<N, Real, Image_View>& image,
		const PASTEL_NO_DEDUCTION(Real)& newMin,
		const PASTEL_NO_DEDUCTION(Real)& newMax)
	{
		ENSURE_OP(newMin, <=, newMax);

		Real minValue = 0;
		Real maxValue = 0;

		minMax(image, minValue, maxValue);

		Real valueDelta = maxValue - minValue;
		Real newValueDelta = newMax - newMin;
		if (valueDelta == 0 || newValueDelta == 0)
		{
			clear(newMax, image);
		}
		else
		{
			Real scale = newValueDelta / valueDelta;

			PackRange_::ScalingFunctor<Real> scalingFunctor(
				minValue, newMin, scale);

			visit(image, scalingFunctor);
		}
	}

}

#endif
