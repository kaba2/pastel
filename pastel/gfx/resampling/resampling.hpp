#ifndef PASTELGFX_RESAMPLING_HPP
#define PASTELGFX_RESAMPLING_HPP

#include "pastel/gfx/resampling.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view/view_tools.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/math_functions.h"

#include <vector>

namespace Pastel
{

	template <
		typename Computation_Element,
		typename Input_RandomAccessConstRange,
		typename Output_RandomAccessRange>
	void resampleRange(
		const Input_RandomAccessConstRange& inputSet,
		const Output_RandomAccessRange& outputSet,
		const ConstFilterPtr& filter,
		const IndexExtenderPtr& indexExtender,
		const ranges::range_value_t<Input_RandomAccessConstRange>& border,
		dreal blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		Pastel::resampleTable<Computation_Element>(
			inputSet,
			outputSet,
			tableFilter(filter), 
			indexExtender, 
			border,
			blurFactor);
	}

	template <
		typename Computation_Element,
		typename Input_RandomAccessConstRange,
		typename Output_RandomAccessRange>
	void resampleRangeTable(
		const Input_RandomAccessConstRange& inputSet,
		const Output_RandomAccessRange& outputSet,
		const ConstTableFilterPtr& filter,
		const IndexExtenderPtr& indexExtender,
		const ranges::range_value_t<Input_RandomAccessConstRange>& border,
		dreal blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		integer inputWidth = inputSet.size();
		integer outputWidth = outputSet.size();

		if (inputWidth == 0 ||
			outputWidth == 0)
		{
			return;
		}

		dreal xStep = (dreal)inputWidth / outputWidth;

		const dreal filterFactor = blurFactor * ((xStep > 1) ? xStep : 1);
		dreal invFilterFactor = inverse(filterFactor);

		const dreal filterRadius = filter->radius() * filterFactor;

		dreal xFilter = 0.5 * xStep;

		// xFilter + xStep * marginWidth > filterRadius
		// =>
		// xStep * marginWidth > filterRadius - xFilter
		// => 
		// marginWidth > (filterRadius - xFilter) / xStep

		integer introEnd = 
			clamp(
			(integer)std::ceil((filterRadius - xFilter) / xStep) + 1,
			0, outputWidth);
		integer mainEnd =
			clamp(outputWidth - introEnd, 
			introEnd, outputWidth);

		for (integer x = 0;x < outputWidth;++x)
		{
			integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			dreal xLocalFilter =
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			dreal sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				dreal weight =
					filter->evaluateInRange(xLocalFilter);

				if (i >= introEnd && i < mainEnd)
				{
					result += weight * inputSet[i];
				}
				else
				{
					if (indexExtender.empty())
					{
						result += weight * border;
					}
					else
					{
						result += weight *
							inputSet[(*indexExtender)(i, inputWidth)];
					}
				}

				sumWeights += weight;
			}

			outputSet[x] = result / sumWeights;

			xFilter += xStep;
		}
	}

}

// Old view-based.

namespace Pastel
{

	template <
		typename Computation_Element,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
	void resampleTable(
		const ConstView<1, Input_Element, Input_View>& input,
		const NoDeduction<ArrayExtender<1, Input_Element>>& arrayExtender,
		const ConstTableFilterPtr& filter,
		const View<1, Output_Element, Output_View>& output,
		dreal blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		integer inputWidth = input.width();
		integer outputWidth = output.width();

		if (inputWidth == 0 ||
			outputWidth == 0)
		{
			return;
		}

		/*
		if (outputWidth == inputWidth)
		{
			copy(input, output);
			return;
		}
		*/

		dreal xStep = (dreal)inputWidth / outputWidth;

		const dreal filterFactor = blurFactor * ((xStep > 1) ? xStep : 1);
		dreal invFilterFactor = inverse(filterFactor);

		const dreal filterRadius = filter->radius() * filterFactor;

		dreal xFilter = 0.5 * xStep;

		// xFilter + xStep * marginWidth > filterRadius
		// =>
		// xStep * marginWidth > filterRadius - xFilter
		// => 
		// marginWidth > (filterRadius - xFilter) / xStep

		integer introEnd = 
			clamp(
			(integer)std::ceil((filterRadius - xFilter) / xStep) + 1,
			0, outputWidth);
		integer mainEnd =
			clamp(outputWidth - introEnd, 
			introEnd, outputWidth);

		for (integer x = 0;x < introEnd;++x)
		{
			integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			dreal xLocalFilter =
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			dreal sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				dreal weight =
					filter->evaluateInRange(xLocalFilter);

				result += weight *
					arrayExtender(input, i);

				xLocalFilter -= invFilterFactor;
				sumWeights += weight;
			}

			output(x) = result / sumWeights;

			xFilter += xStep;
		}

		for (integer x = introEnd;x < mainEnd;++x)
		{
			integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			dreal xLocalFilter =
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			dreal sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				dreal weight =
					filter->evaluateInRange(xLocalFilter);
				
				Computation_Element in = input(i);

				result += weight * in;

				xLocalFilter -= invFilterFactor;
				sumWeights += weight;
			}

			output(x) = result / sumWeights;

