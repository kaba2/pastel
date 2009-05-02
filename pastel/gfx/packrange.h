#ifndef PASTELGFX_PACKRANGE_H
#define PASTELGFX_PACKRANGE_H

#include "pastel/sys/array.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	namespace Detail_PackRange
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

	template <int N, typename Real, typename Image_View>
	void minMax(
		const ConstView<N, Real, Image_View>& image,
		Real& min,
		Real& max)
	{
		min = infinity<Real>();
		max = -infinity<Real>();

		Detail_PackRange::MinMaxFunctor<Real> minMaxFunctor(min, max);
		visit(image, minMaxFunctor);
	}

	template <int N, typename Real, typename Image_View>
	void packRange(
		const View<N, Real, Image_View>& image,
		const PASTEL_NO_DEDUCTION(Real)& newMin,
		const PASTEL_NO_DEDUCTION(Real)& newMax)
	{
		ENSURE2(newMin <= newMax, newMin, newMax);

		Real minValue = 0;
		Real maxValue = 0;

		minMax(image, minValue, maxValue);

		const Real valueDelta = maxValue - minValue;
		const Real newValueDelta = newMax - newMin;
		if (valueDelta == 0 || newValueDelta == 0)
		{
			clear(newMax, image);
		}
		else
		{
			const Real scale = newValueDelta / valueDelta;

			Detail_PackRange::ScalingFunctor<Real> scalingFunctor(
				minValue, newMin, scale);

			visit(image, scalingFunctor);
		}
	}

}

#endif
