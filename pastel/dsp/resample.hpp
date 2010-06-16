#ifndef PASTEL_RESAMPLE_HPP
#define PASTEL_RESAMPLE_HPP

#include "pastel/dsp/resample.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/smallset.h"

#include "pastel/sys/math_functions.h"

#include <vector>

namespace Pastel
{

	template <
		typename Computation_Element,
		typename Input_ConstIterator,
		typename Output_Iterator>
	void resample(
		const RandomAccessRange<Input_ConstIterator>& inputSet,
		const RandomAccessRange<Output_Iterator>& outputSet,
		const ConstFilterPtr& filter,
		const IndexExtenderPtr& indexExtender,
		const typename Input_ConstIterator::value_type& border,
		real blurFactor)
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
		typename Input_ConstIterator,
		typename Output_Iterator>
	void resampleTable(
		const RandomAccessRange<Input_ConstIterator>& inputSet,
		const RandomAccessRange<Output_Iterator>& outputSet,
		const ConstTableFilterPtr& filter,
		const IndexExtenderPtr& indexExtender,
		const typename Input_ConstIterator::value_type& border,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		const integer inputWidth = inputSet.size();
		const integer outputWidth = outputSet.size();

		if (inputWidth == 0 ||
			outputWidth == 0)
		{
			return;
		}

		const real xStep = (real)inputWidth / outputWidth;
		const real filterFactor = blurFactor * ((xStep > 1) ? xStep : 1);
		const real invFilterFactor = inverse(filterFactor);

		const real filterRadius = filter->radius() * filterFactor;

		real xFilter = 0.5 * xStep;

		// xFilter + xStep * marginWidth > filterRadius
		// =>
		// xStep * marginWidth > filterRadius - xFilter
		// => 
		// marginWidth > (filterRadius - xFilter) / xStep

		const integer introEnd = 
			clamp(
			(integer)std::ceil((filterRadius - xFilter) / xStep) + 1,
			0, outputWidth);
		const integer mainEnd =
			clamp(outputWidth - introEnd, 
			introEnd, outputWidth);

		for (integer x = 0;x < outputWidth;++x)
		{
			const integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			const integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			real xLocalFilter = 
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			real sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				const real weight =
					filter->evaluateInRange(xLocalFilter);

				if (i >= introEnd && i < mainEnd)
				{
					result += weight * input[i];
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
							input[(*indexExtender)(i, inputWidth)];
					}
				}

				sumWeights += weight;
			}

			output(x) = result / sumWeights;