			xFilter += xStep;
		}

		for (integer x = mainEnd;x < outputWidth;++x)
		{
			integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			dreal xLocalFilter =
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			dreal sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				dreal weight =
					filter->evaluateInRange(xLocalFilter);

				result += weight *
					arrayExtender(input, i);

				xLocalFilter -= invFilterFactor;
				sumWeights += weight;
			}

			output(x) = result / sumWeights;

			xFilter += xStep;
		}
	}

	template <
		typename Computation_Element,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		void resample(
		const ConstView<1, Input_Element, Input_View>& input,
		const NoDeduction<ArrayExtender<1, Input_Element>>& arrayExtender,
		const ConstFilterPtr& filter,
		const View<1, Output_Element, Output_View>& output,
		dreal blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		Pastel::resampleTable<Computation_Element>(
			input, arrayExtender, 
			tableFilter(filter), output,
			blurFactor);
	}

	namespace Resample_
	{

		template <typename Computation_Element, typename Input_Element>
		class ResampleFunctor
		{
		public:
			ResampleFunctor(
				const ArrayExtender<1, Input_Element>& arrayExtender,
				const ConstTableFilterPtr& filter,
				dreal blurFactor)
			: arrayExtender_(arrayExtender)
			, filter_(filter)
			, blurFactor_(blurFactor)
			{
			}

			template <typename Left_View, typename Right_View>
			void operator()(const Left_View& left,
				const Right_View& right) const
			{
				resample<Computation_Element>(left, arrayExtender_, 
					filter_, right, blurFactor_);
			}

		private:
			const ArrayExtender<1, Input_Element>& arrayExtender_;
			const ConstTableFilterPtr& filter_;
			dreal blurFactor_;
		};

		class AxisValue
		{
		public:
			AxisValue(dreal value,
				integer id)
				: value_(value)
				, axis_(id)
			{
			}

			bool operator<(const AxisValue& that) const
			{
				if (value_ < that.value_)
				{
					return true;
				}
				if (value_ > that.value_)
				{
					return false;
				}
				return axis_ < that.axis_;
			}

			dreal value_;
			integer axis_;
		};

	}

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		requires (N > 1)
	void resampleTable(
		const ConstView<N, Input_Element, Input_View>& input,
		const NoDeduction<ArrayExtender<N, Input_Element>>& arrayExtender,
		const ConstTableFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		dreal blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		// The n-dimensional resampling is done as
		// n subsequent 1-dimensional resamplings.
		// If the filter radii are equal,
		// then it can be shown (as we will shortly) that the order
		// which maximizes performance is
		// ascending order in (radius[i] * newExtent[i] / oldExtent[i]).
		// If the filter radii are not equal,
		// then it can be shown that the same
		// heuristic gives an optimal order in many, but not all, cases.

		PASTEL_STATIC_ASSERT(N != 1);

		// Find out the mostly-optimal order.

		std::vector<Resample_::AxisValue> axisSet;
		axisSet.reserve(N);
		for (integer i = 0;i < N;++i)
		{
			axisSet.push_back(
				Resample_::AxisValue((filter->radius() * output.extent()[i]) 
				/ input.extent()[i], i));
		}

		std::sort(axisSet.begin(), axisSet.end());

		// The first resampling is from the input view to
		// a temporary array.

		Vector<integer, N> extent = input.extent();
		extent[axisSet[0].axis_] = output.extent()[axisSet[0].axis_];
		Array<Computation_Element, N> tempArray(extent);

		{
			ArrayExtender<1, Input_Element> arrayExtender1D(
				arrayExtender.extender(0), arrayExtender.border());
			Resample_::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender1D, filter, blurFactor);
			visitRows(input, arrayView(tempArray), axisSet[0].axis_, resampleFunctor);
		}

		// The second resampling to the previous-to-last resampling
		// is done between temporary arrays.

		for (integer i = 1;i < N - 1;++i)
		{
			extent[axisSet[i].axis_] = output.extent()[axisSet[i].axis_];
			Array<Computation_Element, N> tempArray2(extent);

			ArrayExtender<1, Computation_Element> arrayExtender1D(
				arrayExtender.extender(i), arrayExtender.border());
			Resample_::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender1D, filter, blurFactor);

			visitRows(constArrayView(tempArray), arrayView(tempArray2), 
				axisSet[i].axis_, resampleFunctor);

			tempArray.swap(tempArray2);
		}

		// The last resampling is done to the output view.

		{
			ArrayExtender<1, Computation_Element> arrayExtender1D(
				arrayExtender.extender(N - 1), arrayExtender.border());
			Resample_::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender1D, filter, blurFactor);

			visitRows(constArrayView(tempArray), output, axisSet[N - 1].axis_, resampleFunctor);
		}
	}

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		requires (N > 1)
	void resample(
		const ConstView<N, Input_Element, Input_View>& input,
		const NoDeduction<ArrayExtender<N, Input_Element>>& arrayExtender,
		const ConstFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		dreal blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		Pastel::resampleTable<Computation_Element>(
			input, arrayExtender, 
			tableFilter(filter), output,
			blurFactor);
	}

}

#endif