			xFilter += xStep;
		}
	}

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Output_Element>
	void resample(
		const ConstSubArray<Input_Element, N>& input,
		const SubArray<Output_Element, N>& output,
		const ConstFilterPtr& filter,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		real blurFactor)
	{
		Pastel::resample(
			input, 
			output, 
			tableFilter(filter), 
			arrayExtender,
			blurFactor);
	}

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Output_Element>
	void resampleTable(
		const ConstSubArray<Input_Element, N>& input,
		const SubArray<Output_Element, N>& output,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstTableFilterPtr& filter,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		// The n-dimensional resampling is done as
		// n subsequent 1-dimensional resamplings.
		// If the filter radii are equal,
		// then it can be shown that the order
		// which maximizes performance is
		// ascending order in (radius[i] * newExtent[i] / oldExtent[i]).
		// If the filter radii are not equal,
		// then it can be shown that the same
		// condition gives an optimal order in many, but not all, cases.

		SubArray<Output_Element, N> output(outputRef);

		const integer dimension = input.dimension();
		ENSURE_OP(dimension, ==, output.dimension());

		if (dimension == 1)
		{
			Pastel::resample(
				randomAccessRange(input.begin(), input.end()),
				randomAccessRange(output.begin(), output.end()),
				filter,
				arrayExtender.extender(0),
				arrayExtender.border(),
				blurFactor);

			return;
		}

		// Find out the mostly-optimal order.

		SmallSet<Detail_Resample::AxisValue> axisSet;
		for (integer i = 0;i < N;++i)
		{
			axisSet.insert(
				Detail_Resample::AxisValue((filter->radius() * output.extent()[i]) 
				/ input.extent()[i], i));
		}

		// The first resampling is from the input array to
		// a temporary array.

		Vector<integer, N> extent = input.extent();
		extent[axisSet[0].axis_] = output.extent()[axisSet[0].axis_];
		Array<Computation_Element, N> tempArray(extent);

		{
			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender.extender(0), arrayExtender.border(), 
				filter, blurFactor);

			visitRows(
				input(), tempArray(), axisSet[0].axis_, resampleFunctor);
		}

		// The second resampling to the previous-to-last resampling
		// is done between temporary arrays.

		for (integer i = 1;i < dimension - 1;++i)
		{
			extent[axisSet[i].axis_] = output.extent()[axisSet[i].axis_];
			Array<Computation_Element, N> tempArray2(extent);

			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender.extender(i), arrayExtender.border(),
				filter, blurFactor);

			visitRows(tempArray(), tempArray2(), axisSet[i].axis_, resampleFunctor);

			tempArray.swap(tempArray2);
		}

		// The last resampling is done to the output array.

		{
			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender.extender(dimension - 1), arrayExtender.border(),
				filter, blurFactor);

			visitRows(tempArray(), output(), axisSet[dimension - 1].axis_, resampleFunctor);
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
		const PASTEL_NO_DEDUCTION((ArrayExtender<1, Input_Element>))& arrayExtender,
		const ConstTableFilterPtr& filter,
		const View<1, Output_Element, Output_View>& output,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		const integer inputWidth = input.width();
		const integer outputWidth = output.width();

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

		const real xStep = (real)inputWidth / outputWidth;
		const real filterFactor = blurFactor * ((xStep > 1) ? xStep : 1);
		const real invFilterFactor = inverse(filterFactor);

		const real filterRadius = filter->radius() * filterFactor;

		real xFilter = 0.5 * xStep;

		// xFilter + xStep * marginWidth > filterRadius
		// =>
		// xStep * marginWidth > filterRadius - xFilter
		// => 
		// marginWidth > (filterRadius - xFilter) / xStep

		const integer introEnd = 
			clamp(
			(integer)std::ceil((filterRadius - xFilter) / xStep) + 1,
			0, outputWidth);
		const integer mainEnd =
			clamp(outputWidth - introEnd, 
			introEnd, outputWidth);

		for (integer x = 0;x < introEnd;++x)
		{
			const integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			const integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			real xLocalFilter = 
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			real sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				const real weight =
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
			const integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			const integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			real xLocalFilter = 
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			real sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				const real weight =
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
			const integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			const integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			real xLocalFilter = 
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			real sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				const real weight =
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
		const PASTEL_NO_DEDUCTION((ArrayExtender<1, Input_Element>))& arrayExtender,
		const ConstFilterPtr& filter,
		const View<1, Output_Element, Output_View>& output,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		Pastel::resampleTable<Computation_Element>(
			input, arrayExtender, 
			tableFilter(filter), output,
			blurFactor);
	}

	namespace Detail_Resample
	{

		template <typename Computation_Element, typename Input_Element>
		class ResampleFunctor
		{
		public:
			ResampleFunctor(
				const ArrayExtender<1, Input_Element>& arrayExtender,
				const ConstTableFilterPtr& filter,
				real blurFactor)
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
			const real blurFactor_;
		};

		class AxisValue
		{
		public:
			AxisValue(real value,
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

			real value_;
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
		typename boost::enable_if_c<(N > 1), void>::type
		resampleTable(
		const ConstView<N, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstTableFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		real blurFactor)
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

		SmallSet<Detail_Resample::AxisValue> axisSet;
		for (integer i = 0;i < N;++i)
		{
			axisSet.insert(
				Detail_Resample::AxisValue((filter->radius() * output.extent()[i]) 
				/ input.extent()[i], i));
		}

		// The first resampling is from the input view to
		// a temporary array.

		Vector<integer, N> extent = input.extent();
		extent[axisSet[0].axis_] = output.extent()[axisSet[0].axis_];
		Array<Computation_Element, N> tempArray(extent);

		{
			const ArrayExtender<1, Input_Element> arrayExtender1D(
				arrayExtender.extender(0), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender1D, filter, blurFactor);
			visitRows(input, arrayView(tempArray), axisSet[0].axis_, resampleFunctor);
		}

		// The second resampling to the previous-to-last resampling
		// is done between temporary arrays.

		for (integer i = 1;i < N - 1;++i)
		{
			extent[axisSet[i].axis_] = output.extent()[axisSet[i].axis_];
			Array<Computation_Element, N> tempArray2(extent);

			const ArrayExtender<1, Computation_Element> arrayExtender1D(
				arrayExtender.extender(i), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender1D, filter, blurFactor);

			visitRows(constArrayView(tempArray), arrayView(tempArray2), 
				axisSet[i].axis_, resampleFunctor);

			tempArray.swap(tempArray2);
		}

		// The last resampling is done to the output view.

		{
			const ArrayExtender<1, Computation_Element> arrayExtender1D(
				arrayExtender.extender(N - 1), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
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
		typename boost::enable_if_c<(N > 1), void>::type
		resample(
		const ConstView<N, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		Pastel::resampleTable<Computation_Element>(
			input, arrayExtender, 
			tableFilter(filter), output,
			blurFactor);
	}

}

#endif
